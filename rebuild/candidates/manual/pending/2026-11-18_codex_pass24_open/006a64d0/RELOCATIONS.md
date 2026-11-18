# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x2E | `?DrawPreparePrimitivesForRendering@CThing@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z` | 0x006A64FD | 0x004C7540 | Landed CThing method and both PDB declarations. Native direct base call at 0x006A64FD. |
| +0x5C | `?GetWorld@CThing@@IAEAAVCWorld@@XZ` | 0x006A652B | 0x004C7AA0 | reloc_bindings |
| +0x6A | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x006A6539 | 0x004C73D0 | reloc_bindings |
| +0x81 | `?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z` | 0x006A6550 | 0x006614F0 | reloc_bindings |
| +0x92 | `??0CThingDrawInformation@@QAE@XZ` | 0x006A6561 | 0x006A4B30 | FableWin 0x01F21AB0 and landed constructor. Native call at 0x006A6561. |
| +0x9F | `?GetStaticFocalPosIfLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DVector@@@Z` | 0x006A656E | 0x004C11F0 | reloc_bindings |
| +0xD0 | `??0CCharString@@QAE@ABV0@@Z` | 0x006A659F | 0x0099EC30 | reloc_bindings |
| +0xEB | `?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ` | 0x006A65BA | 0x004C7A70 | reloc_bindings |
| +0x118 | `?ToWideString@CCharString@@QBE?AVCWideString@@XZ` | 0x006A65E7 | 0x0099E480 | reloc_bindings |
| +0x12E | `??0CEnginePrimitiveText@@QAE@_KABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@_NW4EEngineInterpolationSet@@@Z` | 0x006A65FD | 0x004C3120 | reloc_bindings |
| +0x142 | `??1CWideString@@QAE@XZ` | 0x006A6611 | 0x0099B510 | reloc_bindings |
| +0x14B | `??1CWideString@@QAE@XZ` | 0x006A661A | 0x0099B510 | reloc_bindings |
| +0x152 | `?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ` | 0x006A6621 | 0x004C7A70 | reloc_bindings |
| +0x165 | `__real@3e4ccccd` | 0x006A6633 | 0x012305A0 | IEEE literal payload |
| +0x172 | `??1CCharString@@QAE@XZ` | 0x006A6641 | 0x0099EAE0 | reloc_bindings |
| +0x17B | `??1CCharString@@QAE@XZ` | 0x006A664A | 0x0099EAE0 | reloc_bindings |
| +0x19B | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x006A666A | 0x0040F020 | reloc_bindings |
| +0x1B4 | `?DrawDebugInfo@CTCCreatureGenerator@@QAEXABVCEnginePrimitivePrepareInfo@@@Z` | 0x006A6683 | 0x0074BC20 | NAME_FIXES_QUEUE |
| +0x1B9 | `?g_DrawThingDebugState@@3_NA` | 0x006A6688 | 0x013B83C5 | Native byte selector at 0x006A6688 before physics debug rendering. Source spelling is descriptive. |
| +0x1EA | `?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z` | 0x006A66B9 | 0x004C1060 | reloc_bindings |
| +0x1F4 | `__real@3fc00000` | 0x006A66C2 | 0x0125AADC | IEEE literal payload |
| +0x201 | `??0CCharString@@QAE@XZ` | 0x006A66D0 | 0x0099E4B0 | reloc_bindings |
| +0x20C | `??_C@_02ONHIBMNM@M?5?$AA@` | 0x006A66DB | 0x0125D104 | literal payload prefix |
| +0x213 | `??_C@_00CNPNBAHC@?$AA@` | 0x006A66E2 | 0x0122D70E | literal payload prefix |
| +0x21D | `??YCCharString@@QAEABV0@PBD@Z` | 0x006A66EC | 0x0099F100 | reloc_bindings |
| +0x228 | `??_C@_01JEJKBAGA@g?$AA@` | 0x006A66F7 | 0x0125D100 | literal payload prefix |
| +0x22F | `??_C@_00CNPNBAHC@?$AA@` | 0x006A66FE | 0x0122D70E | literal payload prefix |
| +0x239 | `??YCCharString@@QAEABV0@PBD@Z` | 0x006A6708 | 0x0099F100 | reloc_bindings |
| +0x244 | `??_C@_01GOHFPIOK@m?$AA@` | 0x006A6713 | 0x0125D0FC | literal payload prefix |
| +0x24B | `??_C@_00CNPNBAHC@?$AA@` | 0x006A671A | 0x0122D70E | literal payload prefix |
| +0x255 | `??YCCharString@@QAEABV0@PBD@Z` | 0x006A6724 | 0x0099F100 | reloc_bindings |
| +0x260 | `??_C@_01PAPGNFGE@c?$AA@` | 0x006A672F | 0x0125D0F8 | literal payload prefix |
| +0x267 | `??_C@_00CNPNBAHC@?$AA@` | 0x006A6736 | 0x0122D70E | literal payload prefix |
| +0x271 | `??YCCharString@@QAEABV0@PBD@Z` | 0x006A6740 | 0x0099F100 | reloc_bindings |
| +0x27C | `??_C@_01KBJDNOO@i?$AA@` | 0x006A674B | 0x0125D0F4 | literal payload prefix |
| +0x283 | `??_C@_00CNPNBAHC@?$AA@` | 0x006A6752 | 0x0122D70E | literal payload prefix |
| +0x28D | `??YCCharString@@QAEABV0@PBD@Z` | 0x006A675C | 0x0099F100 | reloc_bindings |
| +0x298 | `??_C@_01KDCPPGHE@r?$AA@` | 0x006A6767 | 0x0125D0F0 | literal payload prefix |
| +0x29F | `??_C@_00CNPNBAHC@?$AA@` | 0x006A676E | 0x0122D70E | literal payload prefix |
| +0x2A9 | `??YCCharString@@QAEABV0@PBD@Z` | 0x006A6778 | 0x0099F100 | reloc_bindings |
| +0x2C4 | `?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ` | 0x006A6793 | 0x004C7A70 | reloc_bindings |
| +0x2ED | `?ToWideString@CCharString@@QBE?AVCWideString@@XZ` | 0x006A67BC | 0x0099E480 | reloc_bindings |
| +0x303 | `??0CEnginePrimitiveText@@QAE@_KABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@_NW4EEngineInterpolationSet@@@Z` | 0x006A67D2 | 0x004C3120 | reloc_bindings |
| +0x317 | `??1CWideString@@QAE@XZ` | 0x006A67E6 | 0x0099B510 | reloc_bindings |
| +0x320 | `??1CWideString@@QAE@XZ` | 0x006A67EF | 0x0099B510 | reloc_bindings |
| +0x327 | `?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ` | 0x006A67F6 | 0x004C7A70 | reloc_bindings |
| +0x33A | `__real@3e4ccccd` | 0x006A6808 | 0x012305A0 | IEEE literal payload |
| +0x347 | `??1CCharString@@QAE@XZ` | 0x006A6816 | 0x0099EAE0 | reloc_bindings |

48 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
