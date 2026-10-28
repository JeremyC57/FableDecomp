# Chunk 530 (agent530, 2026-10-28): 10 of 20 re-queued single functions landed (321-397 bytes)

Re-queue chunk of 20 rows of 321-397 bytes (0x00B41E50..0x00BCF2E0): engine map, video options, cloth rendering, wind,
billboards, sprites and animated meshes. One Opus 5.5 agent at high effort.

- **Landed:** 10 rows with `vland.py --land` (1 MATCH 0x00B59870 UpdateLoadingDistanceVideoOptions, 9 RELOCATION_MATCH),
  every test PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Parked (10, differ/):** the PeekThemeId / PeekThemeBlend / PeekLandscapeHeight trio (retail reserves a `push ecx` slot
  for mapY), 0x00B46A30 LookupSmallObjectLightCache, 0x00B677D0, 0x00B812F0 static cloth Render, 0x00B9DFC0 (model error
  fixed, needs one regrade), 0x00B9E120, 0x00BCC1C0, 0x00BCEE20 (scheduling / x87 order / frame slots).
- **Landed name fix applied:** 0x00B4A6A0 and 0x00B4A6B0 had both been landed as `CEnginePrimitiveRenderer2D::
  Layer2DToDistance(int)` (one name, two addresses). Ego_r.pdb orders Layer2DToSortDistance before Layer2DToDistance and
  places Layer2DSortDistances at +0x1D8 / Layer2DDistances at +0x8C (retail +0x1D4 / +0x88, the offsets the two bodies
  read), so 0x00B4A6A0 is Layer2DToSortDistance and 0x00B4A6B0 Layer2DToDistance, both taking EEngine2DLayer. Both rows
  and their 5 callers (0x00B9E950, 0x00BD1F70, 0x00BE0CD0, 0x00BE0DE0 (was GetLayerDepth), 0x00BE1BA0) were respelled and
  rechecked (2 MATCH, 5 RELOCATION_MATCH, tests PASS); the 0x00B4A6A0 row file was renamed.

Other name fixes (NAME_FIXES.tsv, 29 rows, the rest deferred): 0x009D58D0 CBankFileAsync::GetAsyncEntry, 0x00A25E70
GetMesh (QBE, counted pointer by reference), 0x00A5D5F0 GetVertexShader (catalog GetPixelShader), 0x00B3EFA0
CEngineMap::ReadMapInfoBlock (private static), 0x00BACDD0 AddPrimitive, 0x00B59710 CEngineComponent ctor. The recheck
surfaced an existing clash: `??0CBase@@QAE@XZ` binds 0x0099A2F0 (CBaseClassNonCopyable ctor, chunk 530) in these callers
and 0x0099A2D0 in landed 0x00DF2380 -- queued for the naming pass.
