/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_VPN_BRAVE_VPN_SERVICE_H_
#define BRAVE_COMPONENTS_BRAVE_VPN_BRAVE_VPN_SERVICE_H_

#include <string>

#include "base/bind.h"
#include "base/callback_forward.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "base/sequence_checker.h"
#include "brave/components/api_request_helper/api_request_helper.h"
#include "brave/components/brave_vpn/brave_vpn.mojom.h"
#include "brave/components/skus/common/skus_sdk.mojom.h"
#include "build/build_config.h"
#include "components/keyed_service/core/keyed_service.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/remote_set.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "url/gurl.h"

#if !BUILDFLAG(IS_ANDROID)
#include <memory>
#include <vector>

#include "base/scoped_observation.h"
#include "base/timer/timer.h"
#include "brave/components/brave_vpn/brave_vpn_connection_info.h"
#include "brave/components/brave_vpn/brave_vpn_data_types.h"
#include "brave/components/brave_vpn/brave_vpn_os_connection_api.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#endif  // !BUILDFLAG(IS_ANDROID)

namespace network {
class SharedURLLoaderFactory;
}  // namespace network

#if !BUILDFLAG(IS_ANDROID)
namespace base {
class Value;
}  // namespace base

class PrefService;
#endif  // !BUILDFLAG(IS_ANDROID)

// This class is used by desktop and android.
// However, it includes desktop specific impls and it's hidden
// by IS_ANDROID ifdef.
class BraveVpnService :
#if !BUILDFLAG(IS_ANDROID)
    public brave_vpn::BraveVPNOSConnectionAPI::Observer,
#endif
    public brave_vpn::mojom::ServiceHandler,
    public KeyedService {
 public:
#if BUILDFLAG(IS_ANDROID)
  BraveVpnService(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      base::RepeatingCallback<mojo::PendingRemote<skus::mojom::SkusService>()>
          skus_service_getter);
#else
  BraveVpnService(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* prefs,
      base::RepeatingCallback<mojo::PendingRemote<skus::mojom::SkusService>()>
          skus_service_getter);
#endif
  ~BraveVpnService() override;

  BraveVpnService(const BraveVpnService&) = delete;
  BraveVpnService& operator=(const BraveVpnService&) = delete;

#if !BUILDFLAG(IS_ANDROID)
  void ToggleConnection();
  void RemoveVPNConnnection();

  bool is_connected() const {
    return connection_state_ == brave_vpn::mojom::ConnectionState::CONNECTED;
  }
  bool is_purchased_user() const {
    return purchased_state_ == brave_vpn::mojom::PurchasedState::PURCHASED;
  }
  brave_vpn::mojom::ConnectionState connection_state() const {
    return connection_state_;
  }

  void BindInterface(
      mojo::PendingReceiver<brave_vpn::mojom::ServiceHandler> receiver);
  void LoadPurchasedState();

  // mojom::vpn::ServiceHandler
  void GetConnectionState(GetConnectionStateCallback callback) override;
  void Connect() override;
  void Disconnect() override;
  void CreateVPNConnection() override;
  void GetAllRegions(GetAllRegionsCallback callback) override;
  void GetDeviceRegion(GetDeviceRegionCallback callback) override;
  void GetSelectedRegion(GetSelectedRegionCallback callback) override;
  void SetSelectedRegion(brave_vpn::mojom::RegionPtr region) override;
  void GetProductUrls(GetProductUrlsCallback callback) override;
  void CreateSupportTicket(const std::string& email,
                           const std::string& subject,
                           const std::string& body,
                           CreateSupportTicketCallback callback) override;
  void GetSupportData(GetSupportDataCallback callback) override;
#endif  // !BUILDFLAG(IS_ANDROID)

  using ResponseCallback =
      base::OnceCallback<void(const std::string&, bool success)>;

  // mojom::vpn::ServiceHandler
  void AddObserver(
      mojo::PendingRemote<brave_vpn::mojom::ServiceObserver> observer) override;
  void GetPurchasedState(GetPurchasedStateCallback callback) override;

  void GetAllServerRegions(ResponseCallback callback);
  void GetTimezonesForRegions(ResponseCallback callback);
  void GetHostnamesForRegion(ResponseCallback callback,
                             const std::string& region);
  void GetProfileCredentials(ResponseCallback callback,
                             const std::string& subscriber_credential,
                             const std::string& hostname);
  void GetSubscriberCredential(ResponseCallback callback,
                               const std::string& product_type,
                               const std::string& product_id,
                               const std::string& validation_method,
                               const std::string& purchase_token,
                               const std::string& bundle_id);
  void VerifyPurchaseToken(ResponseCallback callback,
                           const std::string& purchase_token,
                           const std::string& product_id,
                           const std::string& product_type,
                           const std::string& bundle_id);
  void GetSubscriberCredentialV12(ResponseCallback callback,
                                  const std::string& payments_environment,
                                  const std::string& monthly_pass);

 private:
#if !BUILDFLAG(IS_ANDROID)
  friend class BraveAppMenuBrowserTest;
  friend class BraveBrowserCommandControllerTest;
  friend class BraveVPNServiceTest;

  // brave_vpn::BraveVPNOSConnectionAPI::Observer overrides:
  void OnCreated() override;
  void OnCreateFailed() override;
  void OnRemoved() override;
  void OnConnected() override;
  void OnIsConnecting() override;
  void OnConnectFailed() override;
  void OnDisconnected() override;
  void OnIsDisconnecting() override;

  const brave_vpn::BraveVPNConnectionInfo& GetConnectionInfo();
  void LoadCachedRegionData();
  void LoadCachedSelectedRegion();
  void UpdateAndNotifyConnectionStateChange(
      brave_vpn::mojom::ConnectionState state);

  void FetchRegionData(bool background_fetch);
  void OnFetchRegionList(bool background_fetch,
                         const std::string& region_list,
                         bool success);
  bool ParseAndCacheRegionList(const base::Value& region_value);
  void OnFetchTimezones(const std::string& timezones_list, bool success);
  void ParseAndCacheDeviceRegionName(const base::Value& timezons_value);
  void FetchHostnamesForRegion(const std::string& name);
  void OnFetchHostnames(const std::string& region,
                        const std::string& hostnames,
                        bool success);
  void ParseAndCacheHostnames(const std::string& region,
                              base::Value hostnames_value);
  void SetDeviceRegion(const std::string& name);
  void SetFallbackDeviceRegion();
  void SetDeviceRegion(const brave_vpn::mojom::Region& region);
  void SetRegionToPrefs(const std::string& key,
                        const brave_vpn::mojom::Region& region);

  std::string GetCurrentTimeZone();
  void ScheduleBackgroundRegionDataFetch();
  void ScheduleFetchRegionDataIfNeeded();
  std::unique_ptr<brave_vpn::Hostname> PickBestHostname(
      const std::vector<brave_vpn::Hostname>& hostnames);

  void OnGetSubscriberCredentialV12(const std::string& subscriber_credential,
                                    bool success);
  void OnGetProfileCredentials(const std::string& profile_credential,
                               bool success);
  void OnCreateSupportTicket(
      CreateSupportTicketCallback callback,
      int status,
      const std::string& body,
      const base::flat_map<std::string, std::string>& headers);

  brave_vpn::BraveVPNOSConnectionAPI* GetBraveVPNConnectionAPI();
#endif  // !BUILDFLAG(IS_ANDROID)

  using URLRequestCallback =
      base::OnceCallback<void(int,
                              const std::string&,
                              const base::flat_map<std::string, std::string>&)>;

  // KeyedService overrides:
  void Shutdown() override;

  void OAuthRequest(
      const GURL& url,
      const std::string& method,
      const std::string& post_data,
      URLRequestCallback callback,
      const base::flat_map<std::string, std::string>& headers = {});

  void OnGetResponse(ResponseCallback callback,
                     int status,
                     const std::string& body,
                     const base::flat_map<std::string, std::string>& headers);

  void OnGetSubscriberCredential(
      ResponseCallback callback,
      int status,
      const std::string& body,
      const base::flat_map<std::string, std::string>& headers);

  void SetPurchasedState(brave_vpn::mojom::PurchasedState state);
  void EnsureMojoConnected();
  void OnMojoConnectionError();
  void OnCredentialSummary(const std::string& summary_string);
  void OnPrepareCredentialsPresentation(
      const std::string& credential_as_cookie);

#if !BUILDFLAG(IS_ANDROID)
  raw_ptr<PrefService> prefs_ = nullptr;
  std::vector<brave_vpn::mojom::Region> regions_;
  brave_vpn::mojom::Region device_region_;
  brave_vpn::mojom::Region selected_region_;
  std::unique_ptr<brave_vpn::Hostname> hostname_;
  brave_vpn::BraveVPNConnectionInfo connection_info_;
  bool cancel_connecting_ = false;
  brave_vpn::mojom::ConnectionState connection_state_ =
      brave_vpn::mojom::ConnectionState::DISCONNECTED;
  bool needs_connect_ = false;
  base::ScopedObservation<brave_vpn::BraveVPNOSConnectionAPI,
                          brave_vpn::BraveVPNOSConnectionAPI::Observer>
      observed_{this};
  mojo::ReceiverSet<brave_vpn::mojom::ServiceHandler> receivers_;
  base::RepeatingTimer region_data_update_timer_;

  // Only for testing.
  std::string test_timezone_;
  bool is_simulation_ = false;
#endif  // !BUILDFLAG(IS_ANDROID)

  SEQUENCE_CHECKER(sequence_checker_);

  base::RepeatingCallback<mojo::PendingRemote<skus::mojom::SkusService>()>
      skus_service_getter_;
  mojo::Remote<skus::mojom::SkusService> skus_service_;
  brave_vpn::mojom::PurchasedState purchased_state_ =
      brave_vpn::mojom::PurchasedState::NOT_PURCHASED;
  mojo::RemoteSet<brave_vpn::mojom::ServiceObserver> observers_;
  api_request_helper::APIRequestHelper api_request_helper_;
  std::string skus_credential_;
  base::WeakPtrFactory<BraveVpnService> weak_ptr_factory_{this};
};

#endif  // BRAVE_COMPONENTS_BRAVE_VPN_BRAVE_VPN_SERVICE_H_
