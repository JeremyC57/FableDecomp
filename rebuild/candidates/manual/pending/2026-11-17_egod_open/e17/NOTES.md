# e17 notes

## 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading -- RELOCATION_MATCH (7 grades), test PASS, vland PASS
- Port of the 13th-pass draft onto today's headers with Ego_d's locals: the draft's three longs width/height/depth became Ego_d's
  `C3DExtentsI dimensions(header.Width >> l, header.Height >> l, header.Depth >> l)` (new header hdr/engine/C3DExtentsI.h from the
  Ego_d class record) -> the frame residue (depth homed in the top word, 0x34 v 0x30) closed at the first grade. Other renames from Ego_d:
  max_level, allocate_success, total_size (unsigned, divided), long data_size (the read size), data_offset, mipmap_bytesize, c_level,
  current_size; no `bank` local (GetBank() per use).
- Helpers: row-local TouchTexture / TouchFrame copies replaced by the SHARED CGraphicInfo.h bodies (existing opt-ins
  FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME + _TOUCHFRAME + _GETBANK_DIRECT + _ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22 +
  FABLE_RETAIL_CIVCOUNTEDPOINTER_GET); still RELOCATION_MATCH. Plain-inline TouchFrame gives 1064 bytes (called), so the shared forced
  body is needed (Inline-budget line in the row). Row-local inline bodies kept as the landed same-TU siblings 0x009FF220 / 0x009FE460
  write them: GetIndex, IsVolumeTexture, CGraphicFrame::Sizeof.
- Names: GetAsyncEntry (not CreateEntry) for 0x009D58D0 (Ego_r); TextureLoadingPriority is a `long` (Ego_d ?TextureLoadingPriority@@3JA),
  passed with an (EThreadedFilePriority) cast (byte-identical).
- vland: RELOCATION_MATCH PASS; one WARN RELOC_NAME ?RemoveFromResourceList@CResource@@QAEXXZ -> 0x009FC440 labelled
  'CTCActivationTrigger::OnKill' (a propagated catalog label on a byte-identical body; landed rows 0x00A42EA0 / 0x00A26830 / 0x00A25E70
  bind the same symbol there). Coordinator: relabel 0x009FC440.
- Header overlay: hdr/README.md (2 new headers, 2 new opt-ins).

## 0x00875FB0 NCameraModes::DrawSquare -- OPEN, DIFFER(1315v1315), 6 hunks (~22 grades), test PASS (vt.py)
- Ported off the Codex draft's private macros (FABLE_RETAIL_CAMERA_DRAW / VECTOR_SPLINE, local GFSin/GFCos/TWO_PI) onto shared headers:
  C3DVector.h, GFSin.h, GFCos.h, new hdr/engine/GFTanh.h (FableWin ?GFTanh@@YINN@Z), GFWorldDistance.h (GFSqr), CWorld.h, CMainGameComponent.h,
  CEnginePrimitivePrepareInfo.h / CEnginePrimitiveLine.h / CIEngine.h with the opt-in set of the landed 0x006992A0 (camera draw sibling).
  First port grade = the draft's residue exactly; Ego_d's locals equal the draft's (no new names, c1 in the for block). No /Gr flag needed
  (the function is declared __fastcall).
- Residue: all 1315 bytes' instructions equal; loop-init tuples xor ebx,ebx / mov [esp+18h],ebx / lea esi,[esp+94h] retail at
  0x008760C0..0x008760C8 (right after r_lx_p.X's fstp [esp+0BCh], before the stackifier pops), ours after r_lx_p.Y's store (0x008760D6..E7).
- Tried (no match): same-TU C3DVector bodies of the landed CGeneralCircle 0x00875690 (CROSS_XZ_GROUP, SUBTRACT_XY_GROUP: +1 hunk, the
  += operand order flips), +C3DVECTOR/ACCURATE_MAGNITUDE; SCALE_X_PAREN (moves the init 1-2 slots earlier: 5 hunks), ADD_XY_GROUP (5),
  SUBTRACT_X_GROUP (8), combos SCALE_X_PAREN+ADD_XY_GROUP (6) / +CROSS_XZ_GROUP (5) / ADD_XY+CROSS_XZ (5); SCALE/ADD/SUBTRACT_NAMED_RESULT (8);
  natural parens around (float)GFCos/GFSin in r_lx_p or r_ly_p (7), no casts (6).
- Next idea (needs the scheduler replay tools of rebuild/candidates/manual/pending/2026-11-09_linux_hand892/tools/sched, paths to edit):
  find retail's 81-tuple region cut; the paren variants show the init placement IS cut-sensitive (SCALE_X_PAREN moved it).
- Name fix: the landed caller 0x007617A0 NShape::CShape::EditorDrawHighlightPoints calls 0x00875FB0 as a manifest "EditorDrawHighlightPoints_Impl"
  helper; the real name is NCameraModes::DrawSquare (Ego_r/FableWin/Ego_d; ?DrawSquare@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@1M@Z).

## 0x00F3E120 CTriangleStripifier2::FindBestStartVertex -- OPEN, DIFFER(1411v1411), 4 lines (5 grades), test PASS
- The pass-22 draft compiles unchanged on today's headers (`// Flags: /Gr /d2inlT70`) with the recorded residue: the reload pairs at
  0x00F3E291 and 0x00F3E3C5 swapped (ours loads ebx=[esp+24h] before edi=[esp+7Ch]).
- Ego_d adds nothing new: its locals / types / scopes (phase-1 for body {surround_vert, start_surround_vert, {verts_searched {pl {id1, poly,
  id2, id}}}, {rings, vertex_list, vertex_to_ring_pos_map, vertex_to_ring_map, score}}, phase-2 {start_vert {same five}}, phase-3 {start_vert})
  equal the draft; same-TU helper bodies are already defined in Ego_d's object order (BuildRings, CalcSectionScore, CalcSectionPolyMask,
  StripUnusedVerts, BuildCacheOptimizedVertexList). `long id; for (id = 0; ...)` (Ego_d: id is a pl-body local, not a for-init) and
  declaring id1/id2/id up front are byte-identical.
- Probe (not deliverable): an unrelated function placed before the row changes nothing, so the record ids are per function (the TU's
  earlier functions FillInTriStrip / FillInPolygons / BuildVertexAndPolyMaps are not a lever).
- Test: the pass-22 behaviour test (pending/2026-11-14_codex_pass22/return/parked/00f3e120/00f3e120_test.cpp, same row body) copied; vt PASS.
- Helper bodies BuildRings 0x00F3D980? / BuildCacheOptimizedVertexList 0x00F3BA70? / CalcSectionScore 0x00F3D830 are NOT graded at their own
  addresses by me (the call targets are 0x00F3D980, 0x00F3BA70, 0x00F3D830; only 0x00F3D830 has a catalog name).
