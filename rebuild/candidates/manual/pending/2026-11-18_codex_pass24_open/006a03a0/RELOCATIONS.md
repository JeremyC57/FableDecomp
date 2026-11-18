# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xA | `?Initialise@CCreatureActionBase@@UAEXXZ` | 0x006A03A9 | 0x00694D10 | reloc_bindings |
| +0x30 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A03CF | 0x00A01B50 | reloc_bindings |
| +0x44 | `?ACTION_POINT@@3VCCharString@@B` | 0x006A03DF | 0x013BAC9C | reloc_bindings |
| +0x50 | `?GetDummyObjectNamesStartingWith@CTCGraphicAppearance@NGraphicAppearance@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@@Z` | 0x006A03EB | 0x004BDD70 | Both original PDBs and native graphic prefix call |
| +0x5D | `??$sort@PAVCCharString@@@std@@YIXPAVCCharString@@0@Z` | 0x006A03F8 | 0x005E3A50 | Needs binding evidence |
| +0x8C | `?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z` | 0x006A0427 | 0x004C08C0 | reloc_bindings |
| +0xD0 | `?Compare@?$CBasicString@D@@KIJPBD0@Z` | 0x006A0476 | 0x00411570 | reloc_bindings |
| +0x10D | `?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z` | 0x006A04AA | 0x004C08C0 | reloc_bindings (nearby instruction) |
| +0x117 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A04B4 | 0x00A01B50 | reloc_bindings |
| +0x11E | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x006A04BB | 0x004C73D0 | reloc_bindings |
| +0x142 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A04D7 | 0x00A01B50 | reloc_bindings |
| +0x149 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x006A04DE | 0x004C73D0 | reloc_bindings |
| +0x1F6 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A057D | 0x00A01B50 | reloc_bindings |
| +0x20C | `?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z` | 0x006A0593 | 0x006AF640 | reloc_bindings |
| +0x214 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A059B | 0x00A01B50 | reloc_bindings |
| +0x219 | `?GetPersonalSpaceRadius@CThingCreatureBase@@SIMXZ` | 0x006A05A0 | 0x00661CC0 | reloc_bindings |
| +0x230 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A05B7 | 0x00A01B50 | reloc_bindings |
| +0x237 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A05BE | 0x00A01B50 | reloc_bindings |
| +0x23E | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x006A05C5 | 0x004C73D0 | reloc_bindings |
| +0x248 | `__ftol2` | 0x006A05CF | 0x00BFEA70 | reloc_bindings |
| +0x254 | `__ftol2` | 0x006A05FA | 0x00BFEA70 | reloc_bindings (nearby instruction) |
| +0x267 | `__ftol2` | 0x006A05FA | 0x00BFEA70 | reloc_bindings (nearby instruction) |
| +0x273 | `__ftol2` | 0x006A05DB | 0x00BFEA70 | reloc_bindings |
| +0x284 | `?GFGet2DDistance@@YIMABVC2DCoordI@@0@Z` | 0x006A060B | 0x009E2430 | Both original PDBs and native integer-coordinate distance call |
| +0x28D | `?GetPersonalSpaceRadius@CThingCreatureBase@@SIMXZ` | 0x006A0614 | 0x00661CC0 | reloc_bindings |
| +0x2AB | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x006A0632 | 0x00A01B50 | reloc_bindings |
| +0x2B2 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x006A0639 | 0x004C73D0 | reloc_bindings |
| +0x2C1 | `__real@40200000` | 0x006A0647 | 0x01241D68 | IEEE literal payload |
| +0x2CE | `__real@00000000` | 0x006A0654 | 0x0122DEDC | IEEE literal payload |
| +0x333 | `??1CCharString@@QAE@XZ` | 0x006A06B4 | 0x0099EAE0 | reloc_bindings |
| +0x348 | `_free` | 0x006A06C9 | 0x00BFEA14 | reloc_bindings |

30 verified operand bindings. 1 operands still need an unambiguous instruction or binding.
