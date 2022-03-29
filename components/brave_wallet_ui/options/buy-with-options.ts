import { getLocale } from '$web-common/locale'
import { BuyOption } from '../constants/types'

export const BuyOptions = [
  {
    id: 'ramp',
    label: getLocale('braveWalletBuyWithRamp')
  },
  {
    id: 'wyre',
    label: getLocale('braveWalletBuyWithWyre')
  }
] as BuyOption[]
