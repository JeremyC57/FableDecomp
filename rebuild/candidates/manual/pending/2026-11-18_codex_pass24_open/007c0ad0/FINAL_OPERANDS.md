# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x4 | `?g_bShotsEnabled@@3_NA` | 0x007C0AD3 | 0x0137573F | reloc_bindings |
| +0x87 | `?GetCreatureFirePosAndOrientation@NThingShot@@YIXABVCThing@@AAVC3DVector@@AAVCRightHandedSet@@@Z` | 0x007C0B56 | 0x007BF000 | Binding remains unconfirmed |
| +0x8D | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` | 0x007C0B5B | 0x013B86A0 | reloc_bindings |
| +0x99 | `?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ` | 0x007C0B68 | 0x00415FBC | reloc_bindings |
| +0xAB | `??$GetDef@VCThingShotDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingShotDef@@@@@Z` | 0x007C0B7A | 0x0078B230 | reloc_bindings |
| +0xCC | `?GetDefClassTypeNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z` | 0x007C0B9B | 0x009ACCE0 | reloc_bindings |
| +0xD9 | `?GDefStringTable@@3VCDefStringTable@@A` | 0x007C0BA8 | 0x013CA828 | reloc_bindings |
| +0xDE | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x007C0BAD | 0x009D49B0 | reloc_bindings |
| +0xEF | `??_C@_00CNPNBAHC@?$AA@` | 0x007C0BBA | 0x0122D70E | literal payload prefix |
| +0xF4 | `??_C@_06IAMCMNFI@OBJECT?$AA@` | 0x007C0BBF | 0x01235FF4 | literal payload prefix |
| +0x108 | `??_C@_00CNPNBAHC@?$AA@` | 0x007C0C34 | 0x0122D70E | literal payload prefix |
| +0x10D | `??_C@_08OIKLHEDK@CREATURE?$AA@` | 0x007C0C39 | 0x01236054 | literal payload prefix |
| +0x12A | `??_C@_00CNPNBAHC@?$AA@` | 0x007C0C56 | 0x0122D70E | literal payload prefix |
| +0x133 | `??0CCharString@@QAE@PBDJ@Z` | 0x007C0C5F | 0x0099EBF0 | reloc_bindings |
| +0x13A | `??_C@_00CNPNBAHC@?$AA@` | 0x007C0C66 | 0x0122D70E | literal payload prefix |
| +0x143 | `??0CCharString@@QAE@PBDJ@Z` | 0x007C0C6F | 0x0099EBF0 | reloc_bindings |
| +0x14E | `?GetPlayer@CThing@@QAEAAVCPlayer@@XZ` | 0x007C0C7A | 0x004C7A30 | reloc_bindings |
| +0x16C | `??0CThingAICreatureInit@@QAE@PAVCThingPhysical@@0_NABVCCharString@@21@Z` | 0x007C0C98 | 0x004FACD0 | reloc_bindings |
| +0x179 | `?Create@CThingAICreature@@SIPAV1@JABVC3DVector@@JABVCThingAICreatureInit@@@Z` | 0x007C0CA5 | 0x00833800 | reloc_bindings |
| +0x184 | `??1CCharString@@QAE@XZ` | 0x007C0CB0 | 0x0099EAE0 | reloc_bindings |
| +0x18D | `??1CCharString@@QAE@XZ` | 0x007C0CB9 | 0x0099EAE0 | reloc_bindings |
| +0x196 | `??1CCharString@@QAE@XZ` | 0x007C0CC2 | 0x0099EAE0 | reloc_bindings |
| +0x19F | `??1CCharString@@QAE@XZ` | 0x007C0CCB | 0x0099EAE0 | reloc_bindings |
| +0x1D1 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x007C0CF9 | 0x0040F020 | reloc_bindings |
| +0x1E9 | `??_C@_06IAMCMNFI@OBJECT?$AA@` | 0x007C0D11 | 0x01236054 | literal payload prefix |
| +0x1F0 | `??8?$CBasicString@D@@QBE_NPBD@Z` | 0x007C0D18 | 0x004115A0 | reloc_bindings |
| +0x1F9 | `??_C@_08OIKLHEDK@CREATURE?$AA@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x200 | `??8?$CBasicString@D@@QBE_NPBD@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x21B | `??_C@_00CNPNBAHC@?$AA@` | 0x007C0D11 | 0x01236054 | literal payload prefix (nearby instruction) |
| +0x227 | `??0CCharString@@QAE@PBDJ@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x230 | `?GetPlayer@CThing@@QAEAAVCPlayer@@XZ` | Unpaired | Unpaired | Instruction sequence differs |
| +0x24E | `?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x26A | `?AddMode@CTCCreatureModeManager@@QAE_NK@Z` | 0x007C0D33 | 0x0073AE10 | reloc_bindings |
| +0x281 | `??_C@_0O@OCHGCOAM@creature_shot?$AA@` | 0x007C0D4A | 0x0126E264 | terminated literal payload |
| +0x28A | `??0CCharString@@QAE@PBDJ@Z` | 0x007C0D53 | 0x0099EBF0 | reloc_bindings |
| +0x296 | `??0CCharString@@QAE@XZ` | 0x007C0D5F | 0x0099E4B0 | reloc_bindings |
| +0x2AD | `??_C@_01IDAFKMJL@_?$AA@` | 0x007C0D76 | 0x01244DB4 | literal payload prefix |
| +0x2B9 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x007C0D82 | 0x0099F600 | reloc_bindings |
| +0x2C7 | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x007C0D90 | 0x0099F570 | reloc_bindings |
| +0x2D4 | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x007C0D9D | 0x0099EFB0 | reloc_bindings |
| +0x2E0 | `??1CCharString@@QAE@XZ` | 0x007C0DA9 | 0x0099EAE0 | reloc_bindings |
| +0x2E9 | `??1CCharString@@QAE@XZ` | 0x007C0DB2 | 0x0099EAE0 | reloc_bindings |
| +0x2FC | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x007C0DC5 | 0x0099EFB0 | reloc_bindings |
| +0x30C | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x007C0DD5 | 0x0099EFB0 | reloc_bindings |
| +0x318 | `??1CCharString@@QAE@XZ` | 0x007C0DE1 | 0x0099EAE0 | reloc_bindings |
| +0x321 | `??1CCharString@@QAE@XZ` | 0x007C0DEA | 0x0099EAE0 | reloc_bindings |
| +0x336 | `?GDefStringTable@@3VCDefStringTable@@A` | 0x007C0DFB | 0x013CA828 | reloc_bindings |
| +0x33B | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x007C0E00 | 0x009D49B0 | reloc_bindings |
| +0x343 | `?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z` | 0x007C0E13 | 0x004C9D60 | reloc_bindings |
| +0x351 | `??1CCharString@@QAE@XZ` | 0x007C0E21 | 0x0099EAE0 | reloc_bindings |
| +0x383 | `?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1PBV4@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z` | 0x007C0E53 | 0x007C0690 | reloc_bindings |
| +0x38C | `??1CCharString@@QAE@XZ` | 0x007C0E5C | 0x0099EAE0 | reloc_bindings |

46 supported pairs out of 52. See the separate manual audit where supplied.
