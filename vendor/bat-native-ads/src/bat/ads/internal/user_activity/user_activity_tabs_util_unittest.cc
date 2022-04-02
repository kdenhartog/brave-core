/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"

// TODO(Moritz Haller): right header for int64_t?
#include <cstdint>

#include "base/time/time.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_time_util.h"
#include "bat/ads/internal/user_activity/user_activity.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

namespace {
// constexpr int64_t kNotApplicableValue = -1;
constexpr int kMissingValue = -1;
}  // namespace

class BatAdsUserActivityTabsUtilTest : public UnitTestBase {
 protected:
  BatAdsUserActivityTabsUtilTest() = default;

  ~BatAdsUserActivityTabsUtilTest() override = default;
};

// TODO(Moritz Haller): Delete tests
TEST_F(BatAdsUserActivityTabsUtilTest, NoTabsOpened) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int number_of_tabs_opened = GetNumberOfTabsOpened(events);

  // Assert
  EXPECT_EQ(0, number_of_tabs_opened);
}

TEST_F(BatAdsUserActivityTabsUtilTest, TabsOpened) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);

  AdvanceClock(base::Minutes(30));

  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClosedTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int number_of_tabs_opened = GetNumberOfTabsOpened(events);

  // Assert
  EXPECT_EQ(2, number_of_tabs_opened);
}

TEST_F(BatAdsUserActivityTabsUtilTest, GetNumberOfEvents) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);

  AdvanceClock(base::Minutes(30));

  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClosedTab);
  AdvanceClock(base::Minutes(5));

  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int number_of_tabs_opened = GetNumberOfEvents(events, UserActivityEventType::kClickedLink);

  // Assert
  EXPECT_EQ(2, number_of_tabs_opened);
}

TEST_F(BatAdsUserActivityTabsUtilTest, GetNumberOfEventsForMissingEvent) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int number_of_tabs_opened =
      GetNumberOfEvents(events, UserActivityEventType::kClosedTab);

  // Assert
  EXPECT_EQ(0, number_of_tabs_opened);
}

TEST_F(BatAdsUserActivityTabsUtilTest, GetNumberOfEventsFromEmptyHistory) {
  // Arrange

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int number_of_tabs_opened =
      GetNumberOfEvents(events, UserActivityEventType::kClosedTab);

  // Assert
  EXPECT_EQ(0, number_of_tabs_opened);
}

TEST_F(BatAdsUserActivityTabsUtilTest, GetTimeSinceLastEvent) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kPlayedMedia);
  AdvanceClock(base::Minutes(30));

  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kPlayedMedia);
  AdvanceClock(base::Minutes(5));

  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kPlayedMedia);
  AdvanceClock(base::Minutes(5));

  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  AdvanceClock(base::Minutes(1));

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int time =
      GetTimeSinceLastEvent(events, UserActivityEventType::kPlayedMedia);

  // Assert
  const int64_t expected_time = 6 * base::Time::kSecondsPerMinute;
  EXPECT_EQ(expected_time, time);
}

TEST_F(BatAdsUserActivityTabsUtilTest, GetTimeSinceLastEventForMissingEvent) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int time =
      GetTimeSinceLastEvent(events, UserActivityEventType::kPlayedMedia);

  // Assert
  EXPECT_EQ(kMissingValue, time);
}

TEST_F(BatAdsUserActivityTabsUtilTest, GetTimeSinceLastEventFromEmptyHistory) {
  // Arrange

  // Act
  const UserActivityEventList events =
      UserActivity::Get()->GetHistoryForTimeWindow(base::Minutes(30));
  const int time =
      GetTimeSinceLastEvent(events, UserActivityEventType::kPlayedMedia);

  // Assert
  EXPECT_EQ(kMissingValue, time);
}

// TODO(Moritz Haller): Write tests for all types of events?

}  // namespace ads
