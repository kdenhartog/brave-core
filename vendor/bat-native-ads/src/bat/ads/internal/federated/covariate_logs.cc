/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/covariate_logs.h"

#include <utility>

#include "bat/ads/internal/logging.h"

#include "base/check.h"
#include "base/time/time.h"
#include "bat/ads/ads_client.h"
#include "bat/ads/internal/ads_client_helper.h"
#include "bat/ads/internal/federated/covariate_log_entry.h"
#include "bat/ads/internal/federated/log_entries/ad_notification_clicked_covariate_log_entry.h"
#include "bat/ads/internal/federated/log_entries/ad_notification_impression_served_at_covariate_log_entry.h"
#include "bat/ads/internal/federated/log_entries/ad_notification_locale_country_at_time_of_serving_covariate_log_entry.h"
#include "bat/ads/internal/federated/log_entries/ad_notification_number_of_tabs_opened_in_past_30_minutes_log_entry.h"
#include "bat/ads/internal/federated/log_entries/number_of_browser_did_become_active_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_browser_window_is_inactive_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_opened_new_tab_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_closed_tab_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_focused_on_existing_tab_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_clicked_back_or_forward_navigation_buttons_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_clicked_reload_button_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_clicked_link_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_typed_url_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_submitted_form_events.h"
#include "bat/ads/internal/federated/log_entries/number_of_played_media_events.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_browser_did_become_active_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_browser_window_is_active_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_browser_window_is_inactive_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_opened_new_tab_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_closed_tab_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_focused_on_existing_tab_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_clicked_reload_button_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_clicked_link_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_typed_url_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_submitted_form_event.h"
#include "bat/ads/internal/federated/log_entries/time_since_last_played_media_event.h"

namespace ads {

CovariateLogs::CovariateLogs() {
  SetCovariateLogEntry(std::make_unique<AdNotificationNumberOfTabsOpenedInPast30Minutes>());
  SetCovariateLogEntry(std::make_unique<AdNotificationLocaleCountryAtTimeOfServingCovariateLogEntry>());
  
  SetCovariateLogEntry(std::make_unique<NumberOfBrowserDidBecomeActiveEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfBrowserWindowIsInactiveEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfOpenedNewTabEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfClosedTabEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfFocusedOnExistingTabEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfClickedBackOrForwardNavigationButtonsEvent>());
  SetCovariateLogEntry(std::make_unique<NumberOfClickedReloadButtonEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfClickedLinkEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfTypedUrlEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfSubmittedFormEvents>());
  SetCovariateLogEntry(std::make_unique<NumberOfPlayedMediaEvents>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastBrowserDidBecomeActiveEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastBrowserWindowIsActiveEvents>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastBrowserWindowIsInactiveEvents>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastOpenedNewTabEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastClosedTabEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastFocusedOnExistingTabEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastClickedReloadButtonEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastClickedLinkEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastTypedUrlEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastSubmittedFormEvent>());
  SetCovariateLogEntry(std::make_unique<TimeSinceLastPlayedMediaEvent>());
}

CovariateLogs::~CovariateLogs() = default;

void CovariateLogs::SetCovariateLogEntry(
    std::unique_ptr<CovariateLogEntry> entry) {
  DCHECK(entry);
  mojom::CovariateType key = entry->GetCovariateType();
  covariate_log_entries_[key] = std::move(entry);
}

mojom::TrainingCovariatesPtr CovariateLogs::GetTrainingCovariates() const {
  mojom::TrainingCovariatesPtr training_covariates =
      mojom::TrainingCovariates::New();
  for (const auto& covariate_log_entry : covariate_log_entries_) {
    const CovariateLogEntry* entry = covariate_log_entry.second.get();
    DCHECK(entry);

    mojom::CovariatePtr covariate = mojom::Covariate::New();
    covariate->data_type = entry->GetDataType();
    covariate->covariate_type = entry->GetCovariateType();
    covariate->value = entry->GetValue();
    training_covariates->covariates.push_back(std::move(covariate));
  }

  return training_covariates;
}

void CovariateLogs::SetAdNotificationImpressionServedAt(
    const base::Time impression_served_at) {
  auto impression_served_at_covariate_log_entry =
      std::make_unique<AdNotificationImpressionServedAtCovariateLogEntry>();
  impression_served_at_covariate_log_entry->SetLastImpressionAt(
      base::Time::Now());
  SetCovariateLogEntry(std::move(impression_served_at_covariate_log_entry));
}

void CovariateLogs::SetAdNotificationWasClicked(bool was_clicked) {
  auto ad_notification_clicked_covariate_log_entry =
      std::make_unique<AdNotificationClickedCovariateLogEntry>();
  ad_notification_clicked_covariate_log_entry->SetClicked(was_clicked);
  SetCovariateLogEntry(std::move(ad_notification_clicked_covariate_log_entry));
}

void CovariateLogs::LogTrainingCovariates() {
  mojom::TrainingCovariatesPtr training_covariates = GetTrainingCovariates();
  AdsClientHelper::Get()->LogTrainingCovariates(std::move(training_covariates));
}

}  // namespace ads
