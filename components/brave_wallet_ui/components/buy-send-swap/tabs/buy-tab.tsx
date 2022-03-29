import * as React from 'react'
import {
  UserAccountType,
  BuySendSwapViewTypes,
  BraveWallet,
  DefaultCurrencies, BuyServiceId
} from '../../../constants/types'

import {
  AccountsAssetsNetworks,
  Header,
  Buy
} from '..'

import { getUniqueAssets, isSelectedAssetInAssetOptions } from '../../../utils/asset-utils'

export interface Props {
  networkList: BraveWallet.NetworkInfo[]
  selectedNetwork: BraveWallet.NetworkInfo
  assetOptions: BraveWallet.BlockchainToken[]
  buyAmount: string
  showHeader?: boolean
  defaultCurrencies: DefaultCurrencies
  onSubmit: (asset: BraveWallet.BlockchainToken) => void
  onSelectNetwork: (network: BraveWallet.NetworkInfo) => void
  onSelectAccount: (account: UserAccountType) => void
  onSetBuyAmount: (value: string) => void
  onAddNetwork: () => void
  onAddAsset: () => void
  selectedBuyOption: BuyServiceId
  onSelectBuyOption: (optionId: BuyServiceId) => void
  wyreAssetOptions: BraveWallet.BlockchainToken[]
  rampAssetOptions: BraveWallet.BlockchainToken[]
}

function BuyTab (props: Props) {
  const {
    networkList,
    selectedNetwork,
    buyAmount,
    showHeader,
    assetOptions,
    defaultCurrencies,
    onAddAsset,
    onSubmit,
    onSelectNetwork,
    onSelectAccount,
    onSetBuyAmount,
    onAddNetwork,
    selectedBuyOption,
    onSelectBuyOption,
    wyreAssetOptions,
    rampAssetOptions
  } = props
  const [buyView, setBuyView] = React.useState<BuySendSwapViewTypes>('buy')
  const [selectedAsset, setSelectedAsset] = React.useState<BraveWallet.BlockchainToken>(assetOptions[0])

  const onChangeBuyView = (view: BuySendSwapViewTypes) => {
    setBuyView(view)
  }

  const onClickSelectNetwork = (network: BraveWallet.NetworkInfo) => () => {
    onSelectNetwork(network)
    setBuyView('buy')
  }

  const onClickSelectAccount = (account: UserAccountType) => () => {
    onSelectAccount(account)
    setBuyView('buy')
  }

  const onSelectedAsset = (asset: BraveWallet.BlockchainToken) => () => {
    setSelectedAsset(asset)
    setBuyView('buy')
  }

  const onInputChange = (value: string, name: string) => {
    onSetBuyAmount(value)
  }

  const onSubmitBuy = () => {
    onSubmit(selectedAsset)
  }

  const goBack = () => {
    setBuyView('buy')
  }

  React.useEffect(() => {
    if (assetOptions.length > 0) {
      setSelectedAsset(assetOptions[0])
    }
  }, [assetOptions])

  const filteredAssetOptions = React.useMemo(() => {
    return getUniqueAssets(assetOptions)
  }, [assetOptions])

  const isAvailableOnWyre = React.useMemo(() => {
    return isSelectedAssetInAssetOptions(selectedAsset, wyreAssetOptions)
  }, [selectedAsset])

  const isAvailableOnRamp = React.useMemo(() => {
    return isSelectedAssetInAssetOptions(selectedAsset, rampAssetOptions)
  }, [selectedAsset])

  return (
    <>
      {buyView === 'buy' &&
        <>
          {showHeader &&
            <Header
              onChangeSwapView={onChangeBuyView}
            />
          }
          <Buy
            defaultCurrencies={defaultCurrencies}
            buyAmount={buyAmount}
            selectedAsset={selectedAsset}
            selectedNetwork={selectedNetwork}
            onChangeBuyView={onChangeBuyView}
            onInputChange={onInputChange}
            onSubmit={onSubmitBuy}
            networkList={networkList}
            isAvailableOnWyre={isAvailableOnWyre}
            isAvailableOnRamp={isAvailableOnRamp}
            selectedBuyOption={selectedBuyOption}
            onSelectBuyOption={onSelectBuyOption}
          />
        </>
      }
      {buyView !== 'buy' &&
        <AccountsAssetsNetworks
          goBack={goBack}
          assetOptions={filteredAssetOptions}
          onClickSelectAccount={onClickSelectAccount}
          onClickSelectNetwork={onClickSelectNetwork}
          onSelectedAsset={onSelectedAsset}
          selectedView={buyView}
          onAddNetwork={onAddNetwork}
          onAddAsset={onAddAsset}
        />
      }
    </>
  )
}

export default BuyTab
