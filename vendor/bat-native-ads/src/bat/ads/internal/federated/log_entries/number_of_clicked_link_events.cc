/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/log_entries/number_of_clicked_link_events.h"

#include "bat/ads/internal/federated/covariate_logs_util.h"
#include "bat/ads/internal/user_activity/user_activity.h"
#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"
#include "bat/ads/pref_names.h"

namespace ads {

NumberOfClickedLinkEvents::
    NumberOfClickedLinkEvents() = default;

NumberOfClickedLinkEvents::
    ~NumberOfClickedLinkEvents() = default;

mojom::DataType NumberOfClickedLinkEvents::GetDataType()
    const {
  return mojom::DataType::kInt;
}

mojom::CovariateType
NumberOfClickedLinkEvents::GetCovariateType() const {
  return mojom::CovariateType::kNumberOfClickedLinkEvents;
}

std::string NumberOfClickedLinkEvents::GetValue() const {
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));

  return ToString(GetNumberOfEvents(events, ads::UserActivityEventType::kClickedLink));
}

}  // namespace ads
