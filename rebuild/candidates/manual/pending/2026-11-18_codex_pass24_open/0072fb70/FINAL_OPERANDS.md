# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x11 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0072FB80 | 0x00A01B50 | reloc_bindings |
| +0x1C | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0072FB8B | 0x00A01B50 | reloc_bindings |
| +0x30 | `?CanBeRecreated@CTCDummyVillager@@QAE_NXZ` | 0x0072FB9F | 0x0072F9E0 | reloc_bindings |
| +0x69 | `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` | 0x0072FBD8 | 0x006869D0 | reloc_bindings |
| +0x70 | `??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z` | 0x0072FBDF | 0x0042AF3C | reloc_bindings |
| +0x82 | `??$GetSubDef@VCCreatureDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCreatureDef@@@@@Z` | 0x0072FBF3 | 0x00485815 | reloc_bindings |
| +0xA5 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FC16 | 0x00686A70 | reloc_bindings |
| +0xB5 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FC26 | 0x00686A70 | reloc_bindings |
| +0xBC | `?GetPWorldSeed@CTCBase@@IBEPAKXZ` | 0x0072FC2D | 0x00686DA0 | reloc_bindings |
| +0xD1 | `?GFRandom@@YIKKPAK@Z` | 0x0072FC42 | 0x004CDAE0 | reloc_bindings |
| +0xD8 | `?GetFrame@CWorld@@SIJXZ` | 0x0072FC49 | 0x0049D870 | reloc_bindings |
| +0xEC | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FC5D | 0x00686A70 | reloc_bindings |
| +0xF9 | `?GetPWorldSeed@CTCBase@@IBEPAKXZ` | 0x0072FC6A | 0x00686DA0 | reloc_bindings |
| +0x10E | `?GFRandom@@YIKKPAK@Z` | 0x0072FC7F | 0x004CDAE0 | reloc_bindings |
| +0x156 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FCC7 | 0x00686A70 | reloc_bindings |
| +0x15B | `?GetFrame@CWorld@@SIJXZ` | 0x0072FCCC | 0x0049D870 | reloc_bindings |
| +0x167 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FCD6 | 0x00686A70 | reloc_bindings |
| +0x19F | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0072FD0E | 0x0040F020 | reloc_bindings |
| +0x1BB | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | 0x0072FD2A | 0x007EE540 | reloc_bindings |
| +0x1CF | `?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ` | 0x0072FD3E | 0x00686D40 | reloc_bindings |
| +0x1D6 | `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ` | 0x0072FD45 | 0x00449970 | reloc_bindings |
| +0x1DD | `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` | 0x0072FD4C | 0x00487DC0 | reloc_bindings |
| +0x1E4 | `??_C@_0BM@CHOMFIAI@GAZE_OUT_OF_BUILDING_MARKER?$AA@` | 0x0072FD53 | 0x01266DBC | terminated literal payload |
| +0x1F1 | `??0CCharString@@QAE@PBDJ@Z` | 0x0072FD60 | 0x0099EBF0 | reloc_bindings |
| +0x1FB | `?Get@CGameDefinitionManager@@SIAAV1@XZ` | 0x0072FD6A | 0x0044C6B0 | reloc_bindings |
| +0x202 | `?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z` | 0x0072FD71 | 0x009AD410 | reloc_bindings |
| +0x20F | `??1CCharString@@QAE@XZ` | 0x0072FD7E | 0x0099EAE0 | reloc_bindings |
| +0x216 | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | 0x0072FD85 | 0x007EE540 | reloc_bindings |
| +0x23A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0072FDA7 | 0x00A01B50 | reloc_bindings |
| +0x26B | `?_M_insert_overflow@?$vector@PAVCThingMarker@@V?$allocator@PAVCThingMarker@@@std@@@std@@IAEXPAPAVCThingMarker@@ABQAV3@ABU__true_type@2@I_N@Z` | 0x0072FDB9 | 0x0062E7E0 | Binding remains unconfirmed |
| +0x280 | `??$random_shuffle@PAPAVCThingMarker@@@std@@YIXPAPAVCThingMarker@@0@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x2E3 | `??RCThingFilter_IsNotBeingExclusivelyUsed@@QBE_NABVCThing@@@Z` | 0x0072FE1F | 0x006A8AF0 | reloc_bindings |
| +0x2F2 | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FE2E | 0x00686A70 | reloc_bindings |
| +0x2FE | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FE3A | 0x004C73D0 | reloc_bindings |
| +0x31B | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | 0x0072FE57 | 0x00686A70 | reloc_bindings |
| +0x327 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FE63 | 0x004C73D0 | reloc_bindings |
| +0x34C | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FE88 | 0x004C73D0 | reloc_bindings |
| +0x373 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FEAF | 0x004C73D0 | reloc_bindings |
| +0x37C | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FEB8 | 0x004C73D0 | reloc_bindings |
| +0x3DB | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FF17 | 0x004C73D0 | reloc_bindings |
| +0x429 | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | 0x0072FF75 | 0x007EE540 | reloc_bindings |
| +0x435 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FF85 | 0x004C73D0 | reloc_bindings |
| +0x452 | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | 0x0072FFA8 | 0x007EE540 | reloc_bindings |
| +0x469 | `?CreateVillager@CTCDummyVillager@@QAEPAVCThingAICreature@@AAVCTCVillage@@ABVC3DVector@@_N@Z` | 0x0072FFBF | 0x0072F750 | reloc_bindings |
| +0x474 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0072FFCA | 0x004AB8F0 | Binding remains unconfirmed |
| +0x483 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00730017 | 0x00A01B50 | reloc_bindings (nearby instruction) |
| +0x493 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0072FFD9 | 0x00A01B50 | reloc_bindings |
| +0x49A | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FFE0 | 0x004C73D0 | reloc_bindings |
| +0x4A5 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0072FFEB | 0x004C73D0 | reloc_bindings |
| +0x4CA | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00730010 | 0x00A14440 | reloc_bindings |
| +0x4D1 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00730017 | 0x00A01B50 | reloc_bindings |
| +0x501 | `??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z` | 0x00730047 | 0x00487050 | reloc_bindings |
| +0x518 | `_free` | 0x0073005E | 0x00BFEA14 | reloc_bindings |

50 supported pairs out of 53. See the separate manual audit where supplied.
