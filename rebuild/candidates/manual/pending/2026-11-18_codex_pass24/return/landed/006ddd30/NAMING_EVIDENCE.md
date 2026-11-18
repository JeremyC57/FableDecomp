# 0x006DDD30 naming and relocation audit

Ego_r 0x0070B6D0 and FableWin 0x024990D0 agree on `?OnTrophyUseAction@CTCTrophy@@QAEXAAVCThingCreatureBase@@@Z`. The final RTM object emits this leaf. Ego_d identifies the public CTCTrophy member in the trophy module and provides the player, inventory, dialog, and action locals. The shared CPlayerGui singleton supplies the canonical PClass relocation at 0x013B8790. The witness search target 0x006DFA50 matches the full list-and-filter specialization across Ego_r 0x0070D2A0, FableWin 0x0249F8D0 and the Ego_d module. The final source calls this helper out of line.

All 55 operands resolve to the addresses below. Applying them reproduces all 841 retail bytes.

| Operand offset | Retail address | Exact emitted symbol | Evidence |
|---|---|---|---|
| +0x01B | 0x013B8790 | `?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A` | NAME_FIXES_QUEUE |
| +0x026 | 0x00644D54 | `?GetMiniGameType@CDrawQuestInfo@NPlayerGui@@QBE?AW4EMiniGameType@@XZ` | NAME_FIXES_QUEUE |
| +0x035 | 0x00686D40 | `?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ` | NAME_FIXES_QUEUE |
| +0x03C | 0x00449970 | `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ` | NAME_FIXES_QUEUE |
| +0x043 | 0x00633BE0 | `?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ` | NAME_FIXES_QUEUE |
| +0x061 | 0x013B8790 | `?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A` | NAME_FIXES_QUEUE |
| +0x066 | 0x00437CD0 | `?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ` | NAME_FIXES_QUEUE |
| +0x074 | 0x00686D40 | `?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ` | NAME_FIXES_QUEUE |
| +0x07B | 0x00449970 | `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ` | NAME_FIXES_QUEUE |
| +0x082 | 0x00487DC0 | `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` | NAME_FIXES_QUEUE |
| +0x0BB | 0x0040F020 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | NAME_FIXES_QUEUE |
| +0x0F0 | 0x00510D40 | `?clear@?$_List_base@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QAEXXZ` | NAME_FIXES_QUEUE |
| +0x14C | 0x00686A70 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | NAME_FIXES_QUEUE |
| +0x15A | 0x006DFA50 | `??$GetAllThings@VCThingAICreature@@V?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@V?$CFilter_Bind5@VCThingFilter_HasTC@@V1@VCThingFilter_HasScriptSupervision@@V?$CFilter_Not@VCThingFilter_AICreatureHasProperty@@@@V?$CFilter_Not@VCThingFilter_IsLyingDown@@@@@@@CThingSearchTools@@QBEXABV?$CFilter_Bind5@VCThingFilter_HasTC@@V1@VCThingFilter_HasScriptSupervision@@V?$CFilter_Not@VCThingFilter_AICreatureHasProperty@@@@V?$CFilter_Not@VCThingFilter_IsLyingDown@@@@@@AAV?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@@Z` | Exact Ego_r and FableWin names, Ego_d specialization and retail caller |
| +0x172 | 0x006DDCD0 | `?PlaySoundW@CTCTrophy@@AAEKABVCCharString@@@Z` | NAME_FIXES_QUEUE |
| +0x179 | 0x0125FD24 | `??_C@_0BK@CCGCEHAC@TEXT_GUI_TROPHY_PLAY_GAME?$AA@` | Exact object and retail string payload |
| +0x182 | 0x0099EBF0 | `??0CCharString@@QAE@PBDJ@Z` | NAME_FIXES_QUEUE |
| +0x188 | 0x013B86A0 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` | ESTABLISHED_SPELLINGS |
| +0x19A | 0x009C95E0 | `?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z` | NAME_FIXES_QUEUE |
| +0x1A3 | 0x0099EAE0 | `??1CCharString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x1AE | 0x0065F17B | `?size@?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QBEIXZ` | NAME_FIXES_QUEUE |
| +0x1B9 | 0x009C7B80 | `?ConstructString@NGameText@@YI_NAAVCWideString@@JJ@Z` | NAME_FIXES_QUEUE |
| +0x1BE | 0x0125FD08 | `??_C@_1BM@MAMGJOI@?$AAT?$AAE?$AAX?$AAT?$AA_?$AAG?$AAU?$AAI?$AA_?$AAH?$AAE?$AAL?$AAP?$AA?$AA@` | Exact object and retail string payload |
| +0x1C7 | 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` | NAME_FIXES_QUEUE |
| +0x1D4 | 0x0099B720 | `??0CWideString@@QAE@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x1DC | 0x0122D70C | `??_C@_11LOCGONAA@?$AA?$AA@` | Exact object and retail string payload |
| +0x1E1 | 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` | NAME_FIXES_QUEUE |
| +0x1E9 | 0x0122D70C | `??_C@_11LOCGONAA@?$AA?$AA@` | Exact object and retail string payload |
| +0x1EE | 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` | NAME_FIXES_QUEUE |
| +0x1FD | 0x0099B720 | `??0CWideString@@QAE@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x204 | 0x006DD7E0 | `?StartDialogBox@CTCTrophy@@AAEXVCWideString@@K000@Z` | NAME_FIXES_QUEUE |
| +0x20D | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x216 | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x227 | 0x0125FCEC | `??_C@_0BK@KFHLJMKH@TEXT_GUI_TROPHY_NO_PEOPLE?$AA@` | Exact object and retail string payload |
| +0x230 | 0x0099EBF0 | `??0CCharString@@QAE@PBDJ@Z` | NAME_FIXES_QUEUE |
| +0x235 | 0x013B86A0 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` | ESTABLISHED_SPELLINGS |
| +0x247 | 0x009C95E0 | `?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z` | NAME_FIXES_QUEUE |
| +0x250 | 0x0099EAE0 | `??1CCharString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x258 | 0x0122D70C | `??_C@_11LOCGONAA@?$AA?$AA@` | Exact object and retail string payload |
| +0x25D | 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` | NAME_FIXES_QUEUE |
| +0x265 | 0x0122D70C | `??_C@_11LOCGONAA@?$AA?$AA@` | Exact object and retail string payload |
| +0x26A | 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` | NAME_FIXES_QUEUE |
| +0x272 | 0x0122D70C | `??_C@_11LOCGONAA@?$AA?$AA@` | Exact object and retail string payload |
| +0x277 | 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` | NAME_FIXES_QUEUE |
| +0x286 | 0x0099B720 | `??0CWideString@@QAE@ABV0@@Z` | NAME_FIXES_QUEUE |
| +0x28D | 0x006DD7E0 | `?StartDialogBox@CTCTrophy@@AAEXVCWideString@@K000@Z` | NAME_FIXES_QUEUE |
| +0x296 | 0x0099B510 | `??1CWideString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x2B2 | 0x00BFEA1A | `??2@YAPAXI@Z` | NAME_FIXES_QUEUE |
| +0x2D0 | 0x013CA828 | `?GDefStringTable@@3VCDefStringTable@@A` | ESTABLISHED_SPELLINGS |
| +0x2D5 | 0x009D49B0 | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | NAME_FIXES_QUEUE |
| +0x2ED | 0x006DF730 | `??0CCreatureAction_ShowTrophy@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@J@Z` | NAME_FIXES_QUEUE |
| +0x2FB | 0x00668D90 | `?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z` | NAME_FIXES_QUEUE |
| +0x309 | 0x0099EAE0 | `??1CCharString@@QAE@XZ` | NAME_FIXES_QUEUE |
| +0x315 | 0x00664670 | `?SetCurrentAction@CThingCreatureBase@@QAE_NABV?$CCountedPointer@VCCreatureActionBase@@@@@Z` | NAME_FIXES_QUEUE |
| +0x338 | 0x00BFE9BC | `??3@YAXPAX@Z` | NAME_FIXES_QUEUE |
