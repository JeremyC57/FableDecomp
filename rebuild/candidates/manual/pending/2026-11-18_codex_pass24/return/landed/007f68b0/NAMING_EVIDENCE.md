# 0x007F68B0 naming and relocation audit

Ego_r 0x007F1EE0 and FableWin 0x02536A30 agree on `?GetPBestCreatureForInteractionType@CTCTargetingAI@@UBEPAVCThingCreatureBase@@W4ECreatureInteractionType@@@Z`. The final RTM object emits this leaf. Ego_d records pos, ptc_perceive, area, parent_creature and thing_creature. The native call to 0x007448F0 uses SightRadius at +0x40. The adjacent GetExtendedSightRadius body at 0x00744950 uses +0x44.

All 8 operands resolve to the addresses below. Applying them reproduces all 255 retail bytes.

| Operand offset | Retail address | Exact emitted symbol | Evidence |
|---|---|---|---|
| +0x011 | 0x004C73D0 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | NAME_FIXES_QUEUE |
| +0x033 | 0x004365B0 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | NAME_FIXES_QUEUE |
| +0x057 | 0x00BFEA70 | `__ftol2` | NAME_FIXES_QUEUE |
| +0x061 | 0x00BFEA70 | `__ftol2` | NAME_FIXES_QUEUE |
| +0x06C | 0x007448F0 | `?GetSightRadius@CTCPerceiveThing@@QBEMXZ` | NAME_FIXES_QUEUE |
| +0x071 | 0x00BFEA70 | `__ftol2` | NAME_FIXES_QUEUE |
| +0x0E6 | 0x00686D90 | `?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ` | NAME_FIXES_QUEUE |
| +0x0ED | 0x007F7340 | `??$GetPBestThingInRoughArea@V?$CFilter_Bind3@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_IsAICreatureAwareOf@@V?$unary_negate@VCThingFilter_IsIncapacitated@@@std@@@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_IsAICreatureAwareOf@@V?$unary_negate@VCThingFilter_IsIncapacitated@@@std@@@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z` | Ego_r + FableWin exact symbol, corrected retail signature and native call target |

## Same translation unit

The original predicate call operators are `??RCThingFilter_IsAvailableForCreatureInteraction@@QBE_NABVCThingAICreature@@@Z` at 0x006A7430, `??RCThingFilter_IsAICreatureAwareOf@@QBE_NABVCThingCreatureBase@@@Z` at 0x006A7610 and `??RCThingFilter_IsIncapacitated@@QBE_NABVCThing@@@Z` at 0x006A73F0. Ego_d class types and retail bindings identify the reference parameter for each.

The visible 0x007F7340 rough-area specialization has a separate 594-byte extent. Its emitted body is 591 bytes and differs in 41 instruction hunks, recorded in the receipt's `SAME_TU_HELPERS.json`. Its stack frame and grid traversal remain open. The reserved 255-byte caller restores byte identity after relocating to the independently verified retail specialization.
