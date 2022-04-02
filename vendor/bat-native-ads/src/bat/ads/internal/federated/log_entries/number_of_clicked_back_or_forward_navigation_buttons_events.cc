/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/log_entries/number_of_clicked_back_or_forward_navigation_buttons_events.h"

#include "bat/ads/internal/federated/covariate_logs_util.h"
#include "bat/ads/internal/user_activity/user_activity.h"
#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"
#include "bat/ads/pref_names.h"

namespace ads {

NumberOfClickedBackOrForwardNavigationButtonsEvent::
    NumberOfClickedBackOrForwardNavigationButtonsEvent() = default;

NumberOfClickedBackOrForwardNavigationButtonsEvent::
    ~NumberOfClickedBackOrForwardNavigationButtonsEvent() = default;

mojom::DataType NumberOfClickedBackOrForwardNavigationButtonsEvent::GetDataType()
    const {
  return mojom::DataType::kInt;
}

mojom::CovariateType
NumberOfClickedBackOrForwardNavigationButtonsEvent::GetCovariateType() const {
  return mojom::CovariateType::kNumberOfClickedBackOrForwardNavigationButtonsEvent;
}

std::string NumberOfClickedBackOrForwardNavigationButtonsEvent::GetValue() const {
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));

  return ToString(GetNumberOfEvents(events, ads::UserActivityEventType::kClickedBackOrForwardNavigationButtons));
}

}  // namespace ads
