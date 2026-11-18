# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1B | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x009397FA | 0x00A01B50 | reloc_bindings |
| +0x26 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00939805 | 0x00A01B50 | reloc_bindings |
| +0x36 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00939815 | 0x00A01B50 | reloc_bindings |
| +0x55 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00939834 | 0x0040F020 | reloc_bindings |
| +0x77 | `?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z` | 0x00939856 | 0x007FEF40 | reloc_bindings |
| +0x9D | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0093987C | 0x0040F020 | reloc_bindings |
| +0xC0 | `?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ` | 0x0093989F | 0x0073A6E0 | reloc_bindings |
| +0xD7 | `?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ` | 0x009398B6 | 0x008FCFA0 | reloc_bindings |
| +0xDE | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x009398BD | 0x008FCFD0 | reloc_bindings |
| +0xF5 | `?GFROR13@@YIJK@Z` | 0x009398D4 | 0x00497890 | reloc_bindings |
| +0xFB | `_RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS` | 0x009398D9 | 0x013BB700 | reloc_bindings |
| +0x110 | `?GetFrame@CWorld@@SIJXZ` | 0x009398EF | 0x0049D870 | reloc_bindings |
| +0x116 | `_MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS` | Unpaired | Unpaired | Instruction sequence differs |
| +0x124 | `??0CBrainOutput@@QAE@XZ` | 0x00939905 | 0x00835620 | reloc_bindings |
| +0x143 | `??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z` | 0x00939924 | 0x00858030 | reloc_bindings |
| +0x148 | `?GET_UP_FROM_FLOOR_ANIM_NAME@@3VCCharString@@A` | 0x00939929 | 0x013BB6F0 | NAME_FIXES_QUEUE |
| +0x154 | `??_7CCreatureAction_GetUpFromSitting@@6B@` | 0x00939932 | 0x01276C2C | reloc_bindings |
| +0x167 | `?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z` | 0x00939948 | 0x008574D0 | reloc_bindings |
| +0x177 | `?IsFreeToPerformAction@CThingCreatureBase@@QBE_NABVCCreatureActionBase@@@Z` | 0x00939958 | 0x00662210 | reloc_bindings |
| +0x19B | `?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z` | 0x0093997C | 0x00668D90 | reloc_bindings |
| +0x1A8 | `?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z` | 0x00939989 | 0x0088B8F0 | reloc_bindings |
| +0x1B1 | `?RemoveMode@CTCCreatureModeManager@@QAEXK@Z` | 0x00939992 | 0x0073ABA0 | reloc_bindings |
| +0x1BD | `??1CCombatActionBase@@UAE@XZ` | 0x0093999E | 0x0062E900 | reloc_bindings |
| +0x1C6 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x009399A7 | 0x00A01C10 | reloc_bindings |
| +0x1E8 | `??3@YAXPAX@Z` | 0x009399C9 | 0x00BFE9BC | reloc_bindings |
| +0x211 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x009399F2 | 0x0040F020 | reloc_bindings |
| +0x22A | `?ChooseNewIdle@CTCIdleScheduler@@QAEXXZ` | 0x00939A0B | 0x00801A50 | reloc_bindings |

26 supported pairs out of 27. See the separate manual audit where supplied.
