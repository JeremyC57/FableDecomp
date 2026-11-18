# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1F | `?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z` | 0x008E9B8E | 0x008E8FF0 | Binding remains unconfirmed |
| +0x2C | `??9CCharString@@QBE_NABV0@@Z` | 0x008E9B9B | 0x0099E900 | reloc_bindings |
| +0x58 | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x008E9BCE | 0x0099EFB0 | reloc_bindings (nearby instruction) |
| +0x6F | `?GetAnimDistanceAndIndexFromAnimName@NCreatureMode@@YI_NABVCThingCreatureBase@@ABVCCharString@@AAJAAM@Z` | 0x008E9BE3 | 0x008E9080 | Binding remains unconfirmed |
| +0x7D | `?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z` | 0x008E9BF3 | 0x00662A00 | reloc_bindings |
| +0x97 | `?GetDelayTime@CAnimationEntry@@QBEJXZ` | 0x008E9C0D | 0x005DC1F0 | reloc_bindings |
| +0xB7 | `?GetTimeToBlendIn@CAnimationEntry@@QBEJABVCThing@@@Z` | 0x008E9C2D | 0x005DC230 | reloc_bindings |
| +0xC2 | `?GetAnimateThingFlags@CAnimationEntry@@QBEKXZ` | 0x008E9C38 | 0x005DC390 | reloc_bindings |
| +0xE9 | `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z` | 0x008E9C5F | 0x0070C050 | reloc_bindings |
| +0xFF | `?SetMainAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ@Z` | 0x008E9C75 | 0x0070D580 | reloc_bindings |
| +0x130 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008E9CA6 | 0x004365B0 | reloc_bindings |
| +0x146 | `?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ` | 0x008E9CBC | 0x006A1440 | reloc_bindings |
| +0x16F | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008E9CE5 | 0x004365B0 | reloc_bindings |
| +0x18B | `?IsProjectileWeapon@CTCWeapon@@QBE_NXZ` | 0x008E9D01 | 0x005DAA40 | reloc_bindings |
| +0x1BE | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008E9D34 | 0x004365B0 | reloc_bindings |
| +0x1D9 | `?GetAbilityMutexLock@CTCSpecialAbilities@@QBE_NXZ` | 0x008E9D4F | 0x0072A860 | Binding remains unconfirmed |
| +0x1EF | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008E9D65 | 0x0070D8C0 | reloc_bindings |
| +0x1F8 | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008E9D6E | 0x0070D8C0 | reloc_bindings |
| +0x201 | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008E9D77 | 0x0070D8C0 | reloc_bindings |
| +0x20A | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008E9D80 | 0x0070C4E0 | reloc_bindings |
| +0x219 | `??_C@_00CNPNBAHC@?$AA@` | 0x008E9D8F | 0x0122D70E | literal payload prefix |
| +0x22D | `?GetCurrentAbilityStrafeAnim@CTCSpecialAbilities@@QBE?AVCCharString@@XZ` | 0x008E9DA3 | 0x0072B870 | Binding remains unconfirmed |
| +0x234 | `??9CCharString@@QBE_NPBD@Z` | 0x008E9DAA | 0x0099E960 | reloc_bindings |
| +0x252 | `??1CCharString@@QAE@XZ` | 0x008E9DC8 | 0x0099EAE0 | reloc_bindings |
| +0x26A | `?GetCurrentAbilityStrafeAnim@CTCSpecialAbilities@@QBE?AVCCharString@@XZ` | 0x008E9DE0 | 0x0072B870 | Binding remains unconfirmed |
| +0x27A | `?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z` | 0x008E9DF0 | 0x00662A00 | reloc_bindings |
| +0x297 | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008E9E0D | 0x0070D8C0 | reloc_bindings |
| +0x29E | `?GetAnimateThingFlags@CAnimationEntry@@QBEKXZ` | 0x008E9E14 | 0x005DC390 | reloc_bindings |
| +0x2C5 | `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z` | 0x008E9E3B | 0x0070C050 | reloc_bindings |
| +0x2EC | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008E9E62 | 0x0070D8C0 | reloc_bindings |
| +0x303 | `?GetBestFrameInAimingAnim@CModeStrafe@NCreatureMode@@ABE_NAAM@Z` | 0x008E9E79 | 0x008E9460 | Binding remains unconfirmed |
| +0x318 | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008E9E8E | 0x0070C4E0 | reloc_bindings |
| +0x34F | `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z` | 0x008E9EC5 | 0x0070C050 | reloc_bindings |
| +0x365 | `?GetTimeToBlendIn@CAnimEntryLite@NAnimationEntryBodge@@QBEJABVCThing@@@Z` | 0x008E9EDB | 0x0073A690 | Binding remains unconfirmed |
| +0x378 | `?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z` | 0x008E9EEE | 0x0070D780 | reloc_bindings |
| +0x381 | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008E9EF7 | 0x0070C4E0 | reloc_bindings |
| +0x38D | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008E9F02 | 0x0129BA3C | reloc_bindings |
| +0x3AE | `__real@3f000000` | 0x008E9F1B | 0x0122F59C | IEEE literal payload |
| +0x3C9 | `__real@3f000000` | 0x008E9F1B | 0x0122F59C | IEEE literal payload (nearby instruction) |
| +0x3DA | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008E9F2C | 0x0129BA3C | reloc_bindings |
| +0x40F | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008E9F62 | 0x0070C4E0 | reloc_bindings |
| +0x42B | `?RemoveCombinationAnim@CAnimateThing@@QAEXPBVCAnimSet@@@Z` | 0x008E9F76 | 0x0070D8E0 | reloc_bindings |
| +0x458 | `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z` | 0x008E9FA3 | 0x0070C050 | reloc_bindings |
| +0x46E | `?GetTimeToBlendIn@CAnimEntryLite@NAnimationEntryBodge@@QBEJABVCThing@@@Z` | 0x008E9FB9 | 0x0073A690 | Binding remains unconfirmed |
| +0x481 | `?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z` | 0x008E9FCC | 0x0070D780 | reloc_bindings |
| +0x48A | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008E9FD5 | 0x0070C4E0 | reloc_bindings |
| +0x49B | `?SetAlpha@CAnimSet@@QAEXM@Z` | 0x008E9FE6 | 0x0070BFC0 | Binding remains unconfirmed |
| +0x4AD | `?SetAlpha@CAnimSet@@QAEXM@Z` | 0x008E9FF8 | 0x0070BFC0 | Binding remains unconfirmed |
| +0x4BC | `?SetInternalAlpha@CAnimSet@@QAEXJM_N@Z` | 0x008EA007 | 0x0070C280 | reloc_bindings |
| +0x4F3 | `?IsAnalogueLoading@CTCWeapon@@QBE_NXZ` | 0x008EA03E | 0x005DA8F0 | Binding remains unconfirmed |
| +0x504 | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008EA04F | 0x0070C4E0 | reloc_bindings |
| +0x53A | `?RemoveCombinationAnim@CAnimateThing@@QAEXPBVCAnimSet@@@Z` | 0x008EA085 | 0x0070D8E0 | reloc_bindings |
| +0x546 | `?IsProjectileWeaponLoaded@CTCWeapon@@QBE_NXZ` | 0x008EA091 | 0x005DA980 | reloc_bindings |
| +0x56D | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008EA0B8 | 0x0040F020 | reloc_bindings |
| +0x5A3 | `??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z` | 0x008EA0EE | 0x005B33F4 | reloc_bindings |
| +0x5B3 | `?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z` | 0x008EA0FE | 0x0070D780 | reloc_bindings |
| +0x5BC | `?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z` | 0x008EA107 | 0x0070C4E0 | reloc_bindings |
| +0x5D9 | `?SetCurrentAnimAsManualUpdate@CTCAnimatingObject@@QAEXXZ` | 0x008EA124 | 0x007F8D00 | Binding remains unconfirmed |
| +0x601 | `?SetFractionComplete@CTCAnimatingObject@@QAEXM@Z` | 0x008EA151 | 0x007F8D60 | Binding remains unconfirmed |
| +0x612 | `?GetTimeToBlendIn@CAnimationEntry@@QBEJABVCThing@@@Z` | 0x008EA162 | 0x005DC230 | reloc_bindings |
| +0x625 | `?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z` | 0x008EA175 | 0x0070D780 | reloc_bindings |
| +0x62E | `??1CCharString@@QAE@XZ` | 0x008EA17E | 0x0099EAE0 | reloc_bindings |
| +0x64E | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008EA19E | 0x0070D8C0 | reloc_bindings |
| +0x678 | `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z` | 0x008EA1C8 | 0x0070C050 | reloc_bindings |
| +0x68F | `?GetTimeToBlendIn@CAnimEntryLite@NAnimationEntryBodge@@QBEJABVCThing@@@Z` | 0x008EA1DF | 0x0073A690 | Binding remains unconfirmed |
| +0x6A2 | `?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z` | 0x008EA1F2 | 0x0070D780 | reloc_bindings |
| +0x6AD | `?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z` | 0x008EA1FD | 0x0070D8C0 | reloc_bindings |
| +0x6BA | `??1CCharString@@QAE@XZ` | 0x008EA20A | 0x0099EAE0 | reloc_bindings |

54 supported pairs out of 68. See the separate manual audit where supplied.
