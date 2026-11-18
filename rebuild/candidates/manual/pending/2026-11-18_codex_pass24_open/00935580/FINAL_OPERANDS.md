# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x9 | `?PeekPlayerManager@CAIStateGroupBase@@IBEABVCPlayerManager@@XZ` | 0x00935589 | 0x008FCF80 | reloc_bindings |
| +0x10 | `?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ` | 0x00935590 | 0x00449960 | reloc_bindings |
| +0x17 | `?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ` | 0x00935597 | 0x00487DD0 | reloc_bindings |
| +0x58 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x009355D8 | 0x004365B0 | reloc_bindings |
| +0x76 | `?IsPlayerExtendedLineOfSight@CTCPerceiveThing@@QBE_NXZ` | 0x009355F6 | 0x00744830 | reloc_bindings |
| +0x8B | `?ConstantFPS@CMainGameComponent@@1JA` | 0x0093560A | 0x01375550 | reloc_bindings |
| +0x90 | `?GetFrame@CWorld@@SIJXZ` | 0x00935610 | 0x0049D870 | reloc_bindings |
| +0xAA | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00935630 | 0x004C73D0 | reloc_bindings |
| +0xB7 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00935639 | 0x004C73D0 | reloc_bindings |
| +0xC0 | `?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z` | 0x00935642 | 0x006614F0 | reloc_bindings |
| +0xCF | `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ` | 0x00935657 | 0x006A4D00 | reloc_bindings |
| +0xDC | `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ` | 0x00935660 | 0x006A4D00 | reloc_bindings |
| +0x117 | `?GFastOneOverSqrtTable@@3PAKA` | 0x00935699 | 0x013CE558 | reloc_bindings |
| +0x145 | `__real@40400000` | 0x009356C8 | 0x0122DED4 | IEEE literal payload (nearby instruction) |
| +0x14F | `__real@3f000000` | 0x009356D2 | 0x0122F59C | IEEE literal payload (nearby instruction) |
| +0x16F | `__real@00000000` | 0x009356F2 | 0x0122DEDC | IEEE literal payload (nearby instruction) |
| +0x1B2 | `?GFastOneOverSqrtTable@@3PAKA` | 0x00935734 | 0x013CE558 | reloc_bindings |
| +0x1D6 | `__real@40400000` | 0x00935759 | 0x0122DED4 | IEEE literal payload |
| +0x1E0 | `__real@3f000000` | 0x00935763 | 0x0122F59C | IEEE literal payload |
| +0x1FA | `__real@00000000` | 0x0093577D | 0x0122DEDC | IEEE literal payload (nearby instruction) |
| +0x220 | `__real@3ecccccd` | 0x009357A3 | 0x0125A318 | IEEE literal payload |
| +0x231 | `??0?$CIntelligentPointer@$$CBVCThingPlayerCreature@@@@QAE@PBVCThingPlayerCreature@@@Z` | 0x009357BE | 0x00A01B50 | Binding remains unconfirmed |
| +0x23E | `??4?$CIntelligentPointer@$$CBVCThingPlayerCreature@@@@QAEAAV0@ABV0@@Z` | 0x009357C7 | 0x00A01B90 | Binding remains unconfirmed |
| +0x247 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x009357D0 | 0x00A01C10 | reloc_bindings |

22 supported pairs out of 24. See the separate manual audit where supplied.
