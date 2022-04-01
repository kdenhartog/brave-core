/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include <vector>

#include "base/strings/string_util.h"
#include "brave/components/brave_wallet/browser/permission_utils.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace brave_wallet {

TEST(PermissionUtilsUnitTest, GetConcatOriginFromWalletAddresses) {
  std::vector<std::string> addrs = {
      "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A",
      "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B"};
  GURL origin("https://test.com");
  GURL out_origin;
  GURL expected_out_origin(
      "https://"
      "test.com{addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A&addr="
      "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B}");
  EXPECT_TRUE(GetConcatOriginFromWalletAddresses(origin, addrs, &out_origin));
  EXPECT_EQ(out_origin, expected_out_origin);

  EXPECT_FALSE(
      GetConcatOriginFromWalletAddresses(GURL(""), addrs, &out_origin));
  EXPECT_FALSE(GetConcatOriginFromWalletAddresses(
      origin, std::vector<std::string>(), &out_origin));

  // Origin with port case:
  expected_out_origin = GURL(
      "https://"
      "test.com{addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A&addr="
      "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B}:123");

  EXPECT_TRUE(GetConcatOriginFromWalletAddresses(GURL("https://test.com:123"),
                                                 addrs, &out_origin));
  EXPECT_EQ(out_origin, expected_out_origin);
}

TEST(PermissionUtilsUnitTest, ParseRequestingOriginFromSubRequest) {
  struct {
    permissions::RequestType type;
    const char* invalid_origin;
    const char* invalid_origin_with_path;
    const char* valid_origin;
    const char* valid_origin_with_port;
    const char* account;
  } cases[]{
      {permissions::RequestType::kBraveEthereum, "https://test.com0x123",
       "https://test.com0x123/path",
       "https://test.com0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A",
       "https://test.com0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A:123",
       "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A"},
      {permissions::RequestType::kBraveSolana,
       "https://test.com--BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8",
       "https://test.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8/path",
       "https://test.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8",
       "https://test.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8:123",
       "BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8"}};

  std::string requesting_origin;
  std::string account;
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    // Invalid requesting_origin format:
    EXPECT_FALSE(ParseRequestingOriginFromSubRequest(
        cases[i].type, GURL(cases[i].invalid_origin), nullptr, nullptr))
        << "case: " << i;
    EXPECT_FALSE(ParseRequestingOriginFromSubRequest(
        cases[i].type, GURL(cases[i].invalid_origin_with_path), nullptr,
        nullptr))
        << "case: " << i;
    EXPECT_FALSE(ParseRequestingOriginFromSubRequest(cases[i].type, GURL(""),
                                                     nullptr, nullptr))
        << "case: " << i;
    // invalid type
    EXPECT_FALSE(ParseRequestingOriginFromSubRequest(
        permissions::RequestType::kGeolocation, GURL(cases[i].valid_origin),
        nullptr, nullptr))
        << "case: " << i;
    EXPECT_TRUE(ParseRequestingOriginFromSubRequest(
        cases[i].type, GURL(cases[i].valid_origin), &requesting_origin,
        &account))
        << "case: " << i;
    EXPECT_EQ(requesting_origin, "https://test.com");
    EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(account, cases[i].account))
        << "case: " << i;

    EXPECT_TRUE(ParseRequestingOriginFromSubRequest(
        cases[i].type, GURL(cases[i].valid_origin_with_port),
        &requesting_origin, &account))
        << "case: " << i;
    EXPECT_EQ(requesting_origin, "https://test.com:123");
    EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(account, cases[i].account))
        << "case: " << i;
  }

  // separator in domain would still work
  EXPECT_TRUE(ParseRequestingOriginFromSubRequest(
      permissions::RequestType::kBraveSolana,
      GURL("https://test__.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8"),
      &requesting_origin, &account));
  EXPECT_EQ(requesting_origin, "https://test__.com");
  EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(
      account, "BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8"));
  EXPECT_TRUE(ParseRequestingOriginFromSubRequest(
      permissions::RequestType::kBraveSolana,
      GURL("https://"
           "test__.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8:123"),
      &requesting_origin, &account));
  EXPECT_EQ(requesting_origin, "https://test__.com:123");
  EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(
      account, "BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8"));
}

TEST(PermissionUtilsUnitTest, ParseRequestingOrigin) {
  struct {
    permissions::RequestType type;
    const char* invalid_origin;
    const char* invalid_origin_with_path;
    const char* valid_origin;
    const char* valid_origin_with_port;
    const char* valid_origin_two_accounts;
    const char* valid_origin_two_accounts_with_port;
    const char* account1;
    const char* account2;
  } cases[]{
      {permissions::RequestType::kBraveEthereum, "https://test.com0x123",
       "https://test.com0x123/path",
       "https://test.com{addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A}",
       "https://test.com{addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A}:123",
       "https://"
       "test.com{addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A&addr="
       "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B}",
       "https://"
       "test.com{addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A&addr="
       "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B}:123",
       "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A",
       "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B"},
      {permissions::RequestType::kBraveSolana,
       "https://test.com--BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8",
       "https://test.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8/path",
       "https://test.com{addr=BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8}",
       "https://"
       "test.com{addr=BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8}:123",
       "https://"
       "test.com{addr=BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8&addr="
       "JDqrvDz8d8tFCADashbUKQDKfJZFobNy13ugN65t1wvV}",
       "https://"
       "test.com{addr=BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8&addr="
       "JDqrvDz8d8tFCADashbUKQDKfJZFobNy13ugN65t1wvV}:123",
       "BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8",
       "JDqrvDz8d8tFCADashbUKQDKfJZFobNy13ugN65t1wvV"}};

  std::string requesting_origin;
  std::string account;
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    // Invalid requesting_origin format:
    EXPECT_FALSE(ParseRequestingOrigin(
        cases[i].type, GURL(cases[i].invalid_origin), nullptr, nullptr))
        << "case: " << i;
    EXPECT_FALSE(ParseRequestingOrigin(cases[i].type,
                                       GURL(cases[i].invalid_origin_with_path),
                                       nullptr, nullptr))
        << "case: " << i;
    EXPECT_FALSE(
        ParseRequestingOrigin(cases[i].type, GURL(""), nullptr, nullptr))
        << "case: " << i;
    // invalid type
    EXPECT_FALSE(ParseRequestingOrigin(permissions::RequestType::kGeolocation,
                                       GURL(cases[i].valid_origin), nullptr,
                                       nullptr))
        << "case: " << i;

    std::queue<std::string> address_queue;

    // Origin without port:
    std::string requesting_origin;
    EXPECT_TRUE(ParseRequestingOrigin(cases[i].type,
                                      GURL(cases[i].valid_origin),
                                      &requesting_origin, &address_queue));
    EXPECT_EQ(requesting_origin, "https://test.com") << "case: " << i;
    EXPECT_EQ(address_queue.size(), 1u) << "case: " << i;
    EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(address_queue.front(),
                                                 cases[i].account1))
        << "case: " << i;

    EXPECT_TRUE(ParseRequestingOrigin(cases[i].type,
                                      GURL(cases[i].valid_origin_two_accounts),
                                      &requesting_origin, nullptr))
        << "case: " << i;
    EXPECT_EQ(requesting_origin, "https://test.com") << "case: " << i;

    // Origin with port:
    EXPECT_TRUE(ParseRequestingOrigin(cases[i].type,
                                      GURL(cases[i].valid_origin_with_port),
                                      &requesting_origin, nullptr))
        << "case: " << i;
    EXPECT_EQ(requesting_origin, "https://test.com:123") << "case: " << i;

    EXPECT_FALSE(ParseRequestingOrigin(
        cases[i].type, GURL(cases[i].valid_origin_two_accounts_with_port),
        &requesting_origin, &address_queue))
        << "Non-empty address_queue param should return false. case: " << i;

    address_queue = std::queue<std::string>();
    EXPECT_TRUE(ParseRequestingOrigin(
        cases[i].type, GURL(cases[i].valid_origin_two_accounts_with_port),
        &requesting_origin, &address_queue))
        << "case: " << i;
    EXPECT_EQ(requesting_origin, "https://test.com:123") << "case: " << i;
    EXPECT_EQ(address_queue.size(), 2u) << "case: " << i;
    EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(address_queue.front(),
                                                 cases[i].account1))
        << "case: " << i;
    address_queue.pop();
    EXPECT_TRUE(base::EqualsCaseInsensitiveASCII(address_queue.front(),
                                                 cases[i].account2))
        << "case: " << i;
  }
}

TEST(PermissionUtilsUnitTest, GetSubRequestOrigin) {
  GURL new_origin;
  GURL old_origin("https://test.com");
  GURL old_origin_with_port("https://test.com:123");

  struct {
    permissions::RequestType type;
    const char* account;
    const char* expected_new_origin;
    const char* expected_new_origin_with_port;
  } cases[] = {
      {permissions::RequestType::kBraveEthereum,
       "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B",
       "https://test.com0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B",
       "https://test.com0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B:123"},
      {permissions::RequestType::kBraveSolana,
       "BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8",
       "https://test.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8",
       "https://"
       "test.com__BrG44HdsEhzapvs8bEqzvkq4egwevS3fRE6ze2ENo6S8:123"}

  };
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    EXPECT_FALSE(GetSubRequestOrigin(cases[i].type, GURL(""), cases[i].account,
                                     &new_origin))
        << "case: " << i;
    // invalid type
    EXPECT_FALSE(GetSubRequestOrigin(permissions::RequestType::kGeolocation,
                                     old_origin, cases[i].account, &new_origin))
        << "case: " << i;
    EXPECT_FALSE(
        GetSubRequestOrigin(cases[i].type, old_origin, "", &new_origin))
        << "case: " << i;
    EXPECT_FALSE(GetSubRequestOrigin(cases[i].type, old_origin,
                                     cases[i].account, nullptr))
        << "case: " << i;

    EXPECT_TRUE(GetSubRequestOrigin(cases[i].type, old_origin, cases[i].account,
                                    &new_origin))
        << "case: " << i;
    EXPECT_EQ(new_origin, GURL(cases[i].expected_new_origin));
    EXPECT_TRUE(GetSubRequestOrigin(cases[i].type, old_origin_with_port,
                                    cases[i].account, &new_origin))
        << "case: " << i;
    EXPECT_EQ(new_origin, GURL(cases[i].expected_new_origin_with_port))
        << "case: " << i;
  }
}

TEST(PermissionUtilsUnitTest, GetConnectWithSiteWebUIURL) {
  GURL base_url("chrome://wallet-panel.top-chrome/");
  std::vector<std::string> addrs = {
      "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A",
      "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B"};
  std::string origin = "https://a.test.com:123";
  GURL url_out = GetConnectWithSiteWebUIURL(base_url, addrs, origin);
  EXPECT_EQ(url_out.spec(),
            "chrome://wallet-panel.top-chrome/"
            "?addr=0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8A&addr="
            "0xaf5Ad1E10926C0Ee4af4eDAC61DD60E853753f8B&origin=https://"
            "a.test.com:123&etld-plus-one=test.com#connectWithSite");
}

}  // namespace brave_wallet
