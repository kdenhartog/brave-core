import {
  BraveWallet, BuyServiceId,
  UserAccountType
} from '../constants/types'

import { getBuyAssetUrl } from '../common/async/lib'

export function GetBuyOrFaucetUrl (selectedBuyOption: BuyServiceId, networkChainId: string, asset: BraveWallet.BlockchainToken, account: UserAccountType, buyAmount: string, currency: string): Promise<string> {
  return new Promise(async (resolve, reject) => {
    switch (networkChainId) {
      case BraveWallet.MAINNET_CHAIN_ID:
        getBuyAssetUrl(selectedBuyOption, BraveWallet.MAINNET_CHAIN_ID, account.address, asset.symbol, buyAmount, currency)
          .then(resolve)
          .catch(reject)
        break
      case BraveWallet.POLYGON_MAINNET_CHAIN_ID:
        getBuyAssetUrl(selectedBuyOption, BraveWallet.POLYGON_MAINNET_CHAIN_ID, account.address, asset.symbol, buyAmount, currency)
          .then(resolve)
          .catch(reject)
        break
      case BraveWallet.BINANCE_SMART_CHAIN_MAINNET_CHAIN_ID:
        getBuyAssetUrl(selectedBuyOption, BraveWallet.BINANCE_SMART_CHAIN_MAINNET_CHAIN_ID, account.address, asset.symbol, buyAmount, currency)
          .then(resolve)
          .catch(reject)
        break
      case BraveWallet.AVALANCHE_MAINNET_CHAIN_ID:
        getBuyAssetUrl(selectedBuyOption, BraveWallet.AVALANCHE_MAINNET_CHAIN_ID, account.address, asset.symbol, buyAmount, currency)
          .then(resolve)
          .catch(reject)
        break
      case BraveWallet.CELO_MAINNET_CHAIN_ID:
        getBuyAssetUrl(selectedBuyOption, BraveWallet.CELO_MAINNET_CHAIN_ID, account.address, asset.symbol, buyAmount, currency)
          .then(resolve)
          .catch(reject)
        break
      case BraveWallet.SOLANA_MAINNET:
        getBuyAssetUrl(selectedBuyOption, BraveWallet.SOLANA_MAINNET, account.address, asset.symbol, buyAmount, currency)
          .then(resolve)
          .catch(reject)
        break
      case BraveWallet.ROPSTEN_CHAIN_ID:
        resolve('https://faucet.dimensions.network/')
        break
      case BraveWallet.KOVAN_CHAIN_ID:
        resolve('https://github.com/kovan-testnet/faucet')
        break
      case BraveWallet.RINKEBY_CHAIN_ID:
        resolve('https://www.rinkeby.io/#faucet')
        break
      case BraveWallet.GOERLI_CHAIN_ID:
        resolve('https://goerli-faucet.slock.it/')
        break
      default:
        reject()
    }
  })
}
