import * as React from 'react'
import { BraveWallet } from '../../../constants/types'
import { SelectNetwork } from '../../shared'
// Styled Components
import {
  StyledWrapper,
  DropDown,
  NetworkButton,
  DropDownIcon
} from './style'

export interface Props {
  selectedNetwork: BraveWallet.NetworkInfo
  showNetworkDropDown: boolean
  onClick: () => void
}

function SelectNetworkDropdown (props: Props) {
  const { selectedNetwork, onClick, showNetworkDropDown } = props

  return (
    <StyledWrapper>
      <NetworkButton onClick={onClick}>{selectedNetwork.chainName} <DropDownIcon /></NetworkButton>
      {showNetworkDropDown &&
        <DropDown>
          <SelectNetwork />
        </DropDown>
      }
    </StyledWrapper >
  )
}

export default SelectNetworkDropdown
