/**
 * Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.utils;

import android.app.Activity;
import android.text.TextUtils;
import android.util.Pair;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.vpn.BraveVpnNativeWorker;
import org.chromium.chrome.browser.vpn.models.BraveVpnPrefModel;
import org.chromium.chrome.browser.vpn.models.BraveVpnProfileCredentials;
import org.chromium.chrome.browser.vpn.models.BraveVpnWireguardProfileCredentials;
import org.chromium.chrome.browser.vpn.utils.BraveVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.BraveVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.BraveVpnUtils;
import org.chromium.chrome.browser.vpn.utils.InAppPurchaseWrapper;
import org.chromium.ui.widget.Toast;

import java.util.List;
import java.util.TimeZone;

public class BraveVpnApiResponseUtils {
    public static void queryPurchaseFailed(Activity activity) {
        Log.e("BraveVPN", "queryPurchaseFailed : 1");
        BraveVpnPrefUtils.setPurchaseToken("");
        BraveVpnPrefUtils.setProductId("");
        BraveVpnPrefUtils.setPurchaseExpiry(0L);
        BraveVpnPrefUtils.setSubscriptionPurchase(false);
        if (BraveVpnProfileUtils.getInstance().isBraveVPNConnected(activity)) {
            BraveVpnProfileUtils.getInstance().stopVpn(activity);
        }
        // BraveVpnProfileUtils.getInstance().deleteVpnProfile(activity);
        Toast.makeText(activity, R.string.purchase_token_verification_failed, Toast.LENGTH_LONG)
                .show();
        BraveVpnUtils.dismissProgressDialog();
        Log.e("BraveVPN", "queryPurchaseFailed : 2");
    }

    public static void handleOnGetSubscriberCredential(Activity activity, boolean isSuccess) {
        if (isSuccess) {
            Log.e("BraveVPN", "handleOnGetSubscriberCredential : 1");
            InAppPurchaseWrapper.getInstance().processPurchases(
                    activity, InAppPurchaseWrapper.getInstance().queryPurchases());
            BraveVpnNativeWorker.getInstance().getTimezonesForRegions();
        } else {
            Toast.makeText(activity, R.string.vpn_profile_creation_failed, Toast.LENGTH_SHORT)
                    .show();
            Log.e("BraveVPN", "handleOnGetSubscriberCredential : failed");
            BraveVpnUtils.dismissProgressDialog();
        }
    }

    public static void handleOnGetTimezonesForRegions(Activity activity,
            BraveVpnPrefModel braveVpnPrefModel, String jsonTimezones, boolean isSuccess) {
        if (isSuccess) {
            Log.e("BraveVPN", "handleOnGetTimezonesForRegions : 1");
            String region = BraveVpnUtils.getRegionForTimeZone(
                    jsonTimezones, TimeZone.getDefault().getID());
            Log.e("BraveVPN", "handleOnGetTimezonesForRegions : 2");
            if (!TextUtils.isEmpty(BraveVpnUtils.selectedServerRegion)
                    && BraveVpnUtils.selectedServerRegion != null) {
                region = BraveVpnUtils.selectedServerRegion.equals(
                                 BraveVpnPrefUtils.PREF_BRAVE_VPN_AUTOMATIC)
                        ? region
                        : BraveVpnUtils.selectedServerRegion;
                BraveVpnUtils.selectedServerRegion = null;
            } else {
                String serverRegion = BraveVpnPrefUtils.getServerRegion();
                region = serverRegion.equals(BraveVpnPrefUtils.PREF_BRAVE_VPN_AUTOMATIC)
                        ? region
                        : serverRegion;
            }

            Log.e("BraveVPN", "handleOnGetTimezonesForRegions : 3");

            BraveVpnNativeWorker.getInstance().getHostnamesForRegion(region);
            braveVpnPrefModel.setServerRegion(region);

            Log.e("BraveVPN", "handleOnGetTimezonesForRegions : 4");
        } else {
            Toast.makeText(activity, R.string.vpn_profile_creation_failed, Toast.LENGTH_LONG)
                    .show();
            Log.e("BraveVPN", "handleOnGetTimezonesForRegions : failed");
            BraveVpnUtils.dismissProgressDialog();
        }
    }

    public static Pair<String, String> handleOnGetHostnamesForRegion(Activity activity,
            BraveVpnPrefModel braveVpnPrefModel, String jsonHostNames, boolean isSuccess) {
        Pair<String, String> host = new Pair<String, String>("", "");
        if (isSuccess && braveVpnPrefModel != null) {
            Log.e("BraveVPN", "handleOnGetHostnamesForRegion : 1");
            host = BraveVpnUtils.getHostnameForRegion(jsonHostNames);
            Log.e("BraveVPN", "handleOnGetHostnamesForRegion : " + host);
            BraveVpnNativeWorker.getInstance().getWireguardProfileCredentials(
                    braveVpnPrefModel.getSubscriberCredential(),
                    braveVpnPrefModel.getClientPublicKey(), host.first);
            // BraveVpnNativeWorker.getInstance().getWireguardProfileCredentials(
            //         "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWJzY3JpcHRpb24tdHlwZSI6ImdyZF9wcm9feWVhcmx5Iiwic3Vic2NyaXB0aW9uLXR5cGUtcHJldHR5IjoiUHJvIiwic3Vic2NyaXB0aW9uLWV4cGlyYXRpb24tZGF0ZSI6MTczMjY1NDM1MywiZXhwIjoxNjQ1NDU4MjMxLCJpYXQiOjE2NDQ4NTM0MzF9.uwCmDwuuyfVuPhNG2Q0--h7WIU8wf_I-tgzrF4ercQ8thTrR4BeFJzVn_-moe8at05YimmjiUKay8NFxISdv1A",
            //         "1ibTph0fVcEaMmIBYFabLeR82sm3Vydyp/gQ87tJ9kA=",
            //         "wg-testing-1-yyz.guardianapp.com");
        } else {
            Toast.makeText(activity, R.string.vpn_profile_creation_failed, Toast.LENGTH_LONG)
                    .show();
            Log.e("BraveVPN", "handleOnGetHostnamesForRegion : failed");
            BraveVpnUtils.dismissProgressDialog();
        }
        return host;
    }

    public static void handleOnGetProfileCredentials(Activity activity,
            BraveVpnPrefModel braveVpnPrefModel, String jsonProfileCredentials, boolean isSuccess) {
        // if (isSuccess && braveVpnPrefModel != null) {
        //     Log.e("BraveVPN", "handleOnGetProfileCredentials : 1");
        //     BraveVpnProfileCredentials braveVpnProfileCredentials =
        //             BraveVpnUtils.getProfileCredentials(jsonProfileCredentials);
        //     if (BraveVpnProfileUtils.getInstance().isVPNConnected(activity)) {
        //         BraveVpnProfileUtils.getInstance().stopVpn(activity);
        //     }
        //     Log.e("BraveVPN", "handleOnGetProfileCredentials : 2");
        //     BraveVpnPrefUtils.setPrefModel(braveVpnPrefModel);
        //     try {
        //         Log.e("BraveVPN", "handleOnGetProfileCredentials : 3");
        //         // BraveVpnProfileUtils.getInstance().createVpnProfile(activity,
        //         //         braveVpnPrefModel.getHostname(),
        //         braveVpnProfileCredentials.getUsername(),
        //         //         braveVpnProfileCredentials.getPassword());
        //     } catch (Exception securityException) {
        //         Log.e("BraveVPN", "handleOnGetProfileCredentials : 4");
        //         BraveVpnProfileUtils.getInstance().startVpn(activity);
        //     }
        // } else {
        //     Toast.makeText(activity, R.string.vpn_profile_creation_failed, Toast.LENGTH_LONG)
        //             .show();
        //     Log.e("BraveVPN", "handleOnGetProfileCredentials : failed");
        //     BraveVpnUtils.dismissProgressDialog();
        // }
    }

    // public static void handleOnGetWireguardProfileCredentials(Activity activity,
    //         BraveVpnPrefModel braveVpnPrefModel, String jsonWireguardProfileCredentials, boolean
    //         isSuccess) {
    //     Log.e("BraveVPN", "handleOnGetWireguardProfileCredentials : 0");
    //     if (isSuccess && braveVpnPrefModel != null) {
    //         Log.e("BraveVPN", "handleOnGetWireguardProfileCredentials : 1");
    //         BraveVpnWireguardProfileCredentials braveVpnWireguardProfileCredentials =
    //                 BraveVpnUtils.getWireguardProfileCredentials(jsonWireguardProfileCredentials);
    //         if (BraveVpnProfileUtils.getInstance().isBraveVPNConnected(activity)) {
    //             BraveVpnProfileUtils.getInstance().stopVpn(activity);
    //         }
    //         Log.e("BraveVPN", "handleOnGetWireguardProfileCredentials : 2");
    //         BraveVpnPrefUtils.setPrefModel(braveVpnPrefModel);
    //         BraveVpnProfileUtils.getInstance().startVpn(activity);
    //         // try {
    //         //     Log.e("BraveVPN", "handleOnGetWireguardProfileCredentials : 3");
    //         //     // BraveVpnProfileUtils.getInstance().createVpnProfile(activity,
    //         //     //         braveVpnPrefModel.getHostname(),
    //         braveVpnProfileCredentials.getUsername(),
    //         //     //         braveVpnProfileCredentials.getPassword());
    //         // } catch (Exception securityException) {
    //         //     Log.e("BraveVPN", "handleOnGetWireguardProfileCredentials : 4");
    //         //     BraveVpnProfileUtils.getInstance().startVpn(activity);
    //         // }
    //     } else {
    //         Toast.makeText(activity, R.string.vpn_profile_creation_failed, Toast.LENGTH_LONG)
    //                 .show();
    //         Log.e("BraveVPN", "handleOnGetWireguardProfileCredentials : failed");
    //         BraveVpnUtils.dismissProgressDialog();
    //     }
    // }
}
