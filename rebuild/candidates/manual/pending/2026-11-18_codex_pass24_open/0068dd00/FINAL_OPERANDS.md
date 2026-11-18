# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x4 | `___security_cookie` | 0x0068DD03 | 0x0139C8A8 | reloc_bindings |
| +0x32 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0068DD29 | 0x00A01B50 | reloc_bindings |
| +0x46 | `?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z` | 0x0068DD3D | 0x00687CF0 | reloc_bindings |
| +0x57 | `?IsEventButtonPressed@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z` | 0x0068DD4E | 0x00687A90 | reloc_bindings |
| +0x67 | `?IsEventButtonReleased@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z` | 0x0068DD5E | 0x00687B30 | reloc_bindings |
| +0x78 | `?IsEventButtonHeld@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z` | 0x0068DD6F | 0x00687BD0 | reloc_bindings |
| +0xAA | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068DDA2 | 0x004365B0 | reloc_bindings |
| +0xC6 | `?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ` | 0x0068DDB8 | 0x006A1440 | reloc_bindings |
| +0x10F | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068DDFC | 0x004365B0 | reloc_bindings |
| +0x174 | `?AddData@CGameEvent@@QAEXJPBX@Z` | 0x0068DE82 | 0x009F1760 | reloc_bindings (nearby instruction) |
| +0x192 | `?AddData@CGameEvent@@QAEXJPBX@Z` | 0x0068DE82 | 0x009F1760 | reloc_bindings (nearby instruction) |
| +0x1A3 | `?AddGameEvent@CProcessedInput@@QAEXABVCGameEvent@@@Z` | 0x0068DE90 | 0x00A0D340 | reloc_bindings |
| +0x1B0 | `@__security_check_cookie@4` | 0x0068DE9D | 0x00BFE9F9 | reloc_bindings |
| +0x1DB | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068DEC4 | 0x004365B0 | reloc_bindings |
| +0x212 | `?IsThingVulnerableToMeleeStrikesFromThing@NTargeting@@YI_NABVCThing@@0@Z` | 0x0068DEF5 | 0x007F59F0 | reloc_bindings |
| +0x223 | `?IsTargetedThingOfType@CTCTargetingBase@@QBE_NW4ETargetingType@@@Z` | 0x0068DF06 | 0x007F5A70 | reloc_bindings |
| +0x28E | `?AddGameEvent@CProcessedInput@@QAEXABVCGameEvent@@@Z` | 0x0068DF6E | 0x00A0D340 | reloc_bindings |
| +0x29B | `@__security_check_cookie@4` | 0x0068DF7B | 0x00BFE9F9 | reloc_bindings |
| +0x2D8 | `?AddData@CGameEvent@@QAEXJPBX@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x2E9 | `?AddGameEvent@CProcessedInput@@QAEXABVCGameEvent@@@Z` | 0x0068DFAC | 0x00A0D340 | reloc_bindings |
| +0x2F6 | `@__security_check_cookie@4` | 0x0068DFB9 | 0x00BFE9F9 | reloc_bindings |
| +0x305 | `?IsTargetedThingOfType@CTCTargetingBase@@QBE_NW4ETargetingType@@@Z` | 0x0068DFC8 | 0x007F5A70 | reloc_bindings |
| +0x36D | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068E02E | 0x004365B0 | reloc_bindings |
| +0x390 | `?GetTutorialCategory@CTCActionUseBase@@QBE?AW4ETutorialCategory@@XZ` | 0x0068E050 | 0x007EAC00 | Binding remains unconfirmed |
| +0x3A0 | `?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ` | 0x0068E060 | 0x0049D9D0 | reloc_bindings |
| +0x3A7 | `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` | 0x0068E067 | 0x00487DC0 | reloc_bindings |
| +0x3B6 | `??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z` | 0x0068E076 | 0x0043DBD0 | reloc_bindings |
| +0x3DF | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068E099 | 0x0040F020 | reloc_bindings |
| +0x3FD | `?GetTutorialCategory@CTCActionUseBase@@QBE?AW4ETutorialCategory@@XZ` | 0x0068E0B7 | 0x007EAC00 | Binding remains unconfirmed |
| +0x405 | `?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z` | 0x0068E0BF | 0x006C9FA0 | reloc_bindings |
| +0x443 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068E105 | 0x004365B0 | reloc_bindings |
| +0x492 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068E14B | 0x0040F020 | reloc_bindings |
| +0x4AA | `??_C@_0BF@MCIJCCJO@CTCActionUseReadable?$AA@` | 0x0068E162 | 0x0123A0A8 | terminated literal payload |
| +0x4BB | `??0CCharString@@QAE@PBDJ@Z` | 0x0068E16B | 0x0099EBF0 | reloc_bindings |
| +0x4CF | `??8CCharString@@QBE_NABV0@@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x4F8 | `??1CCharString@@QAE@XZ` | Unpaired | Unpaired | Instruction sequence differs |
| +0x508 | `??1CCharString@@QAE@XZ` | 0x0068E1C2 | 0x0099EAE0 | reloc_bindings |
| +0x575 | `?AddData@CGameEvent@@QAEXJPBX@Z` | 0x0068E227 | 0x009F1760 | reloc_bindings |
| +0x593 | `?AddData@CGameEvent@@QAEXJPBX@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x5A4 | `?AddGameEvent@CProcessedInput@@QAEXABVCGameEvent@@@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x5B1 | `@__security_check_cookie@4` | 0x0068E2B4 | 0x00BFE9F9 | reloc_bindings (nearby instruction) |
| +0x5E7 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0068E260 | 0x0040F020 | reloc_bindings |
| +0x5FF | `??_C@_0P@JMJDEJMN@DOOR_OPEN_FAIL?$AA@` | 0x0068E278 | 0x0125BF18 | terminated literal payload |
| +0x60B | `??0CCharString@@QAE@PBDJ@Z` | 0x0068E281 | 0x0099EBF0 | reloc_bindings |
| +0x622 | `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z` | 0x0068E295 | 0x0066BD00 | reloc_bindings |
| +0x62E | `??1CCharString@@QAE@XZ` | 0x0068E29E | 0x0099EAE0 | reloc_bindings |
| +0x63A | `?SetAsPerformedEvent@CProcessedInput@@QAEXXZ` | 0x0068E2A7 | 0x00A0D390 | reloc_bindings |
| +0x647 | `@__security_check_cookie@4` | 0x0068E2B4 | 0x00BFE9F9 | reloc_bindings |

41 supported pairs out of 48. See the separate manual audit where supplied.
