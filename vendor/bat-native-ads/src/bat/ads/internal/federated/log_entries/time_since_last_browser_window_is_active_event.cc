/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/log_entries/time_since_last_browser_window_is_active_event.h"

#include "bat/ads/internal/federated/covariate_logs_util.h"
#include "bat/ads/internal/user_activity/user_activity.h"
#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"
#include "bat/ads/pref_names.h"

namespace ads {

TimeSinceLastBrowserWindowIsActiveEvents::
    TimeSinceLastBrowserWindowIsActiveEvents() = default;

TimeSinceLastBrowserWindowIsActiveEvents::
    ~TimeSinceLastBrowserWindowIsActiveEvents() = default;

mojom::DataType TimeSinceLastBrowserWindowIsActiveEvents::GetDataType()
    const {
  return mojom::DataType::kInt;
}

mojom::CovariateType
TimeSinceLastBrowserWindowIsActiveEvents::GetCovariateType() const {
  return mojom::CovariateType::kTimeSinceLastBrowserWindowIsActiveEvents;
}

std::string TimeSinceLastBrowserWindowIsActiveEvents::GetValue() const {
  // TODO(Moritz Haller): Optimise? Get once?
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));

  return ToString(GetTimeSinceLastEvent(events, ads::UserActivityEventType::kBrowserDidBecomeActive));
}

}  // namespace ads
