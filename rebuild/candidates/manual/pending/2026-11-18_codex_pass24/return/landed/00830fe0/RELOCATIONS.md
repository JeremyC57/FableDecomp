# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xD | `?SerialiseGameState@CThingCreatureBase@@UAE_NAAVCPersistContext@@@Z` | 0x00830FEC | 0x006628E0 | Both PDBs and the base vtable at slot 3 |
| +0x28 | `??_C@_0BB@ILOMKIFK@HasRelationships?$AA@` | 0x00831007 | 0x01272900 | terminated literal payload |
| +0x38 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x00831017 | 0x004045C0 | reloc_bindings |
| +0x3F | `??2@YAPAXI@Z` | 0x0083101E | 0x00BFEA1A | reloc_bindings |
| +0x75 | `?Reset@?$CCountedPointer@VCRelationshipsInit@CThingAICreature@@@@QAEXPAVCRelationshipsInit@CThingAICreature@@@Z` | 0x00831054 | 0x00834D30 | reloc_bindings |
| +0x91 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831070 | 0x00A01B50 | reloc_bindings |
| +0xA0 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0083107F | 0x00A01B50 | reloc_bindings |
| +0xB4 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831093 | 0x00A01B50 | reloc_bindings |
| +0xCD | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x008310AC | 0x00A01B50 | reloc_bindings |
| +0xDC | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x008310BB | 0x00A01B50 | reloc_bindings |
| +0xF0 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x008310CF | 0x00A01B50 | reloc_bindings |
| +0x10A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x008310E9 | 0x00A01B50 | reloc_bindings |
| +0x119 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x008310F8 | 0x00A01B50 | reloc_bindings |
| +0x12D | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0083110C | 0x00A01B50 | reloc_bindings |
| +0x147 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831126 | 0x00A01B50 | reloc_bindings |
| +0x156 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831135 | 0x00A01B50 | reloc_bindings |
| +0x16A | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831149 | 0x00A01B50 | reloc_bindings |
| +0x184 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831163 | 0x00A01B50 | reloc_bindings |
| +0x193 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831172 | 0x00A01B50 | reloc_bindings |
| +0x1A7 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00831186 | 0x00A01B50 | reloc_bindings |
| +0x1BB | `??_C@_00CNPNBAHC@?$AA@` | 0x0083119A | 0x0122D70E | literal payload prefix |
| +0x1C2 | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x008311A1 | 0x00404500 | reloc_bindings |
| +0x1DC | `?WriteFloat@CDataOutputStream@@QAEXM@Z` | 0x008311BB | 0x00993EE0 | reloc_bindings |
| +0x1F2 | `?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z` | 0x008311D1 | 0x0040EFB0 | reloc_bindings |
| +0x1F7 | `??_C@_00CNPNBAHC@?$AA@` | 0x008311D6 | 0x0122D70E | literal payload prefix |
| +0x1FE | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x008311DD | 0x00404500 | reloc_bindings |
| +0x218 | `?WriteFloat@CDataOutputStream@@QAEXM@Z` | 0x008311F7 | 0x00993EE0 | reloc_bindings |
| +0x22E | `?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z` | 0x0083120D | 0x0040EFB0 | reloc_bindings |
| +0x233 | `??_C@_00CNPNBAHC@?$AA@` | 0x00831212 | 0x0122D70E | literal payload prefix |
| +0x23A | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x00831219 | 0x00404500 | reloc_bindings |
| +0x254 | `?WriteFloat@CDataOutputStream@@QAEXM@Z` | 0x00831233 | 0x00993EE0 | reloc_bindings |
| +0x26A | `?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z` | 0x00831249 | 0x0040EFB0 | reloc_bindings |
| +0x282 | `??_C@_0BA@PAACOGFE@HomeBuildingUID?$AA@` | 0x00831261 | 0x01266DA4 | terminated literal payload |
| +0x291 | `??$Transfer@_K@CPersistContext@@QAEXPBDAA_KAB_K@Z` | 0x00831270 | 0x0044BB00 | reloc_bindings |
| +0x2A2 | `??_C@_0BA@FCAPDLNK@WorkBuildingUID?$AA@` | 0x00831281 | 0x01266D94 | terminated literal payload |
| +0x2B1 | `??$Transfer@_K@CPersistContext@@QAEXPBDAA_KAB_K@Z` | 0x00831290 | 0x0044BB00 | reloc_bindings |
| +0x2C2 | `??_C@_0BC@IHPKBNEF@FatherCreatureUID?$AA@` | 0x008312A1 | 0x012728EC | terminated literal payload |
| +0x2D1 | `??$Transfer@_K@CPersistContext@@QAEXPBDAA_KAB_K@Z` | 0x008312B0 | 0x0044BB00 | reloc_bindings |
| +0x2E2 | `??_C@_0BC@DLAJJGLN@MotherCreatureUID?$AA@` | 0x008312C1 | 0x012728D8 | terminated literal payload |
| +0x2F1 | `??$Transfer@_K@CPersistContext@@QAEXPBDAA_KAB_K@Z` | 0x008312D0 | 0x0044BB00 | reloc_bindings |
| +0x302 | `??_C@_0BC@IOMAFNLM@SpouseCreatureUID?$AA@` | 0x008312E1 | 0x012728C4 | terminated literal payload |
| +0x311 | `??$Transfer@_K@CPersistContext@@QAEXPBDAA_KAB_K@Z` | 0x008312F0 | 0x0044BB00 | reloc_bindings |
| +0x33D | `??3@YAXPAX@Z` | 0x0083131C | 0x00BFE9BC | reloc_bindings |
| +0x352 | `??_C@_0BE@GPKAILJE@OverridingBrainName?$AA@` | 0x00831331 | 0x012728B0 | terminated literal payload |
| +0x359 | `??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@@Z` | 0x00831338 | 0x00411010 | reloc_bindings |
| +0x375 | `??_C@_0BK@NKCOEDIA@EnableCreatureAutoPlacing?$AA@` | 0x00831354 | 0x01272894 | terminated literal payload |
| +0x380 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x0083135F | 0x004045C0 | reloc_bindings |
| +0x3AF | `??_C@_0P@POMMGMGP@HasInformation?$AA@` | 0x0083138E | 0x01272884 | terminated literal payload |
| +0x3BA | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x00831399 | 0x004045C0 | reloc_bindings |
| +0x3EC | `??_C@_0BG@MJEFCHKB@WanderWithInformation?$AA@` | 0x008313CB | 0x0127286C | terminated literal payload |
| +0x3F7 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x008313D6 | 0x004045C0 | reloc_bindings |
| +0x429 | `??_C@_0BE@FLMAEOJB@WaveWithInformation?$AA@` | 0x00831408 | 0x01272858 | terminated literal payload |
| +0x434 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x00831413 | 0x004045C0 | reloc_bindings |
| +0x46A | `??_C@_0BK@NEGBALEG@ContinueAIWithInformation?$AA@` | 0x00831449 | 0x0127283C | terminated literal payload |
| +0x471 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x00831450 | 0x004045C0 | reloc_bindings |
| +0x4A5 | `??_C@_0BE@KJLHCLNP@AllowedToFollowHero?$AA@` | 0x00831484 | 0x01272828 | terminated literal payload |
| +0x4B4 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x00831493 | 0x004045C0 | reloc_bindings |
| +0x4E8 | `??_C@_0CE@GIBGAGMF@RegionFollowingOverriddenFromScr@` | 0x008314C7 | 0x01272804 | literal payload prefix |
| +0x4F3 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x008314D2 | 0x004045C0 | reloc_bindings |
| +0x524 | `??_C@_0BK@CBGFEKLC@RespondingToFollowAndWait?$AA@` | 0x00831503 | 0x012727E8 | terminated literal payload |
| +0x530 | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x0083150F | 0x004045C0 | reloc_bindings |
| +0x562 | `??_C@_0N@GEODIMEL@CanBeCourted?$AA@` | 0x00831541 | 0x012727D8 | terminated literal payload |
| +0x56D | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x0083154C | 0x004045C0 | reloc_bindings |
| +0x59F | `??_C@_0N@JOCAFKNO@CanBeMarried?$AA@` | 0x0083157E | 0x012727C8 | terminated literal payload |
| +0x5AA | `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z` | 0x00831589 | 0x004045C0 | reloc_bindings |

65 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
