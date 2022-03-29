// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

// Constants
import {
  BraveWallet,
  WalletAccountType
} from '../../constants/types'

// Options
import { makeNetworkAsset, RampBuyAssetOptions } from '../../options/asset-options'

// Hooks
import usePricing from './pricing'
import useBalance from './balance'
import { useIsMounted } from './useIsMounted'
import { useLib } from './useLib'

export default function useAssets (
  selectedAccount: WalletAccountType,
  networkList: BraveWallet.NetworkInfo[],
  selectedNetwork: BraveWallet.NetworkInfo,
  userVisibleTokensInfo: BraveWallet.BlockchainToken[],
  spotPrices: BraveWallet.AssetPrice[]
) {
  const isMounted = useIsMounted()
  const { getBuyAssets } = useLib()

  const { computeFiatAmount } = usePricing(spotPrices)
  const getBalance = useBalance(networkList)
  const nativeAsset = React.useMemo(
    () => makeNetworkAsset(selectedNetwork),
    [selectedNetwork]
  )

  const assetsByNetwork = React.useMemo(() => {
    if (!userVisibleTokensInfo) {
      return []
    }

    return userVisibleTokensInfo.filter((token) => token.chainId === selectedNetwork.chainId)
  }, [userVisibleTokensInfo, selectedNetwork])

  const [buyAssetOptions, setBuyAssetOptions] = React.useState<BraveWallet.BlockchainToken[]>([nativeAsset])
  const [wyreAssetOptions, setWyreAssetOptions] = React.useState<BraveWallet.BlockchainToken[]>([])
  const [rampAssetOptions] = React.useState<BraveWallet.BlockchainToken[]>(RampBuyAssetOptions)

  React.useEffect(() => {
    isMounted && getBuyAssets().then(tokens => {
      if (isMounted) {
        const registryTokens = tokens.map(token => ({
          ...token,
          chainId: BraveWallet.MAINNET_CHAIN_ID,
          logo: `chrome://erc-token-images/${token.logo}`
        }) as BraveWallet.BlockchainToken)
        setWyreAssetOptions(registryTokens)

        const allTokens = [...registryTokens, ...RampBuyAssetOptions]
        const currentNetworkTokens = allTokens.filter(token => token.chainId === selectedNetwork.chainId)
        setBuyAssetOptions(currentNetworkTokens)
      }
    }).catch(e => console.error(e))
  }, [selectedNetwork])

  const assetsByValueAndNetwork = React.useMemo(() => {
    if (!assetsByNetwork) {
      return []
    }

    if (!selectedAccount) {
      return []
    }

    return assetsByNetwork.sort(function (a, b) {
      const aBalance = getBalance(selectedAccount, a)
      const bBalance = getBalance(selectedAccount, b)

      const bFiatBalance = computeFiatAmount(bBalance, b.symbol, b.decimals)
      const aFiatBalance = computeFiatAmount(aBalance, a.symbol, a.decimals)

      return bFiatBalance.toNumber() - aFiatBalance.toNumber()
    })
  }, [selectedAccount, assetsByNetwork, getBalance, computeFiatAmount])

  return {
    sendAssetOptions: assetsByNetwork,
    buyAssetOptions,
    rampAssetOptions,
    wyreAssetOptions,
    panelUserAssetList: assetsByValueAndNetwork
  }
}
