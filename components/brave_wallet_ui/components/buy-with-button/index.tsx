import * as React from 'react'

import {
  StyledWrapper,
  Button,
  CaratDown,
  Dropdown,
  Option
} from './style'
import { BuyOption } from '../../constants/types'
import { RampLogo, WyreLogo } from '../buy-send-swap/buy/style'

export interface Props {
  children?: React.ReactNode
  options: BuyOption[]
  value: string
  closeOnSelect?: boolean
  disabled?: boolean
  onSelect: (value: string) => void
}

const SelectBuy = (props: Props) => {
  const { children, options, value, closeOnSelect, disabled, onSelect } = props
  const [isOpen, setIsOpen] = React.useState(false)

  const onClick = () => {
    setIsOpen(!isOpen)
  }

  const onOptionSelect = (value: string) => {
    if (closeOnSelect) {
      setIsOpen(false)
    }

    onSelect(value)
  }

  return (
    <StyledWrapper>
      <Button onClick={onClick} disabled={disabled}>
        {children}
        {!disabled && <CaratDown/>}
      </Button>
      {!disabled && isOpen &&
          <Dropdown>
            {options.map(option =>
              <Option
                key={option.id}
                value={option.id}
                selected={value === option.id}
                onClick={() => onOptionSelect(option.id)}
              >
                {option.id === 'ramp' ? <RampLogo/> : <WyreLogo/>}
                {option.label}
              </Option>
            )}
          </Dropdown>
      }
    </StyledWrapper>
  )
}

SelectBuy.defaultProps = {
  closeOnSelect: true
}

export default SelectBuy
