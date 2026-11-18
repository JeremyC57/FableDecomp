# CRangeCompressor::CalcCompressionScript

The VC7.1 object exports `?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z`. The source-order record in Ego_d identifies `CRangeCompressor::CalcCompressionScript`. The submitted extent is 605 bytes.

- egor donor procedure at 00bdcde0
- fablewin donor procedure at 03319190

- call at 0x00F3AC36 (the caller is not landed)
- call at 0x00F3AC7D (the caller is not landed)
- call at 0x00F3AC92 (the caller is not landed)
- call at 0x00F3AD61 (the caller is not landed)
- call at 0x00F3AD7B (the caller is not landed)
- call at 0x00F3AEC7 in landed 0x00F3AE50 CRangeCompressor::Compress

The EDI-to-EBX copy straddles the block-width guard at 00F3AC0C in the opposite order. The selected function otherwise has the full retail extent.

Checks zero counts, widths, strided values, recursion into subblocks, and equal/unequal encoding costs.
