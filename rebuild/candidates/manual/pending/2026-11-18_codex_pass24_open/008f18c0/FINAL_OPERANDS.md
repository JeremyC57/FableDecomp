# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x10 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x008F18CF | 0x004C73D0 | reloc_bindings |
| +0xBC | `?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z` | 0x008F197B | 0x008F0B20 | reloc_bindings (nearby instruction) |
| +0xD1 | `__real@3f000000` | 0x008F198F | 0x0122F59C | IEEE literal payload (nearby instruction) |
| +0xDE | `__real@3f000000` | 0x008F19A0 | 0x0122F59C | IEEE literal payload (nearby instruction) |
| +0x107 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008F19C9 | 0x0040F020 | reloc_bindings (nearby instruction) |
| +0x126 | `?ClearMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x15C | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x008F1A24 | 0x004C73D0 | reloc_bindings (nearby instruction) |
| +0x278 | `??_C@_0BF@ONLBEBM@SOUND_CARD_HIGHLIGHT?$AA@` | 0x008F1B3A | 0x0125603C | Binding remains unconfirmed |
| +0x281 | `??0CCharString@@QAE@PBDJ@Z` | 0x008F1B43 | 0x0099EBF0 | reloc_bindings |
| +0x28D | `?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z` | 0x008F1B4F | 0x005EA950 | reloc_bindings |
| +0x296 | `??1CCharString@@QAE@XZ` | 0x008F1B58 | 0x0099EAE0 | reloc_bindings |
| +0x2C2 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008F1B80 | 0x0040F020 | reloc_bindings |
| +0x2F1 | `?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z` | 0x008F1BA9 | 0x004C04C0 | reloc_bindings |

11 supported pairs out of 13. See the separate manual audit where supplied.
