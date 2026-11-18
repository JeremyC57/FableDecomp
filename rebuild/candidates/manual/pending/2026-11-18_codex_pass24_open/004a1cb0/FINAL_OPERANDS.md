# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1C | `?IsEnvironmentRequiringWeatherMasks@CEnvironment@@QBE_NXZ` | 0x004A1CCB | 0x006B2C00 | Binding remains unconfirmed |
| +0x46 | `??$GetAllLeafThings@VCThingBuilding@@V?$vector@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVCThingFilter_None@@AAV?$vector@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@std@@@Z` | 0x004A1CF5 | 0x004AD700 | Binding remains unconfirmed |
| +0x5F | `?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z` | 0x004A1D0E | 0x00443F90 | reloc_bindings |
| +0x8C | `??$GetDef@VCThingBuildingDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBuildingDef@@@@@Z` | 0x004A1D3B | 0x004AB840 | reloc_bindings |
| +0xD3 | `?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z` | 0x004A1D82 | 0x006AF640 | reloc_bindings |
| +0x1B5 | `__real@3c23d70a` | 0x004A1E63 | 0x01230A5C | IEEE literal payload |
| +0x269 | `__real@3f800000` | 0x004A1F17 | 0x0122DED8 | IEEE literal payload |
| +0x3D1 | `?GetPhysicsMeshObject@CTCPhysicsBase@@QBE?AVCPhysicsMeshObject@@XZ` | 0x004A207F | 0x006B0850 | reloc_bindings |
| +0x443 | `??1CPhysicsMeshObject@@UAE@XZ` | 0x004A20F1 | 0x00A127D0 | reloc_bindings |
| +0x4A5 | `??0CEnginePrimitiveWeatherMask@@QAE@ABVC3DBoundingSphere@@ABVC3DBoundingBox@@ABVCMatrix3x4@@MKW4EEngineInterpolationSet@@@Z` | 0x004A2153 | 0x0049D000 | Binding remains unconfirmed |
| +0x4F0 | `_free` | 0x004A219E | 0x00BFEA14 | reloc_bindings |
| +0x505 | `?PreparePrimitivesForEngine@CThingManager@@QAEXABVCEnginePrimitivePrepareInfo@@@Z` | 0x004A21B3 | 0x0051EAB0 | Binding remains unconfirmed |
| +0x50A | `?RenderingEnabled@NParticleEngine@@3_NA` | 0x004A21B8 | 0x013756EC | Binding remains unconfirmed |
| +0x514 | `?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ` | 0x004A21C2 | 0x00AEAAF0 | reloc_bindings |
| +0x51B | `?PreparePrimitives@CParticleEmitterManager@NParticleEngine@@QAEXABVCEnginePrimitivePrepareInfo@@@Z` | 0x004A21C9 | 0x00AEB990 | Binding remains unconfirmed |

9 supported pairs out of 15. See the separate manual audit where supplied.
