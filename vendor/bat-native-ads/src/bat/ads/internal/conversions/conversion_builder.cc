/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/conversions/conversion_builder.h"

#include "base/time/time.h"
#include "bat/ads/internal/conversions/conversion_info.h"

namespace ads {

ConversionInfo BuildConversion(const mojom::SearchResultAdPtr& ad_mojom) {
  ConversionInfo conversion;

  conversion.creative_set_id = ad_mojom->creative_set_id;
  conversion.type = ad_mojom->conversion->type;
  conversion.url_pattern = ad_mojom->conversion->url_pattern;
  conversion.advertiser_public_key =
      ad_mojom->conversion->advertiser_public_key;
  conversion.observation_window = ad_mojom->conversion->observation_window;
  conversion.expire_at =
      base::Time::FromDoubleT(ad_mojom->conversion->expire_at);

  return conversion;
}

}  // namespace ads
