# egod2a header overlay (copies of today's rebuild/include files; every addition under a NEW opt-in, default paths unchanged)

| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CEngineLandscapeMeshBuilder.h | FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES | public BuildSharedIndexBufferGenerationTables(C2DCoordI*, unsigned short*, unsigned short*, long, long, long&) decl (0x00BE2CD0) | Ego_r mangled name, Ego_d parameter names; caller 0x00BE3110 |
| engine/CPhysicsMesh.h | FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO | private CalculateBoundingInfo() decl (0x00A6C640) + TotalPoints typed CArray<C3DVector> (same 12 bytes at +0x3C; FaceIndices stay raw) | FableWin ?CalculateBoundingInfo@CPhysicsMesh@@AAEXXZ; Ego_d |
| engine/C3DBoundingBox.h | FABLE_RETAIL_C3DBOUNDINGBOX_SETMINMAX | SetMinMax(min, max) and GetDiagonal() = (Max - Min) * 0.5f bodies | FableWin ?SetMinMax@ (60b/24i = ours), ?GetDiagonal@ (58b/22i = GetCenter's size) |
| engine/C3DBoundingSphere.h | FABLE_RETAIL_C3DBOUNDINGSPHERE_SET | Set(const C3DVector&, float) body | FableWin ?Set@C3DBoundingSphere@@QAEXABVC3DVector@@M@Z 44b/18i = ours |
| engine/CCircle.h | FABLE_RETAIL_CCIRCLE_SET | Set(const C2DVector&, float) body | FableWin ?Set@CCircle@@QAEXABVC2DVector@@M@Z 38b/16i = ours; COMDAT right after CalculateBoundingInfo |
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE_GROUNDHEIGHT | public float GetGroundHeight(const C3DVector&) decl (0x00880200); use together with FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP (the block re-opens `protected:` after it) | reloc_bindings ?GetGroundHeight@CTCPhysicsBase@@QAEMABVC3DVector@@@Z (landed 0x008E5480); row 0x007243C0 calls at 0x007243CC / 0x007245F5 |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_IMPASSABILITYAROUND | GetImpassabilityFactorAround(const C3DVector&, std::vector<float>&, bool) const decl (0x0063E3C0) + extern const C2DCoordI g_BlockNeighbourOffsets[9] (0x01385978); needs FABLE_RETAIL_CWORLDMAP | Ego_r.pdb mangled name; reloc_bindings ?g_BlockNeighbourOffsets@@3QBVC2DCoordI@@B |
