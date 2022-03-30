/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads/search_result_ads/search_result_ad.h"

#include <memory>

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_time_util.h"
#include "bat/ads/internal/unittest_util.h"
#include "bat/ads/public/interfaces/ads.mojom.h"
#include "bat/ads/search_result_ad_info.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsSearchResultAdTest : public SearchResultAdObserver,
                                 public UnitTestBase {
 protected:
  BatAdsSearchResultAdTest()
      : search_result_ad_(std::make_unique<SearchResultAd>()) {
    search_result_ad_->AddObserver(this);
  }

  ~BatAdsSearchResultAdTest() override = default;

  void OnSearchResultAdServed(const SearchResultAdInfo& ad) override {
    ad_ = ad;
    did_serve_ad_ = true;
  }

  void OnSearchResultAdViewed(const SearchResultAdInfo& ad) override {
    ad_ = ad;
    did_view_ad_ = true;
  }

  void OnSearchResultAdClicked(const SearchResultAdInfo& ad) override {
    ad_ = ad;
    did_click_ad_ = true;
  }

  void OnSearchResultAdEventFailed(
      const std::string& uuid,
      const std::string& creative_instance_id,
      const mojom::SearchResultAdEventType event_type) override {
    did_fail_to_fire_event_ = true;
  }

  std::unique_ptr<SearchResultAd> search_result_ad_;

  SearchResultAdInfo ad_;
  bool did_serve_ad_ = false;
  bool did_view_ad_ = false;
  bool did_click_ad_ = false;
  bool did_fail_to_fire_event_ = false;
};

TEST_F(BatAdsSearchResultAdTest, FireViewedEvent) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, FireClickedEvent) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, DoNotFireViewedEventIfAlreadyFired) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, DoNotFireEventWithInvalidPlacementId) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, DoNotFireEventWithInvalidCreativeInstanceId) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, DoNotFireEventWhenNotPermitted) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest,
       DoNotFireEventIfCreativeInstanceIdWasNotFound) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, FireEventIfNotExceededAdsPerHourCap) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, DoNotFireEventIfExceededAdsPerHourCap) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, FireEventIfNotExceededAdsPerDayCap) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BatAdsSearchResultAdTest, DoNotFireEventIfExceededAdsPerDayCap) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

}  // namespace ads
