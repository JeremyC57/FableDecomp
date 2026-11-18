# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x1A | `__real@3b360b61` | 0x00699688 | 0x01238E00 | IEEE literal payload |
| +0x3D | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006996AC | 0x00A01B50 | reloc_bindings |
| +0x44 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x006996B3 | 0x004C73D0 | reloc_bindings |
| +0x57 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006996C6 | 0x00A01B50 | reloc_bindings |
| +0x98 | `?_M_insert_overflow@?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@IAEXPAPBVCThing@@ABQBV3@ABU__true_type@2@I_N@Z` | 0x00699707 | 0x0049C370 | reloc_bindings |
| +0xFF | `?_M_insert_overflow@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@IAEXPAVC3DVector@@ABV3@ABU__false_type@2@I_N@Z` | 0x0069976E | 0x0066E370 | reloc_bindings |
| +0x14E | `?_M_insert_overflow@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@IAEXPAVC3DVector@@ABV3@ABU__false_type@2@I_N@Z` | 0x006997BF | 0x0066E370 | reloc_bindings |
| +0x168 | `__real@401921fb60000000` | 0x006997D8 | 0x0124F2B8 | IEEE literal payload |
| +0x17B | `?GetGroundFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z` | 0x006997EC | 0x0063AB10 | reloc_bindings |
| +0x189 | `__real@00000000` | 0x006997F9 | 0x0122DEDC | IEEE literal payload |
| +0x1A4 | `?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z` | 0x00699815 | 0x0063A300 | reloc_bindings |
| +0x1B0 | `?GetThemeMinCameraSizeZOffsetAt@CWorldMap@@QBEMABVC3DVector@@@Z` | 0x00699821 | 0x0063A540 | reloc_bindings |
| +0x1C0 | `?GetWaterSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z` | 0x00699831 | 0x0063A420 | reloc_bindings |
| +0x21B | `?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z` | 0x0069988C | 0x00877840 | reloc_bindings |
| +0x2D4 | `_free` | 0x00699945 | 0x00BFEA14 | reloc_bindings |
| +0x2E9 | `_free` | 0x0069995A | 0x00BFEA14 | reloc_bindings |

16 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
