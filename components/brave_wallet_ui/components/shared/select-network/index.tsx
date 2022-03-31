import * as React from 'react'
import { useSelector } from 'react-redux'

import SelectNetworkItem from '../select-network-item'
import { BraveWallet, WalletState } from '../../../constants/types'

function SelectNetwork () {
  // redux
  const {
    networkList: networks,
    selectedNetwork
  } = useSelector((state: { wallet: WalletState }) => state.wallet)

  // MULTICHAIN: Remove me once we support SOL and FIL transaction creation.
  // Will be implemented in these 2 issues
  // https://github.com/brave/brave-browser/issues/20698
  // https://github.com/brave/brave-browser/issues/20893
  const networkList = React.useMemo(() => {
    return networks.filter(
      (network) =>
        network.coin !== BraveWallet.CoinType.SOL &&
        network.coin !== BraveWallet.CoinType.FIL
    )
  }, [networks])

  return (
    <>
      {networkList.map((network) =>
        <SelectNetworkItem
          selectedNetwork={selectedNetwork}
          key={`${network.chainId}-${network.coin}`}
          network={network}
        />
      )}
    </>
  )
}

export default SelectNetwork
