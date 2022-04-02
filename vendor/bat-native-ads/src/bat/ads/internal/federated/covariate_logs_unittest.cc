/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/covariate_logs.h"

#include "bat/ads/internal/user_activity/user_activity_tabs_util.h"

#include "base/time/time.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_time_util.h"
#include "bat/ads/internal/user_activity/user_activity.h"

// npm run test -- brave_unit_tests --filter=BatAdsCovariateLogsTest*

namespace ads {

class BatAdsCovariateLogsTest : public UnitTestBase {
 protected:
  BatAdsCovariateLogsTest() = default;

  ~BatAdsCovariateLogsTest() override = default;
};

TEST_F(BatAdsCovariateLogsTest, GetTrainingCovariatesNoUserActivity) {
  // Arrange

  // Act
  mojom::TrainingCovariatesPtr training_covariates =
      GetCovariateLogs()->GetTrainingCovariates();

  // Assert
  EXPECT_EQ(22U, training_covariates->covariates.size());
}

TEST_F(BatAdsCovariateLogsTest, GetTrainingCovariates) {
  // Arrange
  UserActivity::Get()->RecordEvent(UserActivityEventType::kBrowserDidBecomeActive);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kBrowserWindowIsInactive);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kOpenedNewTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClosedTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kFocusedOnExistingTab);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedBackOrForwardNavigationButtons);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedReloadButton);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kClickedLink);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kTypedUrl);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kSubmittedForm);
  UserActivity::Get()->RecordEvent(UserActivityEventType::kPlayedMedia);

  GetCovariateLogs()->SetAdNotificationImpressionServedAt(base::Time::Now());
  GetCovariateLogs()->SetAdNotificationWasClicked(true);

  // Act
  mojom::TrainingCovariatesPtr training_covariates =
      GetCovariateLogs()->GetTrainingCovariates();

  // Assert
  EXPECT_EQ(24U, training_covariates->covariates.size());
}

}  // namespace ads
