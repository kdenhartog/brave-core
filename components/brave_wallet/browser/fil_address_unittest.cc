/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include <vector>

#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "brave/components/brave_wallet/common/fil_address.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {

namespace {
bool ValidatePublicKey(const std::string& public_key_hex,
                       mojom::FilecoinAddressProtocol protocol,
                       const std::string& network,
                       const std::string& expected_address) {
  std::vector<uint8_t> public_key;
  auto result = base::HexStringToBytes(public_key_hex, &public_key);
  EXPECT_TRUE(result);
  auto address = FilAddress::FromPublicKey(public_key, protocol, network);
  EXPECT_FALSE(address.IsEmpty());
  EXPECT_EQ(address.ToChecksumAddress(), expected_address);
  return FilAddress::IsValidAddress(address.ToChecksumAddress());
}
}  // namespace

TEST(FilAddressUnitTest, From) {
  std::string address = "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q";
  EXPECT_EQ(FilAddress::From(address).ToChecksumAddress(), address);

  // Valid BLS address
  address = "f1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q";
  EXPECT_EQ(FilAddress::From(address).ToChecksumAddress(), address);
  EXPECT_FALSE(FilAddress::From(address).IsEmpty());

  // Valid secp256k1 address
  address =
      "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
      "oz4o4tpa4mvigcrayh4a";
  EXPECT_EQ(FilAddress::From(address).ToChecksumAddress(), address);

  address =
      "f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
      "oz4o4tpa4mvigcrayh4a";
  EXPECT_EQ(FilAddress::From(address).ToChecksumAddress(), address);

  address =
      "t3yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaby2smx7a";
  EXPECT_EQ(FilAddress::From(address).ToChecksumAddress(), address);

  // wrong size for SECP256K1 account
  address =
      "f1wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
      "oz4o4tpa4mvigcrayh4a";
  EXPECT_NE(FilAddress::From(address).ToChecksumAddress(), address);

  // wrong size for BLS account
  address = "t3h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q";
  EXPECT_NE(FilAddress::From(address).ToChecksumAddress(), address);

  // broken key
  address = "t1h3n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q";
  EXPECT_NE(FilAddress::From(address).ToChecksumAddress(), address);

  address = "";
  EXPECT_EQ(FilAddress::From(address).ToChecksumAddress(), address);
  EXPECT_TRUE(FilAddress::From(address).IsEmpty());
}

TEST(FilAddressUnitTest, IsValidAddress) {
  EXPECT_TRUE(
      FilAddress::IsValidAddress("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));
  EXPECT_TRUE(FilAddress::IsValidAddress(
      "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
      "oz4o4tpa4mvigcrayh4a"));
  EXPECT_TRUE(
      FilAddress::IsValidAddress("f1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));
  EXPECT_TRUE(FilAddress::IsValidAddress(
      "t3yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaby2smx7a"));
  EXPECT_FALSE(FilAddress::IsValidAddress(
      "f1wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
      "oz4o4tpa4mvigcrayh4a"));
  EXPECT_FALSE(
      FilAddress::IsValidAddress("t3h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));
  EXPECT_FALSE(
      FilAddress::IsValidAddress("t1h3n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));
  EXPECT_FALSE(FilAddress::IsValidAddress(""));
}

TEST(FilAddressUnitTest, FromPublicKey) {
  EXPECT_TRUE(ValidatePublicKey("3F666D84EFEC7BEA64C90135BFEF2D9A3D834380",
                                mojom::FilecoinAddressProtocol::SECP256K1,
                                mojom::kFilecoinTestnet,
                                "t1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq"));

  EXPECT_TRUE(ValidatePublicKey("3F666D84EFEC7BEA64C90135BFEF2D9A3D834380",
                                mojom::FilecoinAddressProtocol::SECP256K1,
                                mojom::kFilecoinMainnet,
                                "f1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq"));

  EXPECT_TRUE(ValidatePublicKey(
      "B5774F3D8546D3E797653A5423EFFA7AB06D4CD3587697D3647798D9FE739167EBEAF1EF"
      "053F957A7678EE4DE0E32A83",
      mojom::FilecoinAddressProtocol::BLS, mojom::kFilecoinTestnet,
      "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4t"
      "pa4mvigcrayh4a"));

  EXPECT_TRUE(ValidatePublicKey(
      "B5774F3D8546D3E797653A5423EFFA7AB06D4CD3587697D3647798D9FE739167EBEAF1EF"
      "053F957A7678EE4DE0E32A83",
      mojom::FilecoinAddressProtocol::BLS, mojom::kFilecoinMainnet,
      "f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4t"
      "pa4mvigcrayh4a"));

  auto empty_address = FilAddress::FromPublicKey(
      {}, mojom::FilecoinAddressProtocol::SECP256K1, mojom::kFilecoinTestnet);
  EXPECT_EQ(empty_address.ToChecksumAddress(), "");
  EXPECT_TRUE(empty_address.IsEmpty());

  empty_address = FilAddress::FromPublicKey(
      {}, mojom::FilecoinAddressProtocol::SECP256K1, mojom::kFilecoinMainnet);
  EXPECT_EQ(empty_address.ToChecksumAddress(), "");
  EXPECT_TRUE(empty_address.IsEmpty());

  empty_address = FilAddress::FromPublicKey(
      {}, mojom::FilecoinAddressProtocol::BLS, mojom::kFilecoinMainnet);
  EXPECT_EQ(empty_address.ToChecksumAddress(), "");
  EXPECT_TRUE(empty_address.IsEmpty());

  empty_address = FilAddress::FromPublicKey(
      {}, mojom::FilecoinAddressProtocol::BLS, mojom::kFilecoinTestnet);
  EXPECT_EQ(empty_address.ToChecksumAddress(), "");
  EXPECT_TRUE(empty_address.IsEmpty());

  // wrong key/protocol pair bls
  std::vector<uint8_t> public_bls_key;
  auto result = base::HexStringToBytes(
      "B5774F3D8546D3E797653A5423EFFA7AB06D4CD3587697D3647798D9FE739167EBEAF1EF"
      "053F957A7678EE4DE0E32A83",
      &public_bls_key);
  EXPECT_TRUE(result);

  empty_address = FilAddress::FromPublicKey(
      public_bls_key, mojom::FilecoinAddressProtocol::SECP256K1,
      mojom::kFilecoinTestnet);
  EXPECT_EQ(empty_address.ToChecksumAddress(), "");
  EXPECT_TRUE(empty_address.IsEmpty());

  // wrong key/protocol pair secp
  std::vector<uint8_t> public_secp_key;
  result = base::HexStringToBytes("3F666D84EFEC7BEA64C90135BFEF2D9A3D834380",
                                  &public_secp_key);
  EXPECT_TRUE(result);

  empty_address = FilAddress::FromPublicKey(
      public_bls_key, mojom::FilecoinAddressProtocol::SECP256K1,
      mojom::kFilecoinTestnet);
  EXPECT_EQ(empty_address.ToChecksumAddress(), "");
  EXPECT_TRUE(empty_address.IsEmpty());
}

TEST(FilAddressUnitTest, FromUncompressedPublicKey) {
  std::string uncompressed_key_hex =
      "04E11EE1369349B470BD324C27F2BE177929B258B1B1302E15E6D0A0DCD4BB7C9219405B"
      "B1B40835A4249D28CD6B98F649034A7D5A5E8F1C9A0F557C532BE08EAA";
  std::vector<uint8_t> public_secp_key;
  auto result = base::HexStringToBytes(uncompressed_key_hex, &public_secp_key);
  EXPECT_TRUE(result);
  auto uncompressed_key = FilAddress::FromUncompressedPublicKey(
      public_secp_key, mojom::FilecoinAddressProtocol::SECP256K1,
      mojom::kFilecoinTestnet);
  EXPECT_EQ(uncompressed_key.ToChecksumAddress(),
            "t1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy");

  uncompressed_key = FilAddress::FromUncompressedPublicKey(
      public_secp_key, mojom::FilecoinAddressProtocol::SECP256K1,
      mojom::kFilecoinMainnet);
  EXPECT_EQ(uncompressed_key.ToChecksumAddress(),
            "f1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy");

  uncompressed_key = FilAddress::FromUncompressedPublicKey(
      public_secp_key, mojom::FilecoinAddressProtocol::BLS,
      mojom::kFilecoinTestnet);
  EXPECT_EQ(uncompressed_key.ToChecksumAddress(), "");
  EXPECT_TRUE(uncompressed_key.IsEmpty());
  uncompressed_key = FilAddress::FromUncompressedPublicKey(
      {}, mojom::FilecoinAddressProtocol::SECP256K1, mojom::kFilecoinTestnet);
  EXPECT_EQ(uncompressed_key.ToChecksumAddress(), "");
  EXPECT_TRUE(uncompressed_key.IsEmpty());
}

TEST(FilAddressUnitTest, Comparison) {
  EXPECT_EQ(FilAddress::From("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"),
            FilAddress::From("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));

  EXPECT_NE(FilAddress::From("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"),
            FilAddress::From("f1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));
  EXPECT_NE(FilAddress::From("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"),
            FilAddress::From("t3h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"));
  EXPECT_NE(
      FilAddress::From("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q"),
      FilAddress::From(
          "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"));

  EXPECT_NE(
      FilAddress::From(
          "t1wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"),
      FilAddress::From(
          "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"));
  EXPECT_NE(
      FilAddress::From(
          "f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"),
      FilAddress::From(
          "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"));
  EXPECT_EQ(
      FilAddress::From(
          "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"),
      FilAddress::From(
          "t3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2"
          "oz4o4tpa4mvigcrayh4a"));
}

}  // namespace brave_wallet
