# 0x00929020 naming and relocation audit

Ego_r 0x0094C210 and FableWin 0x02C65800 agree on `?IsRunnable@CAIStateGroup_BanditPlayDice@@MAE_NXZ`. The final RTM object emits this leaf. Ego_d identifies this function in ai_state_group_bandit_play_dice.obj. Native vtable 0x0128AD54 slot 11 selects it. The marker list at +0x70 is VillageMarkers, as confirmed by the Ego_d list type and shared CTCVillage layout. The actual GetPVillage member precedes all list reads.

All 14 operands resolve to the addresses below. Applying them reproduces all 507 retail bytes.

| Operand offset | Retail address | Exact emitted symbol | Evidence |
|---|---|---|---|
| +0x007 | 0x0049D870 | `?GetFrame@CWorld@@SIJXZ` | ESTABLISHED_SPELLINGS |
| +0x06A | 0x0040F020 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | NAME_FIXES_QUEUE |
| +0x081 | 0x007EE540 | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | NAME_FIXES_QUEUE |
| +0x0AA | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x0B9 | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x0CD | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x0E5 | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x103 | 0x0040F020 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | NAME_FIXES_QUEUE |
| +0x124 | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x146 | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x14F | 0x004C73D0 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | NAME_FIXES_QUEUE |
| +0x158 | 0x004C73D0 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | NAME_FIXES_QUEUE |
| +0x1A0 | 0x00A01B50 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | NAME_FIXES_QUEUE |
| +0x1E0 | 0x0049D870 | `?GetFrame@CWorld@@SIJXZ` | ESTABLISHED_SPELLINGS |
