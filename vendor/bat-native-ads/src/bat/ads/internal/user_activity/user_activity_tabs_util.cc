/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"

#include "base/time/time.h"
#include "bat/ads/internal/user_activity/user_activity_event_types.h"

namespace ads {

namespace {
// TODO(Moritz Haller): Define somewhere as alias
// constexpr int64_t kNotApplicableValue = -1;
constexpr int kMissingValue = -1;
}  // namespace

// TODO(Moritz Haller): Remove
int GetNumberOfTabsOpened(const UserActivityEventList& events) {
  const int count = std::count_if(
      events.cbegin(), events.cend(), [](const UserActivityEventInfo& event) {
        return event.type == UserActivityEventType::kOpenedNewTab;
      });

  return count;
}

// TODO(Moritz Haller): pass enum as const?
int GetNumberOfEvents(const UserActivityEventList& events, UserActivityEventType event_type) {
  const int count = std::count_if(
      events.cbegin(), events.cend(), [event_type](const UserActivityEventInfo& event) {
        return event.type == event_type;
      });

  return count;
}

// TODO(Moritz Haller): Can we make this more efficient?
int GetTimeSinceLastEvent(const UserActivityEventList& events, UserActivityEventType event_type) {
  const auto iter = std::find_if(
      events.crbegin(), events.crend(), [event_type](const UserActivityEventInfo& event) {
        return event.type == event_type;
      });

  if (iter == events.crend()) {
    return kMissingValue;
  }

  const base::TimeDelta time_delta = base::Time::Now() - iter->created_at;
  return (int) time_delta.InSeconds();
  // TODO(Moritz Haller): Add int64 support to data store and remove cast
}

}  // namespace ads
