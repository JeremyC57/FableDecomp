# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x13 | `?START_WEAPON_POINT_NAME@@3VCCharString@@B` | 0x00680062 | 0x013BAC38 | reloc_bindings |
| +0x20 | `??0CCharString@@QAE@ABV0@@Z` | 0x0068006F | 0x0099EC30 | reloc_bindings |
| +0x25 | `?END_WEAPON_POINT_NAME@@3VCCharString@@B` | 0x00680074 | 0x013BAC3C | reloc_bindings |
| +0x2E | `??0CCharString@@QAE@ABV0@@Z` | 0x0068007D | 0x0099EC30 | reloc_bindings |
| +0x33 | `?PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME@@3VCCharString@@B` | 0x00680082 | 0x013BAC30 | reloc_bindings |
| +0x3C | `??0CCharString@@QAE@ABV0@@Z` | 0x0068008B | 0x0099EC30 | reloc_bindings |
| +0x55 | `?PeekStaticMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z` | 0x006800A4 | 0x00A24280 | reloc_bindings |
| +0x6B | `?PeekAnimatedMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z` | 0x006800BA | 0x00A242B0 | reloc_bindings |
| +0x86 | `??$GetSubDef@VCAppearanceDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceDef@@@@@Z` | 0x006800D5 | 0x0042B0A2 | reloc_bindings |
| +0x91 | `??_C@_07MGCPDNLD@DEFAULT?$AA@` | 0x006800E0 | 0x01230554 | literal payload prefix |
| +0x9A | `??0CCharString@@QAE@PBDJ@Z` | 0x006800E9 | 0x0099EBF0 | reloc_bindings |
| +0xBB | `??_7CAnimFilterNoGroup@@6B@` | 0x00680107 | 0x0122FFDC | reloc_bindings |
| +0xC0 | `?AnimExists@CAnimationSet@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z` | 0x0068010F | 0x005DC2E0 | reloc_bindings |
| +0xD7 | `??_7CAnimFilterBase@@6B@` | 0x00680123 | 0x0122FFCC | reloc_bindings |
| +0xEB | `??1CCharString@@QAE@XZ` | 0x0068013A | 0x0099EAE0 | reloc_bindings |
| +0x115 | `?ACTION_POINT_01@@3VCCharString@@B` | 0x00680164 | 0x013BACA0 | reloc_bindings |
| +0x12E | `?GetMeshStats@C3DMeshInfo@@QBE?AV?$CCountedPointer@$$CBVC3DMeshStats@@@@XZ` | 0x0068017D | 0x00A25530 | reloc_bindings |
| +0x13A | `?Calc@CCRC@@SIKKPBXK@Z` | 0x00680189 | 0x004014A0 | reloc_bindings |
| +0x142 | `?HasDummyObject@C3DMeshStats@@QBE_NK@Z` | 0x00680191 | 0x00A2EA60 | reloc_bindings |
| +0x171 | `??3@YAXPAX@Z` | 0x006801C0 | 0x00BFE9BC | reloc_bindings |
| +0x18D | `??1CCharString@@QAE@XZ` | 0x006801DC | 0x0099EAE0 | reloc_bindings |
| +0x196 | `??1CCharString@@QAE@XZ` | 0x006801E5 | 0x0099EAE0 | reloc_bindings |
| +0x19F | `??1CCharString@@QAE@XZ` | 0x006801EE | 0x0099EAE0 | reloc_bindings |
| +0x1AE | `?NullChar@CCharString@@1DB` | 0x006801FD | 0x0129AAF4 | reloc_bindings |

24 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
