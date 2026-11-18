# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x38 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0091B5A7 | 0x0040F020 | reloc_bindings |
| +0x4F | `__real@00000000` | 0x0091B5BD | 0x0122DEDC | IEEE literal payload |
| +0x66 | `?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ` | 0x0091B5D5 | 0x00744430 | reloc_bindings |
| +0x7D | `_malloc` | 0x0091B5E4 | 0x00BFEA0E | reloc_bindings |
| +0x94 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0091B5FD | 0x004C73D0 | reloc_bindings |
| +0x9F | `__real@41000000` | 0x0091B616 | 0x0125916C | IEEE literal payload (nearby instruction) |
| +0xA4 | `__ftol2` | 0x0091B60F | 0x00BFEA70 | reloc_bindings |
| +0xAC | `__real@41000000` | 0x0091B616 | 0x0125916C | IEEE literal payload |
| +0xB3 | `__ftol2` | 0x0091B61E | 0x00BFEA70 | reloc_bindings |
| +0xBC | `__real@41000000` | 0x0091B623 | 0x0125916C | IEEE literal payload |
| +0xC5 | `__ftol2` | 0x0091B62D | 0x00BFEA70 | reloc_bindings |
| +0xCD | `__real@41000000` | 0x0091B632 | 0x0125916C | IEEE literal payload |
| +0xD6 | `__ftol2` | 0x0091B63C | 0x00BFEA70 | reloc_bindings |
| +0xFF | `?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ` | 0x0091B665 | 0x007446A0 | reloc_bindings |
| +0x129 | `?GetThingManager@CAIStateGroupBase@@IBEAAVCThingManager@@XZ` | 0x0091B68F | 0x008FCF60 | Binding remains unconfirmed |
| +0x130 | `?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ` | 0x0091B696 | 0x0051E490 | reloc_bindings |
| +0x137 | `??$GetAllTCsInRoughArea@VCTCPerceiveThing@@V?$CFilter_Bind2@VCThingFilter_IsPerceivingThing@@VCThingFilter_IsNot@@@@V?$list@PAVCTCPerceiveThing@@V?$allocator@PAVCTCPerceiveThing@@@std@@@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsPerceivingThing@@VCThingFilter_IsNot@@@@AAV?$list@PAVCTCPerceiveThing@@V?$allocator@PAVCTCPerceiveThing@@@std@@@std@@@Z` | 0x0091B69D | 0x0091BA40 | Binding remains unconfirmed |
| +0x13C | `?SetAsIndirectlyAwareOfPerceivedThing@CTCPerceiveThing@@QAEXXZ` | 0x0091B6A2 | 0x007449E0 | reloc_bindings |
| +0x159 | `??$for_each@U?$_List_iterator@PAVCTCPerceiveThing@@U?$_Nonconst_traits@PAVCTCPerceiveThing@@@std@@@std@@V?$mem_fun_t@XVCTCPerceiveThing@@@2@@std@@YI?AV?$mem_fun_t@XVCTCPerceiveThing@@@0@U?$_List_iterator@PAVCTCPerceiveThing@@U?$_Nonconst_traits@PAVCTCPerceiveThing@@@std@@@0@0V10@@Z` | 0x0091B6B7 | 0x0091B890 | Binding remains unconfirmed |
| +0x162 | `??1?$list@PAVCTCPerceiveThing@@V?$allocator@PAVCTCPerceiveThing@@@std@@@std@@QAE@XZ` | 0x0091B6C0 | 0x0091B910 | Binding remains unconfirmed |
| +0x169 | `??_C@_05BOHNMGBD@ALARM?$AA@` | 0x0091B6C7 | 0x012737BC | literal payload prefix |
| +0x172 | `??0CCharString@@QAE@PBDJ@Z` | 0x0091B6D0 | 0x0099EBF0 | reloc_bindings |
| +0x185 | `?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z` | 0x0091B6E3 | 0x00662A00 | reloc_bindings |
| +0x190 | `??1CCharString@@QAE@XZ` | 0x0091B6EE | 0x0099EAE0 | reloc_bindings |
| +0x1B4 | `??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@ABVCSoundPair@@_N33JJ@Z` | 0x0091B712 | 0x00842570 | reloc_bindings |
| +0x1BC | `?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ` | 0x0091B71A | 0x007446A0 | reloc_bindings |
| +0x1CA | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x0091B728 | 0x00A01B10 | reloc_bindings |
| +0x1D3 | `??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x1D8 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x0091B736 | 0x00A01B90 | reloc_bindings |
| +0x1E4 | `?AIAction_PerformActionTargetFaceAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@V?$CIntelligentPointer@$$CBVCThing@@@@_N@Z` | 0x0091B742 | 0x0097DCB0 | reloc_bindings |
| +0x1EB | `?RunToPerceivedThingPosition@CAIStateGroup_SeekPerceivedThing@@AAEXXZ` | 0x0091B749 | 0x0091B3A0 | Binding remains unconfirmed |
| +0x1F4 | `??1CCreatureActionBase@@UAE@XZ` | 0x0091B752 | 0x00693EF0 | reloc_bindings |

26 supported pairs out of 32. See the separate manual audit where supplied.
