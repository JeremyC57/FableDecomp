# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x5C | `?GetEyeGraphic@CThingCreatureBase@@QBEABVCEngineGraphic@@XZ` | 0x0075430D | 0x00662160 | Both original PDBs and native eye graphic access |
| +0xBB | `??0CEnginePrimitiveMorphedStaticMesh@@QAE@XZ` | 0x0075436C | 0x007554C0 | pass 24 naming evidence |
| +0xC7 | `?EYE_LEFT_DUMMY_OBJECT_NAME@@3VCCharString@@B` | 0x00754378 | 0x013BAC54 | reloc_bindings |
| +0xCE | `?GetFirstPrimitiveWithDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBEPBVCPrimitive@2@ABVCCharString@@@Z` | 0x0075437F | 0x004BC410 | Both original PDBs and native left-dummy lookup |
| +0xFA | `??4?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAEAAV01@ABV01@@Z` | 0x007543AB | 0x00728830 | reloc_bindings |
| +0x132 | `?GetFadeDistance@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ` | 0x007543E3 | 0x004BC180 | reloc_bindings |
| +0x152 | `?GetTintColour@CTCGraphicAppearance@NGraphicAppearance@@QBE?BVCRGBColour@@XZ` | 0x00754403 | 0x004BD040 | reloc_bindings |
| +0x166 | `?GetPMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@QAE?AV?$CIVCountedPointer@VCEngineMeshEffect@@@@XZ` | 0x00754417 | 0x004BCF50 | reloc_bindings |
| +0x1D4 | `??0CPrimitive@NGraphicAppearance@@QAE@W4EPrimitiveType@1@J@Z` | 0x00754485 | 0x00642390 | Both original PDBs and native left-primitive constructor |
| +0x1DC | `?AddPersistentPrimitive@CTCGraphicAppearance@NGraphicAppearance@@QAEPAVCPrimitive@2@ABV32@@Z` | 0x0075448D | 0x004C0050 | reloc_bindings |
| +0x1E8 | `??1CPrimitive@NGraphicAppearance@@QAE@XZ` | 0x00754499 | 0x004C4160 | reloc_bindings |
| +0x22B | `?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z` | 0x007544DC | 0x004BBE10 | reloc_bindings |
| +0x23D | `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` | 0x007544EE | 0x00A55F90 | reloc_bindings |
| +0x345 | `?GetRight@CRightHandedSet@@QBE?AVC3DVector@@XZ` | 0x007545F6 | 0x0049CE00 | reloc_bindings |
| +0x352 | `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` | 0x00754603 | 0x00A55F90 | reloc_bindings |
| +0x481 | `??0CEnginePrimitiveAttachmentInfo@@QAE@XZ` | 0x00754736 | 0x0099E4B0 | Needs binding evidence |
| +0x48E | `?EYE_LEFT_DUMMY_OBJECT_NAME@@3VCCharString@@B` | 0x00754744 | 0x013BAC54 | reloc_bindings (nearby instruction) |
| +0x497 | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x0075473F | 0x0099E4B0 | Needs binding evidence |
| +0x49C | `??_C@_00CNPNBAHC@?$AA@` | 0x00754744 | 0x013BAC54 | Needs binding evidence |
| +0x4A9 | `??4CCharString@@QAEABV0@PBD@Z` | 0x00754767 | 0x0099EFE0 | reloc_bindings |
| +0x4EF | `?SetAttachedPrimitiveGraphics@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@AAVCEnginePrimitiveAttachmentInfo@@PBV32@@Z` | 0x007547AD | 0x004BD690 | Both original PDBs and native left primitive attachment |
| +0x4F8 | `??1CEnginePrimitiveAttachmentInfo@@QAE@XZ` | 0x007547BF | 0x0099EAE0 | Needs binding evidence |
| +0x502 | `?EYE_RIGHT_DUMMY_OBJECT_NAME@@3VCCharString@@B` | 0x007547C9 | 0x013BAC58 | reloc_bindings |
| +0x509 | `?GetFirstPrimitiveWithDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBEPBVCPrimitive@2@ABVCCharString@@@Z` | 0x007547D0 | 0x004BC410 | Both original PDBs and native right-dummy lookup |
| +0x52C | `??0CPrimitive@NGraphicAppearance@@QAE@W4EPrimitiveType@1@J@Z` | 0x007547F3 | 0x00642390 | Both original PDBs and native right-primitive constructor |
| +0x534 | `?AddPersistentPrimitive@CTCGraphicAppearance@NGraphicAppearance@@QAEPAVCPrimitive@2@ABV32@@Z` | 0x007547FB | 0x004C0050 | reloc_bindings |
| +0x540 | `??1CPrimitive@NGraphicAppearance@@QAE@XZ` | 0x00754807 | 0x004C4160 | reloc_bindings |
| +0x583 | `?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z` | 0x0075484A | 0x004BBE10 | reloc_bindings |
| +0x598 | `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` | 0x0075485F | 0x00A55F90 | reloc_bindings |
| +0x6D0 | `?GetRight@CRightHandedSet@@QBE?AVC3DVector@@XZ` | 0x00754997 | 0x0049CE00 | reloc_bindings |
| +0x6DA | `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` | 0x007549A1 | 0x00A55F90 | reloc_bindings |
| +0x7D9 | `??0CEnginePrimitiveAttachmentInfo@@QAE@XZ` | 0x00754AA4 | 0x0099E4B0 | Needs binding evidence |
| +0x7E6 | `?EYE_RIGHT_DUMMY_OBJECT_NAME@@3VCCharString@@B` | 0x00754AB2 | 0x013BAC58 | reloc_bindings (nearby instruction) |
| +0x7EF | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x00754AAD | 0x0099E4B0 | Needs binding evidence |
| +0x7F4 | `??_C@_00CNPNBAHC@?$AA@` | 0x00754AB2 | 0x013BAC58 | Needs binding evidence |
| +0x801 | `??4CCharString@@QAEABV0@PBD@Z` | 0x00754AD5 | 0x0099EFE0 | reloc_bindings |
| +0x847 | `?SetAttachedPrimitiveGraphics@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@AAVCEnginePrimitiveAttachmentInfo@@PBV32@@Z` | 0x00754B1B | 0x004BD690 | Both original PDBs and native right primitive attachment |
| +0x850 | `??1CEnginePrimitiveAttachmentInfo@@QAE@XZ` | 0x00754B2D | 0x0099EAE0 | Needs binding evidence |
| +0x861 | `??1CEnginePrimitiveMorphedStaticMesh@@QAE@XZ` | 0x00754B3E | 0x00755520 | pass 24 naming evidence |

31 verified operand bindings. 8 operands still need an unambiguous instruction or binding.
