# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x35 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009313C4 | 0x00A01B50 | reloc_bindings |
| +0x44 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009313D3 | 0x00A01B50 | reloc_bindings |
| +0x58 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009313E7 | 0x00A01B50 | reloc_bindings |
| +0x5F | `?IsLocked@CThingBuilding@@QBE_NXZ` | 0x009313EE | 0x0082DE20 | reloc_bindings |
| +0x7C | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093140E | 0x00A01B50 | reloc_bindings |
| +0xA5 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931437 | 0x004365B0 | reloc_bindings |
| +0xDA | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093146B | 0x00A01B50 | reloc_bindings |
| +0xE5 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931476 | 0x00A01B50 | reloc_bindings |
| +0xF5 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931486 | 0x00A01B50 | reloc_bindings |
| +0x10A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093149B | 0x00A01B50 | reloc_bindings |
| +0x13C | `?_M_insert_overflow@?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@IAEXPAPBVCThing@@ABQBV3@ABU__true_type@2@I_N@Z` | 0x009314CD | 0x0049C370 | reloc_bindings |
| +0x170 | `?OrderByHeight2@@YI_NPBVCThing@@0@Z` | 0x00931501 | 0x00930D90 | NAME_FIXES_QUEUE |
| +0x179 | `??$sort@PAPBVCThing@@P6I_NPBV1@0@Z@std@@YIXPAPBVCThing@@0P6I_NPBV1@1@Z@Z` | 0x0093150A | 0x009328D0 | Binding remains unconfirmed |
| +0x184 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x00931515 | 0x00A01B10 | reloc_bindings |
| +0x191 | `??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@` | 0x0093151F | 0x01237F1C | reloc_bindings |
| +0x196 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00931527 | 0x00A01B90 | reloc_bindings |
| +0x1A1 | `?GetFrame@CWorld@@SIJXZ` | 0x00931530 | 0x0049D870 | reloc_bindings |
| +0x1DA | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931569 | 0x0040F020 | reloc_bindings |
| +0x1F2 | `?IsTalking@CTCTalk@@QBE_NXZ` | 0x00931581 | 0x006E28A0 | reloc_bindings |
| +0x203 | `??_C@_0CD@OPBMFBIF@TEXT_AI_OPINION_ACT_SCREAM_TO_SE@` | 0x00931592 | 0x01291E70 | literal payload prefix |
| +0x20C | `??0CCharString@@QAE@PBDJ@Z` | 0x0093159B | 0x0099EBF0 | reloc_bindings |
| +0x220 | `?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z` | 0x009315AF | 0x006E3B30 | reloc_bindings |
| +0x22B | `??1CCharString@@QAE@XZ` | 0x009315BA | 0x0099EAE0 | reloc_bindings |
| +0x237 | `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ` | 0x009315C6 | 0x0097E200 | reloc_bindings |
| +0x243 | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x009315D2 | 0x008FCF40 | reloc_bindings |
| +0x24C | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x009315DB | 0x00A01C10 | reloc_bindings |
| +0x25E | `_free` | 0x009315ED | 0x00BFEA14 | reloc_bindings |
| +0x26F | `?ConstantFPS@CMainGameComponent@@1JA` | 0x009315FD | 0x01375550 | reloc_bindings |
| +0x27A | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x00931609 | 0x008FCFD0 | reloc_bindings |
| +0x286 | `?GFFloatRandom@@YIMMPAK@Z` | 0x00931615 | 0x004978A0 | reloc_bindings |
| +0x28C | `__real@40a00000` | 0x0093161A | 0x01291DF4 | IEEE literal payload |
| +0x299 | `?GetFrame@CWorld@@SIJXZ` | 0x00931628 | 0x0049D870 | reloc_bindings |
| +0x2AA | `__ftol2` | 0x00931639 | 0x00BFEA70 | reloc_bindings |
| +0x2B6 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931645 | 0x00A01B50 | reloc_bindings |
| +0x2C3 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931652 | 0x00A01B50 | reloc_bindings |
| +0x2D5 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931664 | 0x00A01B50 | reloc_bindings |
| +0x2F9 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x00931688 | 0x00A01B10 | reloc_bindings |
| +0x302 | `??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@` | 0x00931690 | 0x01237F1C | reloc_bindings |
| +0x307 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00931696 | 0x00A01B90 | reloc_bindings |
| +0x310 | `?AIAction_MoveToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@MAA_NW4EMovementType@@_N444M@Z` | 0x0093169F | 0x0097E380 | reloc_bindings |
| +0x32B | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x009316BA | 0x008FCF40 | reloc_bindings |
| +0x347 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009316D6 | 0x00A01B50 | reloc_bindings |
| +0x361 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009316F0 | 0x00A01B50 | reloc_bindings |
| +0x368 | `?LockAllDoors@CThingBuilding@@QAEX_N0@Z` | 0x009316F7 | 0x0082DB00 | reloc_bindings |
| +0x371 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x00931700 | 0x00A01C10 | reloc_bindings |
| +0x37F | `_free` | 0x00931712 | 0x00BFEA14 | reloc_bindings |
| +0x397 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093172A | 0x00A01B50 | reloc_bindings |
| +0x3A6 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931739 | 0x00A01B50 | reloc_bindings |
| +0x3BA | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093174D | 0x00A01B50 | reloc_bindings |
| +0x3C1 | `?IsLocked@CThingBuilding@@QBE_NXZ` | 0x00931754 | 0x0082DE20 | reloc_bindings |
| +0x3D0 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931763 | 0x00A01B50 | reloc_bindings |
| +0x3F6 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931789 | 0x004365B0 | reloc_bindings |
| +0x437 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x009317CA | 0x004365B0 | reloc_bindings |
| +0x459 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009317EC | 0x00A01B50 | reloc_bindings |
| +0x468 | `__real@3f000000` | 0x009317FA | 0x0122F59C | IEEE literal payload |
| +0x480 | `?GetPhysPos@CThing@@QBE?AVC3DVector@@XZ` | 0x00931813 | 0x004C7950 | reloc_bindings |
| +0x488 | `?GetNearbyWanderPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@_N@Z` | 0x0093181B | 0x006CB6E0 | reloc_bindings |
| +0x49C | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093182F | 0x00A01B50 | reloc_bindings |
| +0x4A3 | `?IsPointInside@CThingBuilding@@QBE_NABVC3DVector@@@Z` | 0x00931836 | 0x0082D000 | reloc_bindings |
| +0x4B7 | `?IsPosNavigable@CTCCreatureNavigation@@QBE_NABVC3DVector@@@Z` | 0x0093184A | 0x006CBA50 | reloc_bindings |
| +0x4C4 | `?GetFrame@CWorld@@SIJXZ` | 0x00931857 | 0x0049D870 | reloc_bindings |
| +0x4FD | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931890 | 0x0040F020 | reloc_bindings |
| +0x515 | `?IsTalking@CTCTalk@@QBE_NXZ` | 0x009318A8 | 0x006E28A0 | reloc_bindings |
| +0x523 | `??_C@_0CD@OPBMFBIF@TEXT_AI_OPINION_ACT_SCREAM_TO_SE@` | 0x009318B3 | 0x01291E70 | literal payload prefix |
| +0x52C | `??0CCharString@@QAE@PBDJ@Z` | 0x009318BC | 0x0099EBF0 | reloc_bindings |
| +0x540 | `?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z` | 0x009318D0 | 0x006E3B30 | reloc_bindings |
| +0x54B | `??1CCharString@@QAE@XZ` | 0x009318DB | 0x0099EAE0 | reloc_bindings |
| +0x557 | `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ` | 0x009318E7 | 0x0097E200 | reloc_bindings |
| +0x563 | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x009318F3 | 0x008FCF40 | reloc_bindings |
| +0x571 | `?ConstantFPS@CMainGameComponent@@1JA` | 0x00931900 | 0x01375550 | reloc_bindings |
| +0x57C | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x0093190C | 0x008FCFD0 | reloc_bindings |
| +0x588 | `?GFFloatRandom@@YIMMPAK@Z` | 0x00931918 | 0x004978A0 | reloc_bindings |
| +0x58E | `__real@40a00000` | 0x0093191D | 0x01291DF4 | IEEE literal payload |
| +0x59B | `?GetFrame@CWorld@@SIJXZ` | 0x0093192B | 0x0049D870 | reloc_bindings |
| +0x5AC | `__ftol2` | 0x0093193C | 0x00BFEA70 | reloc_bindings |
| +0x5EF | `?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z` | 0x0093197F | 0x0097E890 | reloc_bindings |
| +0x637 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009319C7 | 0x00A01B50 | reloc_bindings |
| +0x646 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009319D6 | 0x00A01B50 | reloc_bindings |
| +0x65A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009319EA | 0x00A01B50 | reloc_bindings |
| +0x661 | `?IsLockable@CThingBuilding@@QBE_NXZ` | 0x009319F1 | 0x0082DD90 | reloc_bindings |
| +0x670 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A00 | 0x00A01B50 | reloc_bindings |
| +0x677 | `?IsLocked@CThingBuilding@@QBE_NXZ` | 0x00931A07 | 0x0082DE20 | reloc_bindings |
| +0x687 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A17 | 0x00A01B50 | reloc_bindings |
| +0x6AD | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931A3D | 0x0040F020 | reloc_bindings |
| +0x6C6 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A56 | 0x00A01B50 | reloc_bindings |
| +0x6CF | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A5F | 0x00A01B50 | reloc_bindings |
| +0x6DA | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A6A | 0x00A01B50 | reloc_bindings |
| +0x6E1 | `?IsLocked@CThingBuilding@@QBE_NXZ` | 0x00931A71 | 0x0082DE20 | reloc_bindings |
| +0x6EC | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A7C | 0x00A01B50 | reloc_bindings |
| +0x70A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931A9A | 0x00A01B50 | reloc_bindings |
| +0x711 | `?LockAllDoors@CThingBuilding@@QAEX_N0@Z` | 0x00931AA1 | 0x0082DB00 | reloc_bindings |
| +0x71A | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x00931AAA | 0x00A01B10 | reloc_bindings |
| +0x728 | `??_7?$CIntelligentPointer@VCThing@@@@6B@` | 0x00931AB5 | 0x01238C6C | reloc_bindings |
| +0x72D | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00931ABD | 0x00A01B90 | reloc_bindings |
| +0x734 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931AC4 | 0x00A01B50 | reloc_bindings |
| +0x75D | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931AED | 0x0040F020 | reloc_bindings |
| +0x777 | `??_C@_0BM@CHOMFIAI@GAZE_OUT_OF_BUILDING_MARKER?$AA@` | 0x00931B07 | 0x01266DBC | terminated literal payload |
| +0x799 | `??0CCharString@@QAE@PBDJ@Z` | 0x00931B29 | 0x0099EBF0 | reloc_bindings |
| +0x7A3 | `?Get@CGameDefinitionManager@@SIAAV1@XZ` | 0x00931B33 | 0x0044C6B0 | reloc_bindings |
| +0x7AA | `?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z` | 0x00931B3A | 0x009AD410 | reloc_bindings |
| +0x7B7 | `??1CCharString@@QAE@XZ` | 0x00931B47 | 0x0099EAE0 | reloc_bindings |
| +0x7D6 | `??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z` | 0x00931B65 | 0x004CC340 | reloc_bindings |
| +0x7E1 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931B70 | 0x00A01B50 | reloc_bindings |
| +0x7F6 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931B85 | 0x00A01B50 | reloc_bindings |
| +0x801 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931B90 | 0x00A01B50 | reloc_bindings |
| +0x808 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00931B97 | 0x004C73D0 | reloc_bindings |
| +0x812 | `?IsPointInside@CThingBuilding@@QBE_NABVC3DVector@@@Z` | 0x00931BA1 | 0x0082D000 | reloc_bindings |
| +0x81D | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931BAC | 0x00A01B50 | reloc_bindings |
| +0x82A | `??RCThingFilter_IsNotBeingExclusivelyUsed@@QBE_NABVCThing@@@Z` | 0x00931BB9 | 0x006A8AF0 | reloc_bindings |
| +0x835 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931BC4 | 0x00A01B50 | reloc_bindings |
| +0x84A | `?push_back@?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@QAEXABQAVCThing@@@Z` | 0x00931BD9 | 0x00436C80 | reloc_bindings |
| +0x879 | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x00931C08 | 0x008FCFD0 | reloc_bindings |
| +0x882 | `?GFRandom@@YIKKPAK@Z` | 0x00931C11 | 0x004CDAE0 | reloc_bindings |
| +0x892 | `??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z` | 0x00931C21 | 0x004A96F0 | reloc_bindings |
| +0x89E | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931C2D | 0x00A01B50 | reloc_bindings |
| +0x8A8 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00931C37 | 0x00A01B90 | reloc_bindings |
| +0x8B4 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x00931C43 | 0x00A01C10 | reloc_bindings |
| +0x8BE | `_free` | 0x00931C4D | 0x00BFEA14 | reloc_bindings |
| +0x8CA | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931C59 | 0x00A01B50 | reloc_bindings |
| +0x8D7 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931C6A | 0x00A01B50 | reloc_bindings |
| +0x8F5 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931C87 | 0x00A01B50 | reloc_bindings |
| +0x90D | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00931C9F | 0x004C73D0 | reloc_bindings |
| +0x92B | `?AIAction_MoveToPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@AA_NW4EMovementType@@_N4PBVCThing@@4M@Z` | 0x00931CBD | 0x0097EC00 | reloc_bindings |
| +0x95C | `?ConstantFPS@CMainGameComponent@@1JA` | 0x00931CED | 0x01375550 | reloc_bindings |
| +0x967 | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x00931CF9 | 0x008FCFD0 | reloc_bindings |
| +0x973 | `?GFFloatRandom@@YIMMPAK@Z` | 0x00931D05 | 0x004978A0 | reloc_bindings |
| +0x979 | `__real@40000000` | 0x00931D0A | 0x01291E04 | IEEE literal payload |
| +0x986 | `?GetFrame@CWorld@@SIJXZ` | 0x00931D18 | 0x0049D870 | reloc_bindings |
| +0x997 | `__ftol2` | 0x00931D29 | 0x00BFEA70 | reloc_bindings |
| +0x9AF | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x00931D3E | 0x00A01B10 | reloc_bindings |
| +0x9B8 | `??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@` | 0x00931D46 | 0x01237F1C | reloc_bindings |
| +0x9BD | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931D4C | 0x00A01B50 | reloc_bindings |
| +0x9C5 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00931D54 | 0x00A01B90 | reloc_bindings |
| +0x9CE | `?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z` | 0x00931D5D | 0x0097D6F0 | reloc_bindings |
| +0x9DF | `?GetFrame@CWorld@@SIJXZ` | 0x00931D6E | 0x0049D870 | reloc_bindings |
| +0x9F4 | `?GetFrame@CWorld@@SIJXZ` | 0x00931D83 | 0x0049D870 | reloc_bindings |
| +0xA0C | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x00931D9B | 0x00A01C10 | reloc_bindings |
| +0xA1B | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931DAA | 0x00A01B50 | reloc_bindings |
| +0xA2A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931DB9 | 0x00A01B50 | reloc_bindings |
| +0xA46 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931DD5 | 0x00A01B50 | reloc_bindings |
| +0xA4F | `?IsAwareOfThingIndirectly@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z` | 0x00931DDE | 0x00864700 | reloc_bindings |
| +0xA83 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931E12 | 0x004365B0 | reloc_bindings |
| +0xAA5 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931E34 | 0x00A01B50 | reloc_bindings |
| +0xAAC | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00931E3B | 0x004C73D0 | reloc_bindings |
| +0xAB5 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00931E44 | 0x004C73D0 | reloc_bindings |
| +0xAFC | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00931E8B | 0x004C73D0 | reloc_bindings |
| +0xB09 | `?GetNearbyDirectedPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@M0MAAV2@@Z` | 0x00931E98 | 0x006CB780 | reloc_bindings |
| +0xB1C | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00931EAB | 0x004C73D0 | reloc_bindings |
| +0xB4E | `__real@3f000000` | 0x00931EDC | 0x0122F59C | IEEE literal payload |
| +0xB71 | `??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingBuilding@@@@@@YI_NABV?$CIntelligentPointer@VCThingBuilding@@@@@Z` | 0x00931F00 | 0x0065F8CA | reloc_bindings |
| +0xB81 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00931F10 | 0x00A01B50 | reloc_bindings |
| +0xB88 | `?IsPointInside@CThingBuilding@@QBE_NABVC3DVector@@@Z` | 0x00931F17 | 0x0082D000 | reloc_bindings |
| +0xB95 | `?GetFrame@CWorld@@SIJXZ` | 0x00931F24 | 0x0049D870 | reloc_bindings |
| +0xBCE | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00931F5D | 0x0040F020 | reloc_bindings |
| +0xBE6 | `?IsTalking@CTCTalk@@QBE_NXZ` | 0x00931F75 | 0x006E28A0 | reloc_bindings |
| +0xBF3 | `??_C@_0CD@OPBMFBIF@TEXT_AI_OPINION_ACT_SCREAM_TO_SE@` | 0x00931F82 | 0x01291E70 | literal payload prefix |
| +0xBFC | `??0CCharString@@QAE@PBDJ@Z` | 0x00931F8B | 0x0099EBF0 | reloc_bindings |
| +0xC10 | `?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z` | 0x00931F9F | 0x006E3B30 | reloc_bindings |
| +0xC1B | `??1CCharString@@QAE@XZ` | 0x00931FAA | 0x0099EAE0 | reloc_bindings |
| +0xC27 | `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ` | 0x00931FB6 | 0x0097E200 | reloc_bindings |
| +0xC33 | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x00931FC2 | 0x008FCF40 | reloc_bindings |
| +0xC41 | `?ConstantFPS@CMainGameComponent@@1JA` | 0x00931FCF | 0x01375550 | reloc_bindings |
| +0xC4C | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x00931FDB | 0x008FCFD0 | reloc_bindings |
| +0xC58 | `?GFFloatRandom@@YIMMPAK@Z` | 0x00931FE7 | 0x004978A0 | reloc_bindings |
| +0xC5E | `__real@40a00000` | 0x00931FEC | 0x01291DF4 | IEEE literal payload |
| +0xC6B | `?GetFrame@CWorld@@SIJXZ` | 0x00931FFA | 0x0049D870 | reloc_bindings |
| +0xC7C | `__ftol2` | 0x0093200B | 0x00BFEA70 | reloc_bindings |
| +0xCB8 | `?AIAction_MoveToPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@AA_NW4EMovementType@@_N4PBVCThing@@4M@Z` | 0x00932047 | 0x0097EC00 | reloc_bindings |
| +0xCC2 | `?GetFrame@CWorld@@SIJXZ` | 0x00932051 | 0x0049D870 | reloc_bindings |
| +0xD22 | `?IsTalking@CTCTalk@@QBE_NXZ` | 0x009320B1 | 0x006E28A0 | reloc_bindings |
| +0xD33 | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x009320C2 | 0x008FCFD0 | reloc_bindings |
| +0xD4A | `?GFROR13@@YIJK@Z` | 0x009320D9 | 0x00497890 | reloc_bindings |
| +0xD60 | `??_C@_0CC@CCDAJMD@TEXT_AI_OPINION_ACT_COWER_TO_SEL@` | 0x009320EF | 0x01291E4C | literal payload prefix |
| +0xD69 | `??0CCharString@@QAE@PBDJ@Z` | 0x009320F8 | 0x0099EBF0 | reloc_bindings |
| +0xD7D | `?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z` | 0x0093210C | 0x006E3B30 | reloc_bindings |
| +0xD88 | `??_C@_0DF@IJCPLPDA@TEXT_AI_OPINION_ACT_SERVILE_GROV@` | 0x00932117 | 0x01291E14 | literal payload prefix |
| +0xD91 | `??0CCharString@@QAE@PBDJ@Z` | 0x00932120 | 0x0099EBF0 | reloc_bindings |
| +0xDA5 | `?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z` | 0x00932134 | 0x006E3B30 | reloc_bindings |
| +0xDB0 | `??1CCharString@@QAE@XZ` | 0x0093213F | 0x0099EAE0 | reloc_bindings |
| +0xDBC | `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ` | 0x0093214B | 0x0097E200 | reloc_bindings |
| +0xDCC | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x0093215B | 0x008FCF40 | reloc_bindings |
| +0xDDA | `?ConstantFPS@CMainGameComponent@@1JA` | 0x00932168 | 0x01375550 | reloc_bindings |
| +0xDE5 | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x00932174 | 0x008FCFD0 | reloc_bindings |
| +0xDF1 | `?GFFloatRandom@@YIMMPAK@Z` | 0x00932180 | 0x004978A0 | reloc_bindings |
| +0xDF7 | `__real@40a00000` | 0x00932185 | 0x01291DF4 | IEEE literal payload |
| +0xE04 | `?GetFrame@CWorld@@SIJXZ` | 0x00932193 | 0x0049D870 | reloc_bindings |
| +0xE15 | `__ftol2` | 0x009321A4 | 0x00BFEA70 | reloc_bindings |
| +0xE1F | `??_C@_0P@CMOHEFMH@ST_IDLE_SUBTLE?$AA@` | 0x009321AE | 0x01276DBC | terminated literal payload |
| +0xE28 | `??0CCharString@@QAE@PBDJ@Z` | 0x009321B7 | 0x0099EBF0 | reloc_bindings |
| +0xE34 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x009321C3 | 0x00A01B10 | reloc_bindings |
| +0xE3F | `??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@` | 0x009321CD | 0x01237F1C | reloc_bindings |
| +0xE44 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009321D3 | 0x00A01B50 | reloc_bindings |
| +0xE4C | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x009321DB | 0x00A01B90 | reloc_bindings |
| +0xE82 | `??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@ABVCSoundPair@@_N33JJ@Z` | 0x00932211 | 0x008424F0 | reloc_bindings |
| +0xE8A | `?AIAction_PerformActionTargetFace@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@V?$CIntelligentPointer@$$CBVCThing@@@@@Z` | 0x00932219 | 0x0097DF60 | Binding remains unconfirmed |
| +0xE98 | `??1CCreatureActionBase@@UAE@XZ` | 0x00932227 | 0x00693EF0 | reloc_bindings |
| +0xEA1 | `??1CCharString@@QAE@XZ` | 0x00932230 | 0x0099EAE0 | reloc_bindings |
| +0xEB0 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0093223F | 0x00A01B50 | reloc_bindings |
| +0xEB9 | `?IsAwareOfThingIndirectly@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z` | 0x00932248 | 0x00864700 | reloc_bindings |
| +0xED8 | `?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ` | 0x00932267 | 0x0097E200 | reloc_bindings |
| +0xEF3 | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x00932282 | 0x008FCF40 | reloc_bindings |
| +0xF02 | `?Terminate@CAIStateGroupBase@@QAEXXZ` | 0x00932291 | 0x008FCF40 | reloc_bindings |
| +0xF0B | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x0093229A | 0x00A01C10 | reloc_bindings |

201 supported pairs out of 203. See the separate manual audit where supplied.
