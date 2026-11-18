# egod2a notes

## Coordinator items
- Every ref/<addr>/egod_locals.txt handed to this lane is EMPTY (0 bytes). The stock lane/egod_locals.py prints only the FIRST
  complete record of a name, so an overloaded name (CRightHandedSet::CRightHandedSet: 4 records, the C3DAngles one is the 4th,
  debug length 545) gives the wrong record. tmp/egod_all.py (the same script printing every complete record) found the right one.

## 0x00A229E0 CRightHandedSet::CRightHandedSet(const C3DAngles&) -- OPEN, DIFFER(689v689), differ/00a229e0.cpp
- Ego_d 4th record: sin_yz cos_yz sin_xy cos_xy sin_xz cos_xz; vx2 vy2 vz2 vxvy vyvz vxvz; stvx stvy stvz; sin_up_yz cos_up_yz
  sin_up_xy cos_up_xy; CMatrix3x4 rotation (no `up` vector local). FableWin /Od 545b/182i.
- What moved it from 124 to 209/222 masked-equal (frame 0x4C equal, all x87 equal): the Ego_d product locals declared just
  before first use with the rotation entries written in retail's COLUMN order (E11 E21 E31 E12 E22 E32 E13 E23 E33, vyvz ahead of
  E12), up = (-sin_xy*sin_yz, -cos_xy*sin_yz, cos_yz) through sin_up_* = -sin_xy..., written into Up's members then
  `rotation.TransformVector(Up)` (FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR, in place): a named `up` C3DVector or a temporary for
  TransformVector(src, dst) costs three frame words.
- Residue: the first Normalise's estimate constant `mov esi,17Ch` is scheduled at 0x00A22ADF (inside the rotation block) where
  retail issues it after the table load (0x00A22BB8); the region cut is elsewhere. Tried (no move): GFSin/GFCos v intrinsic
  sin/cos (same bytes), every Normalise opt-in (SQUARE_HELPER, GROUPED_SUM, SQUARE_GROUPED, PAREN_SQUARES_ZYX, LOCALS, LOCALS_XYZ,
  NAMED_SQUARED, UNPAREN, default), every GFFastOneOverSqrt Newton paren opt-in + ORIGINAL_UNION, parenthesised rotation entries,
  E41..E43 = 0, Forward.Z first, cos_xz-first diagonal, GFSqr named result (probe overlay, removed).
- /Od: this text 518b/173i v FableWin 545b/182i (frame 0x80 both): the ctor text still misses ~9 instructions of IL. FableWin helper
  sizes: Normalise 133b/48i (SQUARE_HELPER text 118b/43i), GFSqr 22b/10i (ours 13b/6i), GFSin/GFCos 30b/10i (= our text without
  the intrinsic pragma), TransformVector 150b/55i (= ours). Next idea: find the 9 missing /Od instructions (more IL before the first
  estimate should move the cut) and the Normalise/GFSqr texts FableWin's sizes prove.

## 0x00BE2CD0 CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables -- OPEN, DIFFER(1091v1088), differ/00be2cd0.cpp
- Old draft was a local model (own C2DCoordI / vector stand-ins); ported onto engine/CArray.h (FABLE_RETAIL_CARRAY +
  GETASCARRAY_START), engine/C2DCoordI.h and a new opt-in FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES (hdr/) declaring the
  method (Ego_r ?BuildSharedIndexBufferGenerationTables@CEngineLandscapeMeshBuilder@@QAEXPAVC2DCoordI@@PAG1JJAAJ@Z; Ego_d parameter
  names verts / tri_striped_indicies / tri_list_indicies / width / height / tri_striped_index_count (long&)).
- Ego_d locals used verbatim: overflow_check (__int64), vertex_id[17][17], temp_tri_striped_buffer (CArray<unsigned short>),
  ofs_table_x / ofs_table_y [2][2][3], vertex_count, poly_count; scopes x { y { poly_within_quad { odd_strip_length,
  needs_order_flip (bool), odd_square, poly_verts[3] { i { idx, idy, vert } } } } }. VC7.1 unrolls the 3-trip i loop (retail has
  three copies), so the loop form is right.
- Residue (same as the old draft): retail keeps constant 1 in edi (the ofs-table 1s and `and edx,edi`, re-set at 0x00BE30C3) and 0 in
  ebx, poly_within_quad in [esp+20h] (`cmp dword ptr [esp+20h],1` 0x00BE2F74), the tri-list cursor &tri_list[poly_count*3+2] in
  [esp+28h]; ours keeps poly_within_quad in edi, `and edx,1`, cursor base +2. The resize value temp shares [esp+10h] with
  poly_count in retail, own slot in ours. Tried: tri-list arms (==1 first / flag), needs_order_flip as the strip test, declaration
  order of the counters / tables (worse or same).

## 0x0059FEE3 CTCInventoryTrade::AddBuyItemToList -- OPEN, DIFFER(1552v1556), differ/0059fee3.cpp
- Ego_d: parameters item_def, quantity, pbuyable_list, pviewport, dynamic; locals buyable, pmanager, pmenu_entry (CCountedPointer
  <CComponent>), pplayer (const CThingPlayerCreature*), ptc_inventory, already_have_special, value_pair (pair<CComponent* const,
  pair<long,long> >), pmenu_entry_real (CMenuEntry*), component_type (CWideString), paction_load_mesh / unload_mesh /
  load_item_description / unload_item_description / load_action_menu / unload_action_menu / load_item_name. The old draft's
  statement list maps onto these one to one; the twin AddBuyItemToListPC (0x005A6CA1, unlanded) has the same set plus `paction`.
- Residue unchanged by the renames, the named value_pair and three declaration orders (ptc_inventory before pmenu_entry, pmanager
  after it, already_have_special declared early): retail splits `this` (home [ebp-0Ch] then esi from 0x0059FF54) and keeps the 0 /
  ptc_inventory in ebx; ours keeps `this` in ebx (c2prio 17 v 13 per the old notes). Next: what lowers `this`'s priority in the
  first stretch (a c2prio trace of the HasTC/GetTC block).

## 0x00A6C640 CPhysicsMesh::CalculateBoundingInfo -- OPEN, DIFFER(517v517), differ/00a6c640.cpp (one instruction pair from a match)
- NAME: the method is PRIVATE: FableWin.pdb ?CalculateBoundingInfo@CPhysicsMesh@@AAEXXZ (0x030F8770; Ego_d record of the same name).
  The old draft's leaf ?CalculateBoundingInfo@CPhysicsMesh@@QAEXXZ is wrong (NAME_FIXES.tsv).
- Ported from the local model onto engine/CPhysicsMesh.h (new opt-in CPHYSICSMESH_CALCULATEBOUNDINGINFO: private declaration +
  TotalPoints typed CArray<C3DVector>), C3DBoundingBox (new SETMINMAX: SetMinMax + GetDiagonal), C3DBoundingSphere (new SET), CCircle
  (new SET) -- every helper body equals FableWin's /Od size (SetMinMax 60b/24i, Set 44b/18i, CCircle::Set 38b/16i; CCircle::Set is
  the COMDAT right after the row in FableWin). Needs FABLE_RETAIL_DEF_CLASS_BASE (the repo CPhysicsMesh.h size assert fails without
  it, also on the default path).
- Ego_d locals: min_vector, max_vector, centre, radial_diagonal (C3DVector), vertex_index (long, for scope), loop_pos (const C3DVector&).
- What closed it to 517v517 / 154 of 161 + frame: centre / radial_diagonal computed from the locals (`(max_vector + min_vector) *
  0.5f`, not BoundingBox.GetCenter()), and BoundingBox.SetMinMax AFTER both (min / max then stay live and the diagonal gets its own
  frame words, retail frame 0x44).
- Residue: `pop edi` scheduled before `fstp [ecx+0A4h]` in retail. No move from: GetMagnitude2D ONE_EXPRESSION / ORIGINAL_SUM,
  C2DVector(centre) (C2DVECTOR_FROM3D) / ToC2DVector, `/ 2.0f`; GetMagnitude opt-ins SQUARED_LOCAL / XY_THEN_Z / NAMED_SQUARED /
  X_THEN_YZ change other things (worse). FableWin sizes not yet matched: GFFastSqrt 22b/9i (ours 19b/8i), GetMagnitude2D 52b/21i
  (ours 53b/21i), GetMagnitude 66b/26i (default text 31b/13i). Next: fit those helper texts (the last region's tuple count).

## 0x007243C0 CTCPhysicsStandard::UpdateVelocityForImpassability -- RELOCATION_MATCH PASS (vland strict), src/ + test/
- Old draft was a local model; rewritten on engine/CTCPhysicsStandard.h (existing IMPULSE opt-in declares the protected method,
  OVERRIDES gives IsOnMeshSurface = slot 55 +0xDC), CTCPhysicsBase.h (existing WORLDMAP + NEW CTCPHYSICSBASE_GROUNDHEIGHT:
  ?GetGroundHeight@CTCPhysicsBase@@QAEMABVC3DVector@@@Z 0x00880200), CWorldMap.h (NEW CWORLDMAP_IMPASSABILITYAROUND: Ego_r
  ?GetImpassabilityFactorAround@CWorldMap@@QBEXABVC3DVector@@AAV?$vector@M...@std@@_N@Z 0x0063E3C0 + ?g_BlockNeighbourOffsets@@3QBVC2DCoordI@@B
  0x01385978), C3DVector (existing GETMAGNITUDEANDNORMALISE 0x00A14480, CLEAR, DOT_ORDERED_XYZ), STLport vector<float>::reserve.
- Ego_d locals verbatim: map_z; block { impassability_factor_around, cell_offset, magnitude, final_force, map_gap, dot, for c1 {
  mag, to_impass { force_component, fade_out_factor } }, { new_z } }.
- What closed it: `final_force.Clear()` (retail zeroes Z, Y, X: the chained-assignment Clear), cell_offset member-wise with
  `cell_offset.Z = 0.0f`, `to_impass = C3DVector((float)offset.X, (float)offset.Y, 0.0f) + cell_offset` (fild Y first, Z folded;
  this also removed the aligned loop head + jmp the ctor spelling produced), then FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_XYZ for
  retail's X, Y, Z dot order (default / XY_THEN_Z / ORIGINAL_SCOPE / GROUPED give other orders).
- vland WARNs (expected): RELOC_NAME 0x00880200 manifest label 'NUISystem::CSpellContainerList::AddChild' is a propagated wrong
  label (reloc_bindings binds GetGroundHeight via the landed 0x008E5480); RELOC_ALIAS __CIfmod = the MSVCR71 import thunk.
- Test links the row object with stub callees and an independent float model; kills mutants of gap < 0.7, mag != 0 (NaN), mag < 1.4
  (corner blocks at 1.414), mag > 1.0 (fade band), dot > 0 and !IsOnMeshSurface. `magnitude > 0.0f` is equivalent (a zero force
  gives dot 0) so no mutant of it is observable.

## 0x00B40260 CEngineMap::PeekHasWaterFast -- RELOCATION_MATCH PASS (vland strict), Flags /d2inlT1267
- The old draft was a local model of a forced-inline PeekLayerValue. Rewritten from the LANDED TWIN CEngineMap::PeekWaterDepth
  0x00B40BD0 (same opt-ins: CENGINEMAP_PEEK / PEEKWATER / PEEK_BODIES / GPENGINEWORLDMAP, CIDRAWMAP(_DRAW), CENGINEWORLDMAP(_GETMAPXY),
  CDEFINITIONMANAGER_GETDEFCLASSINDEX, CENGINELANDSCAPERENDERER(_PEEKTHEME)) plus FABLE_RETAIL_PASS22_WATER_PATCH_LOAD for
  engine/EWaterType.h (WATER_TYPE_NULL, Ego_d EWaterType enumerators). Ego_d: `const long mapx, const long mapy`, t0 at function
  scope, { t1 { t2 } } -> nested ifs, `PeekTheme(GetDefClassIndexFromGlobalIndex(tN)).WaterType != WATER_TYPE_NULL` return true.
- First compile at the twin's /d2inlT576 already had every expanded body right; only the first PeekThemeId site stayed a call.
  Bisected: T1266 fails, T1267 is the smallest flag that expands all three (RELOCATION_MATCH). COORDINATOR: the value is high
  (the twin's is 576); no c2 inline trace tool exists in this lane to quote scores -- the likely cause is the one-function TU
  (in Ego_d's object PeekThemeId's own out-of-line copy is compiled right before this row, so retail costed the first site by the
  cost rule, ours by the budget rule). Check it with the trace before landing; a smaller N may exist with the TU's real order.
- Test links the row object with a fake map / draw map / definition manager / theme table: each layer's `> 0` stop (index 0 is wet,
  so dropping a test reports water), each water-type return, the three-layer limit and both clamps of an off-map cell.

## 0x00A6C640 pass-1 continuation (owner direction: hand-written block + escalation)
- Escalation: the `advisor` tool timed out; this lane has no Agent tool, so no Fable reviewer could be launched (coordinator:
  escalate 0x00A6C640 and 0x00A229E0, both one scheduling decision from a match).
- More tried (no move of `pop edi`): `++vertex_index`; circle radius as C2DVector(...).GetMagnitude() (moves the cut far: pop edi
  lands at 0x00A6C7AB, frame changes -- the cut IS sensitive to the radius helper's IL); a GetMagnitude2D text that equals FableWin's
  /Od size exactly (`float squared=X*X+Y*Y; return sqrt(squared);` 52b/21i, probe opt-in C3DVECTOR_MAGNITUDE2D_SQUARED_SQRT, removed)
  alone and with GetMagnitude SQUARED_LOCAL / XY_THEN_Z / X_THEN_YZ. Next: the GFFastSqrt text (FableWin 22b/9i, ours 19b/8i) and
  GetMagnitude's (66b/26i) together, judged by the pop position.

## Rows read but not started
- 0x0091C220 CAIStateGroup_NymphAttack::IsTargetInLineOfSight: Ego_d (pTarget is CThingCreatureBase*; block { int_normal, delta,
  pCamMan (const CGameCameraManager*), rad, phit_thing (const CThing*), things (vector<const CThing*>), targPos, midPoint, int_pos,
  cam_rad }). Retail: `if (pTarget && !dead-bit +0x91)`, targPos copy, midPoint = (targPos + creature pos) * 0.5f, delta = targPos -
  midPoint, three _ftol (mid X, mid Y, 1.1f * 2D length of delta) -> C2DBoxI, GetWorld()->ThingSearchTools (+0x20)
  GetAllThingsInRoughArea(box, CFilter_Bind2<IsNot, IsNot>, things) 0x0091C9C0, GetWorld()->[+0x34] vcall slot 10 (&pCamMan),
  targPos.Z += 1.0f, pCamMan->Tools (+0x78) IsLineOfSight(things, creature pos, targPos, int_pos, int_normal, cam_rad, &phit_thing,
  7, false) 0x00877840 (NCameraModes_CTools.h declares it). No CAIStateGroup_NymphAttack header exists yet.
- 0x00935C40 CAIStateGroup_MinionPutOutFires::GoToWell: Ego_d ret (EAIActionReturn), pwell (CThingObject*) { ipwell
  (CIntelligentPointer<CThingObject>), nav_route_calculated (bool) { ptc_flammable } }. Needs a new class header (none exists);
  the landed siblings 0x00935AD0 / 0x00935A80 / 0x00936020 are old local models, so they give no include set.
