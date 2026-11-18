# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x37 | `??$__find@PAPAVCThingGameObject@@PAV1@@std@@YIPAPAVCThingGameObject@@PAPAV1@0ABQAV1@ABUrandom_access_iterator_tag@0@@Z` | 0x007C8226 | 0x007C8130 | FableWin debug _Find and native random-access iterator stride and tag |
| +0xBF | `?RotateXY@C3DVector@@QAEXM@Z` | 0x007C82AE | 0x00A14040 | reloc_bindings |
| +0xC7 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007C82B6 | 0x004C73D0 | reloc_bindings |
| +0xD0 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007C82BF | 0x004C73D0 | reloc_bindings |
| +0x11B | `?GFastOneOverSqrtTable@@3PAKA` | 0x007C8308 | 0x013CE558 | reloc_bindings |
| +0x157 | `__real@40400000` | 0x007C833D | 0x0122DED4 | IEEE literal payload |
| +0x161 | `__real@3f000000` | 0x007C8347 | 0x0122F59C | IEEE literal payload |
| +0x199 | `__real@00000000` | 0x007C837F | 0x0122DEDC | IEEE literal payload |
| +0x1B2 | `?RotateXY@C3DVector@@QAEXM@Z` | 0x007C83A1 | 0x00A14040 | reloc_bindings |
| +0x1B9 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007C83A8 | 0x004C73D0 | reloc_bindings |
| +0x214 | `??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1ABVC3DVector@@2_N333@Z` | 0x007C8403 | 0x00789238 | reloc_bindings |
| +0x223 | `?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z` | 0x007C8412 | 0x00868CA0 | reloc_bindings |
| +0x237 | `?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z` | 0x007C8426 | 0x00868CE0 | reloc_bindings |
| +0x275 | `?push_back@?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@QAEXABQAVCThingGameObject@@@Z` | 0x007C8464 | 0x0078915C | FableWin and native STLport vector growth and append paths |
| +0x28B | `??_C@_0BA@JOOOCGM@CTCSolidToThing?$AA@` | 0x007C847A | 0x0123B47C | terminated literal payload |
| +0x294 | `??0CCharString@@QAE@PBDJ@Z` | 0x007C8483 | 0x0099EBF0 | reloc_bindings |
| +0x2A5 | `?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z` | 0x007C8494 | 0x004C9D60 | reloc_bindings |
| +0x2AE | `??1CCharString@@QAE@XZ` | 0x007C849D | 0x0099EAE0 | reloc_bindings |
| +0x2CD | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x007C84BC | 0x0040F020 | reloc_bindings |
| +0x2E6 | `?AddThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z` | 0x007C84D5 | 0x00770FA0 | reloc_bindings |
| +0x2EF | `??1CHitParameters@@QAE@XZ` | 0x007C84DE | 0x00789358 | reloc_bindings |

21 supported pairs out of 21. See the separate manual audit where supplied.
