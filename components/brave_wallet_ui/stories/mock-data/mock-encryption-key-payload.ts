import { BraveWallet } from '../../constants/types'

export const mockEncryptionKeyPayload: BraveWallet.GetEncryptionPublicKeyRequest = {
  address: '0x3f29A1da97149722eB09c526E4eAd698895b426',
  message: 'This is a test message.',
  origin: {
    url: 'https://app.skiff.org'
  }
}
