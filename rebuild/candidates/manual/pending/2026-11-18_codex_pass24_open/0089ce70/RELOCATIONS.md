# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x64 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0089CED3 | 0x0040F020 | reloc_bindings |
| +0x7D | `?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ` | 0x0089CEEC | 0x007446A0 | reloc_bindings |
| +0x8C | `?IsAwareOfThing@CTCPerceiveThing@@QBE_NXZ` | 0x0089CEFB | 0x00643B3B | reloc_bindings |
| +0x9D | `?GetSoundRadius@CTCPerceiveThing@@QBEMXZ` | 0x0089CF0C | 0x00744440 | reloc_bindings |
| +0xA8 | `?GetSmellRadius@CTCPerceiveThing@@QBEMXZ` | 0x0089CF17 | 0x00744450 | NAME_FIXES_QUEUE |
| +0xCC | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0089CF3B | 0x004C73D0 | reloc_bindings |
| +0xD5 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0089CF44 | 0x004C73D0 | reloc_bindings |
| +0x11F | `?GetExtendedSightRadius@CTCPerceiveThing@@QBEMXZ` | 0x0089CF8E | 0x00744950 | reloc_bindings |
| +0x12A | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0089CF99 | 0x004C73D0 | reloc_bindings |
| +0x133 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0089CFA2 | 0x004C73D0 | reloc_bindings |
| +0x1A1 | `?GFastOneOverSqrtTable@@3PAKA` | 0x0089D00E | 0x013CE558 | reloc_bindings |
| +0x1CF | `__real@40400000` | 0x0089D03D | 0x0122DED4 | IEEE literal payload |
| +0x1D9 | `__real@3f000000` | 0x0089D047 | 0x0122F59C | IEEE literal payload |
| +0x202 | `?GetFOV@CTCPerceiveThing@@QBEMXZ` | 0x0089D071 | 0x007445D0 | reloc_bindings |
| +0x208 | `__real@401921fb60000000` | 0x0089D076 | 0x0124F2B8 | IEEE literal payload |

15 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
