/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads/search_result_ads/search_result_ad.h"

#include "base/check.h"
#include "bat/ads/internal/account/deposits/deposit_builder.h"
#include "bat/ads/internal/account/deposits/deposit_info.h"
#include "bat/ads/internal/ad_events/ad_event.h"
#include "bat/ads/internal/ad_events/ad_event_info.h"
#include "bat/ads/internal/ad_events/ad_event_util.h"
#include "bat/ads/internal/ad_events/search_result_ads/search_result_ad_event_factory.h"
#include "bat/ads/internal/ads/search_result_ads/search_result_ad_builder.h"
#include "bat/ads/internal/ads/search_result_ads/search_result_ad_info.h"
#include "bat/ads/internal/ads/search_result_ads/search_result_ad_permission_rules.h"
#include "bat/ads/internal/conversions/conversion_builder.h"
#include "bat/ads/internal/conversions/conversion_info.h"
#include "bat/ads/internal/database/tables/ad_events_database_table.h"
#include "bat/ads/internal/database/tables/conversions_database_table.h"
#include "bat/ads/internal/database/tables/deposits_database_table.h"
#include "bat/ads/internal/logging.h"

namespace ads {

SearchResultAd::SearchResultAd() = default;

SearchResultAd::~SearchResultAd() = default;

void SearchResultAd::AddObserver(SearchResultAdObserver* observer) {
  DCHECK(observer);
  observers_.AddObserver(observer);
}

void SearchResultAd::RemoveObserver(SearchResultAdObserver* observer) {
  DCHECK(observer);
  observers_.RemoveObserver(observer);
}

void SearchResultAd::FireEvent(
    const std::string& placement_id,
    const mojom::SearchResultAdPtr& ad_mojom,
    const mojom::SearchResultAdEventType event_type) {
  const SearchResultAdInfo& ad = BuildSearchResultAd(ad_mojom, placement_id);

  if (placement_id.empty() || ad.creative_instance_id.empty()) {
    BLOG(1, "Failed to fire search result ad event due to invalid placement id "
                << placement_id << " or creative instance id "
                << ad.creative_instance_id);
    NotifySearchResultAdEventFailed(placement_id, ad.creative_instance_id,
                                    event_type);
    return;
  }

  search_result_ads::frequency_capping::PermissionRules permission_rules;
  if (!permission_rules.HasPermission()) {
    BLOG(1, "Search result ad: Not allowed due to permission rules");
    NotifySearchResultAdEventFailed(placement_id, ad.creative_instance_id,
                                    event_type);
    return;
  }

  const DepositInfo& deposit = BuildDeposit(ad_mojom);

  const ConversionInfo& conversion = BuildConversion(ad_mojom);

  database::table::Deposits deposits_database_table;
  deposits_database_table.Save(deposit, [=](const bool success) {
    if (!success) {
      BLOG(0, "Failed to save deposits state");
      NotifySearchResultAdEventFailed(placement_id, ad.creative_instance_id,
                                      event_type);
      return;
    }

    BLOG(3, "Successfully saved deposits state");

    database::table::Conversions conversion_database_table;
    conversion_database_table.Save({conversion}, [=](const bool success) {
      if (!success) {
        BLOG(0, "Failed to save conversions state");
        NotifySearchResultAdEventFailed(placement_id, ad.creative_instance_id,
                                        event_type);
        return;
      }

      BLOG(3, "Successfully saved conversions state");

      FireEvent(placement_id, ad, event_type);
    });
  });
}

///////////////////////////////////////////////////////////////////////////////

void SearchResultAd::FireEvent(
    const std::string& placement_id,
    const SearchResultAdInfo& ad,
    const mojom::SearchResultAdEventType event_type) {
  database::table::AdEvents database_table;
  database_table.GetForType(
      mojom::AdType::kSearchResultAd,
      [=](const bool success, const AdEventList& ad_events) {
        if (!success) {
          BLOG(1, "Search result ad: Failed to get ad events");
          NotifySearchResultAdEventFailed(placement_id, ad.creative_instance_id,
                                          event_type);
          return;
        }

        if (event_type == mojom::SearchResultAdEventType::kViewed &&
            HasFiredAdViewedEvent(ad, ad_events)) {
          BLOG(1,
               "Search result ad: Not allowed as already viewed placement id "
                   << placement_id);
          NotifySearchResultAdEventFailed(placement_id, ad.creative_instance_id,
                                          event_type);
          return;
        }

        if (event_type == mojom::SearchResultAdEventType::kViewed) {
          // We must fire an ad served event due to search result ads not being
          // delivered by the library
          FireEvent(placement_id, ad, mojom::SearchResultAdEventType::kServed);
        }

        const auto ad_event =
            search_result_ads::AdEventFactory::Build(event_type);
        ad_event->FireEvent(ad);

        NotifySearchResultAdEvent(ad, event_type);
      });
}

void SearchResultAd::NotifySearchResultAdEvent(
    const SearchResultAdInfo& ad,
    const mojom::SearchResultAdEventType event_type) const {
  switch (event_type) {
    case mojom::SearchResultAdEventType::kServed: {
      NotifySearchResultAdServed(ad);
      break;
    }

    case mojom::SearchResultAdEventType::kViewed: {
      NotifySearchResultAdViewed(ad);
      break;
    }

    case mojom::SearchResultAdEventType::kClicked: {
      NotifySearchResultAdClicked(ad);
      break;
    }
  }
}

void SearchResultAd::NotifySearchResultAdServed(
    const SearchResultAdInfo& ad) const {
  for (SearchResultAdObserver& observer : observers_) {
    observer.OnSearchResultAdServed(ad);
  }
}

void SearchResultAd::NotifySearchResultAdViewed(
    const SearchResultAdInfo& ad) const {
  for (SearchResultAdObserver& observer : observers_) {
    observer.OnSearchResultAdViewed(ad);
  }
}

void SearchResultAd::NotifySearchResultAdClicked(
    const SearchResultAdInfo& ad) const {
  for (SearchResultAdObserver& observer : observers_) {
    observer.OnSearchResultAdClicked(ad);
  }
}

void SearchResultAd::NotifySearchResultAdEventFailed(
    const std::string& placement_id,
    const std::string& creative_instance_id,
    const mojom::SearchResultAdEventType event_type) const {
  for (SearchResultAdObserver& observer : observers_) {
    observer.OnSearchResultAdEventFailed(placement_id, creative_instance_id,
                                         event_type);
  }
}

}  // namespace ads
