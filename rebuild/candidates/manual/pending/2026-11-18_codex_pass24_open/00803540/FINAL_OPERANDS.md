# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x27 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00803566 | 0x0040F020 | reloc_bindings |
| +0x42 | `?GetPullBackAmount@CTCHeroRangedAttacks@@QBEMXZ` | 0x00803581 | 0x00789A70 | NAME_FIXES_QUEUE |
| +0x4D | `?GetShotAccuracyFraction@CTCHeroRangedAttacks@@QBEMXZ` | 0x0080358C | 0x00789A80 | Both PDBs and native one-field GetShotAccuracyFraction |
| +0x5E | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0080359D | 0x00686A70 | reloc_bindings |
| +0xAE | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008035EC | 0x0129BA3C | reloc_bindings |
| +0xBF | `__real@3f800000` | 0x008035FD | 0x0122DED8 | IEEE literal payload |
| +0xF3 | `__real@3e124925` | 0x00803631 | 0x012722EC | IEEE literal payload |
| +0xFD | `__real@3dcccccd` | 0x0080363B | 0x0122EE58 | IEEE literal payload |
| +0x12B | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0080366A | 0x004365B0 | reloc_bindings |
| +0x149 | `?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ` | 0x00803688 | 0x00663340 | reloc_bindings |
| +0x17C | `?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ` | 0x008036BB | 0x00663340 | reloc_bindings |
| +0x1AD | `?GetParameter@CHeroAbilityStatWeight@@QBEMABVCTCHeroExperience@@@Z` | 0x008036EC | 0x00851BD0 | reloc_bindings |
| +0x206 | `__real@bf800000` | 0x00803744 | 0x0122DEE0 | IEEE literal payload |
| +0x222 | `__real@bf800000` | 0x00803760 | 0x0122DEE0 | IEEE literal payload |
| +0x22F | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x0080376E | 0x00A14440 | reloc_bindings |
| +0x27A | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x008037B9 | 0x00A14440 | reloc_bindings |
| +0x281 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x008037C0 | 0x00686A70 | reloc_bindings |
| +0x288 | `?GetWorldSeed@CWorld@@QBEPAKXZ` | 0x008037C7 | 0x0049D810 | reloc_bindings |
| +0x294 | `?GFFloatRandom@@YIMMPAK@Z` | 0x008037D3 | 0x004978A0 | reloc_bindings |
| +0x29F | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x008037DE | 0x00686A70 | reloc_bindings |
| +0x2A6 | `?GetWorldSeed@CWorld@@QBEPAKXZ` | 0x008037E5 | 0x0049D810 | reloc_bindings |
| +0x2B2 | `?GFFloatRandom@@YIMMPAK@Z` | 0x008037F1 | 0x004978A0 | reloc_bindings |
| +0x2BC | `?TWO_PI@@3NB` | 0x008037FA | 0x0124F2B8 | reloc_bindings |
| +0x3A9 | `__real@00000000` | 0x008038E7 | 0x0122DEDC | IEEE literal payload |
| +0x3BD | `__real@00000000` | 0x008038FB | 0x0122DEDC | IEEE literal payload |
| +0x3CE | `__real@00000000` | 0x0080390C | 0x0122DEDC | IEEE literal payload |

26 supported pairs out of 26. See the separate manual audit where supplied.
