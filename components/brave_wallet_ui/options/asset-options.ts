// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,I
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { BraveWallet } from '../constants/types'
import {
  ALGOIconUrl,
  BATIconUrl,
  BNBIconUrl,
  BTCIconUrl,
  ETHIconUrl,
  ZRXIconUrl,
  SOLIconUrl,
  FILECOINIconUrl,
  FEVRIconUrl
} from '../assets/asset-icons'
import {
  CeloIcon,
  FantomIcon,
  OptimismIcon
} from '../assets/network-icons'
import MoonCatIcon from '../assets/png-icons/mooncat.png'

export function makeNetworkAsset (network: BraveWallet.NetworkInfo) {
  let logo
  switch (true) {
    case network.chainId === BraveWallet.OPTIMISM_MAINNET_CHAIN_ID:
      logo = OptimismIcon
      break

    case network.chainId === BraveWallet.POLYGON_MAINNET_CHAIN_ID:
      logo = 'chrome://erc-token-images/matic.png'
      break

    case network.chainId === BraveWallet.BINANCE_SMART_CHAIN_MAINNET_CHAIN_ID:
      logo = BNBIconUrl
      break

    case network.chainId === BraveWallet.AVALANCHE_MAINNET_CHAIN_ID:
      logo = 'chrome://erc-token-images/avax.png'
      break

    case network.chainId === BraveWallet.FANTOM_MAINNET_CHAIN_ID:
      logo = FantomIcon
      break

    case network.chainId === BraveWallet.CELO_MAINNET_CHAIN_ID:
      logo = CeloIcon
      break

    case network.symbol.toUpperCase() === 'SOL':
      logo = SOLIconUrl
      break

    case network.symbol.toUpperCase() === 'FIL':
      logo = FILECOINIconUrl
      break

    case network.symbol.toUpperCase() === 'ETH':
      logo = 'chrome://erc-token-images/eth.png'
      break

    default:
      logo = ''
  }

  return {
    contractAddress: '',
    name: network.symbolName,
    symbol: network.symbol,
    logo: logo,
    isErc20: false,
    isErc721: false,
    decimals: network.decimals,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: network.chainId
  } as BraveWallet.BlockchainToken
}

export const ETH = {
  contractAddress: '',
  name: 'Ethereum',
  symbol: 'ETH',
  logo: ETHIconUrl,
  isErc20: false,
  isErc721: false,
  decimals: 18,
  visible: true,
  tokenId: '',
  coingeckoId: '',
  chainId: '0x1'
} as BraveWallet.BlockchainToken

export const BAT = {
  contractAddress: '0x0D8775F648430679A709E98d2b0Cb6250d2887EF',
  name: 'Basic Attention Token',
  symbol: 'BAT',
  logo: 'chrome://erc-token-images/bat.png',
  isErc20: true,
  isErc721: false,
  decimals: 18,
  visible: false,
  tokenId: '',
  coingeckoId: ''
} as BraveWallet.BlockchainToken

// Use only with storybook as dummy data.
export const NewAssetOptions: BraveWallet.BlockchainToken[] = [
  ETH,
  {
    contractAddress: '2',
    name: 'Basic Attention Token',
    symbol: 'BAT',
    logo: BATIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '3',
    name: 'Binance Coin',
    symbol: 'BNB',
    logo: BNBIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x3'
  },
  {
    contractAddress: '4',
    name: 'Bitcoin',
    symbol: 'BTC',
    logo: BTCIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '5',
    name: 'Algorand',
    symbol: 'ALGO',
    logo: ALGOIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '6',
    name: '0x',
    symbol: 'ZRX',
    logo: ZRXIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '7',
    name: 'AcclimatedMoonCats',
    symbol: 'AMC',
    logo: MoonCatIcon,
    isErc20: false,
    isErc721: true,
    decimals: 0,
    visible: true,
    tokenId: '0x42a5',
    coingeckoId: '',
    chainId: '0x1'
  }
]

// Use only with storybook as dummy data.
export const AccountAssetOptions: BraveWallet.BlockchainToken[] = [
  ETH,
  {
    contractAddress: '0x0D8775F648430679A709E98d2b0Cb6250d2887EF',
    name: 'Basic Attention Token',
    symbol: 'BAT',
    logo: BATIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '3',
    name: 'Binance Coin',
    symbol: 'BNB',
    logo: BNBIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 8,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '4',
    name: 'Bitcoin',
    symbol: 'BTC',
    logo: BTCIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 8,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '5',
    name: 'Algorand',
    symbol: 'ALGO',
    logo: ALGOIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 8,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  },
  {
    contractAddress: '0xE41d2489571d322189246DaFA5ebDe1F4699F498',
    name: '0x',
    symbol: 'ZRX',
    logo: ZRXIconUrl,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: '0x1'
  }
]

// Ramp Buy Asset Options
export const RampBuyAssetOptions: BraveWallet.BlockchainToken[] = [
  {
    contractAddress: '',
    name: 'Ethereum',
    symbol: 'ETH',
    logo: `chrome://wallet/${ETHIconUrl}`,
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.MAINNET_CHAIN_ID
  },
  {
    contractAddress: '',
    name: 'Avalanche',
    symbol: 'AVAX',
    logo: 'chrome://erc-token-images/avax.png',
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.AVALANCHE_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0x82030cdbd9e4b7c5bb0b811a61da6360d69449cc',
    name: 'RealFevr',
    symbol: 'BSC_FEVR',
    logo: `chrome://wallet/${FEVRIconUrl}`,
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.BINANCE_SMART_CHAIN_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '',
    name: 'CELO',
    symbol: 'CELO',
    logo: 'chrome://erc-token-images/celo.png',
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.CELO_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0xD8763CBa276a3738E6DE85b4b3bF5FDed6D6cA73',
    name: 'Celo Euro',
    symbol: 'CEUR',
    logo: 'chrome://erc-token-images/ceur.png',
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.CELO_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0x765DE816845861e75A25fCA122bb6898B8B1282a',
    name: 'Celo Dollar',
    symbol: 'CUSD',
    logo: 'chrome://erc-token-images/cusd.png',
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.CELO_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0x6b175474e89094c44da98b954eedeac495271d0f',
    name: 'DAI',
    symbol: 'DAI',
    logo: 'chrome://erc-token-images/dai.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.MAINNET_CHAIN_ID
  },
  {
    contractAddress: '',
    name: 'Polygon',
    symbol: 'MATIC',
    logo: 'chrome://erc-token-images/matic.png',
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.POLYGON_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0x8f3cf7ad23cd3cadbd9735aff958023239c6a063',
    name: 'DAI',
    symbol: 'MATIC_DAI',
    logo: 'chrome://erc-token-images/dai.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.POLYGON_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0x7ceB23fD6bC0adD59E62ac25578270cFf1b9f619',
    name: 'ETH',
    symbol: 'MATIC_ETH',
    logo: 'chrome://erc-token-images/eth.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.POLYGON_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0xbbba073c31bf03b8acf7c28ef0738decf3695683',
    name: 'Sandbox',
    symbol: 'MATIC_SAND',
    logo: 'chrome://erc-token-images/sand.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.POLYGON_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0x2791bca1f2de4661ed88a30c99a7a9449aa84174',
    name: 'USDC',
    symbol: 'MATIC_USDC',
    logo: 'chrome://erc-token-images/usdc.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.POLYGON_MAINNET_CHAIN_ID
  },
  {
    contractAddress: '',
    name: 'Solana',
    symbol: 'SOLANA_SOL',
    logo: `chrome://wallet/${SOLIconUrl}`,
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.SOLANA_MAINNET
  },
  {
    contractAddress: 'Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB',
    name: 'Tether',
    symbol: 'SOLANA_USDT',
    logo: 'chrome://erc-token-images/usdt.png',
    isErc20: false,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.SOLANA_MAINNET
  },
  {
    contractAddress: '0xdac17f958d2ee523a2206206994597c13d831ec7',
    name: 'Tether',
    symbol: 'USDT',
    logo: 'chrome://erc-token-images/usdt.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.MAINNET_CHAIN_ID
  },
  {
    contractAddress: '0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48',
    name: 'USDC',
    symbol: 'USDC',
    logo: 'chrome://erc-token-images/usdc.png',
    isErc20: true,
    isErc721: false,
    decimals: 18,
    visible: true,
    tokenId: '',
    coingeckoId: '',
    chainId: BraveWallet.MAINNET_CHAIN_ID
  }
]

export const RampSupportedCurrencies = ['USD', 'EUR', 'GBP']
