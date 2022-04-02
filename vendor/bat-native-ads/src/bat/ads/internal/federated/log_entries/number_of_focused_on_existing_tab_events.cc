/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/log_entries/number_of_focused_on_existing_tab_events.h"

#include "bat/ads/internal/federated/covariate_logs_util.h"
#include "bat/ads/internal/user_activity/user_activity.h"
#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"
#include "bat/ads/pref_names.h"

namespace ads {

NumberOfFocusedOnExistingTabEvents::
    NumberOfFocusedOnExistingTabEvents() = default;

NumberOfFocusedOnExistingTabEvents::
    ~NumberOfFocusedOnExistingTabEvents() = default;

mojom::DataType NumberOfFocusedOnExistingTabEvents::GetDataType()
    const {
  return mojom::DataType::kInt;
}

mojom::CovariateType
NumberOfFocusedOnExistingTabEvents::GetCovariateType() const {
  return mojom::CovariateType::kNumberOfFocusedOnExistingTabEvents;
}

std::string NumberOfFocusedOnExistingTabEvents::GetValue() const {
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));

  return ToString(GetNumberOfEvents(events, ads::UserActivityEventType::kFocusedOnExistingTab));
}

}  // namespace ads
