## 0x00867B40


## 0x00867B40 CCombatAction_GenericStrikeResponseKnockdown::FinishAction -- author attempt 1 (wf9, 2026-11-12)
**Result: the code matches retail; the REGISTERED ORACLE IS 3 BYTES SHORT.**
- Final source `00867b40.cpp` (= `ready/payload.json`): fg.py vs the registered oracle = `DIFFER(468v465)`, one hunk, "insert `add esp,0x128 ; ret`"
  at the very end. The registered oracle ($S/batch/wf9_oracle.tsv, manifest length 465) is a strict prefix of the real function: Fable.exe
  0x00867B40..0x00867D13 is 468 bytes (`... 5d 81 c4 28 01 00 00 c3`, then int3 to 0x00867D20); the oracle stops inside the last
  `add esp,128h` (ends `81 c4 28 01`, missing `00 00 c3`). A 465-byte target cannot be matched by any source.
- Against the true 468 bytes (read from Fable.exe into `shadow/batch/wf9fix_oracle.tsv`; fg.py run with `S=$W/shadow`, nothing outside
  $W written): `RELOCATION_MATCH`, also with `LANDVERIFY_STRICT_HEADERS=1`. Behaviour test `00867b40_test.cpp`: vt.py PASS.
- **Coordinator action:** correct the row's oracle / manifest length 465 -> 468, then land `ready/payload.json` + `ready/oracle.tsv`
  (written by addmatch.py with the 468-byte oracle; kept out of $W root because the registered grade is not MATCH). The twin
  0x00867D20 CCombatAction_GenericResponseToExplosion::FinishAction (hand261: "same residue", also 468v465-like) probably has the same cut.
- What made it match (the old "register tie" was model error, as the dossier predicted): (1) CThing::GetTC(&modes) from the shared
  header under FABLE_RETAIL_CTHING_GETTC_EXPANDED (the manager stays in the key's frame slot, the index goes to edi);
  (2) a NAMED `const C3DAnimationInfo* animation_info = PeekWorld().PeekMeshBank()->PeekAnimation(index);` before
  `GetAnimationStats()` (chained, the hidden result slot is pushed before the PeekAnimation call); (3) LeaveDeadCreature through the
  inline `CThing::PeekBaseDef()` (body in CThingBaseDef.h under FABLE_RETAIL_CTHING; `GetPBaseDef()` copies the CDefPointer = refcount code);
  (4) real class sizes for the three temporaries (Die 0xAC, BeStunned 0xAC, Recover 0x118) share the one 0x118 slot by themselves.
- Headers / flags: no compiler flag, no pragma. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD,
  FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_FINISHACTION, FABLE_RETAIL_CCREATUREACTION_DIE. Existing opt-ins used: see the
  #define block (C3DAnimationInfo from the shared header needs the CMeshDataBank retail chain + FABLE_RETAIL_BASE_OBJECT,
  CRESOURCE_RESOURCELIST, CRESOURCELIST, SCOPED_PTR). Local views only for classes with no engine header: CCreatureAction_BeStunned
  (member `Flip` +0xA8 per the quarantine header), CCombatAction_GenericStrikeResponseKnockdownRecover (derives the shared RecoverBase).
- Names: nothing made up. IsKnockdownToStun is declared only (anonymous namespace, __fastcall; its landed body is row 0x00865AC0).
- Landed names proved wrong (NAME_FIXES.tsv): 0x00692950 landed "Dest_val" is ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ;
  0x004C7AB0 landed "CViewBase::GetDisplayEngine" is ?PeekWorld@CThing@@IBEABVCWorld@@XZ (both in egor_pdb_names.tsv and FableWin signatures).
- Ideas tried (grades): v1 port onto shared headers (472v465, 9 hunks: hidden-result order, PeekBaseDef called out of line, flip
  register rotation); v2/v3 GetPBaseDef().Get() / operator-> (refcount copy, worse); v2 + FABLE_RETAIL_CTHING PeekBaseDef body (code
  identical to retail); v4..v7 header purity (C3DAnimationInfo from the shared header). Next idea: none for the code; fix the oracle length.

## 0x00867B40 -- author attempt 2 (wf9, 2026-11-12): attempt 1's finding re-verified independently; nothing left to change in the source
**Blocker is the registered oracle, not the code.** No source edit was made (the source already equals retail), so no new spellings were graded.
- Re-read from Fable.exe with an independent PE reader: 0x00867B40 runs to 0x00867D13 = **468 bytes** (`... 5f 5e 5d 81 c4 28 01 00 00 c3`, then
  int3 to 0x00867D20). `$S/batch/wf9_oracle.tsv` holds 465 bytes that are a strict prefix of the exe and stop INSIDE the last instruction
  (`81 c4 28 01`; the next exe bytes are `00 00 c3 cc`). A function cannot end in half an `add esp,imm32`, so no source can grade MATCH
  against the registered 465 bytes. q.py reads oracles only from the repo catalog, `$S/batch/*_oracle.tsv` and the repo batch folders --
  all outside $W, so the author cannot correct it.
- Grades this attempt (all on `00867b40.cpp`, unchanged): registered oracle = `DIFFER(468v465)`, 1 hunk = "ours has `add esp,0x128 ; ret`
  after `pop ebp`" (exactly the 7 bytes retail has at 0x00867D0D); corrected 468-byte oracle (`shadow/batch/wf9fix_oracle.tsv`, fg.py with
  `S=$W/shadow`) = `RELOCATION_MATCH`, and again with `LANDVERIFY_STRICT_HEADERS=1`; `vt.py 00867b40 00867b40_test.cpp` = PASS.
- `ready/payload.json` + `ready/oracle.tsv` (468 bytes, verified prefix-equal to the exe and followed by int3) hold the row in landing
  format; payload source/test are byte-identical to `00867b40.cpp` / `00867b40_test.cpp`. No banned tokens, `// Leaf:` line present, no flags.
- **Coordinator:** fix this row's length 465 -> 468 in the wf9 oracle / manifest, then land from `ready/`. Do not send a third author at
  the 465-byte target. In the same run only one other row is not followed by padding or a 16-byte boundary: 0x005BE8F3 (482 bytes, next exe
  bytes `56 8b f1 8b` -- looks like a fragment that runs into the next function; check its length too). The twin 0x00867D20
  (CCombatAction_GenericResponseToExplosion::FinishAction) is not in the wf9 oracle; its first `ret`+int3 in the exe is at length 507 --
  compare with its manifest length before authoring it.
- Best next idea for an author: none. The remaining step is the oracle length correction.

## 0x00867B40 -- author attempt 3 (wf9, 2026-11-12): same finding a third time; one new fact for the coordinator
**Still blocked by the registered oracle length, not by the code.** No source edit (nothing in the code differs from retail).
- Grades this attempt (source = `differ/00867b40.cpp`, copied to `b1.cpp`): registered oracle `DIFFER(468v465)`, 1 hunk = ours has
  `add esp,0x128 ; ret` after `pop ebp`, which is retail 0x00867D0D..0x00867D13; corrected 468-byte oracle (shadow S under $W) with
  `LANDVERIFY_STRICT_HEADERS=1` = `RELOCATION_MATCH`; `vt.py` = PASS. `$S/batch/wf9_oracle.tsv` still holds 465 bytes ending `81 c4 28 01`.
- New fact: the repo itself already records the right length. `ghidra_out/coverage.tsv` line 22133 lists
  `00867b40 ?FinishAction@CCombatAction_GenericStrikeResponseKnockdown@@UAEXXZ ... 468`. So the 465 is a wf9 / parked-batch (hand261) row
  error, not a Ghidra function-bounds question. Two sources for 468: Fable.exe bytes (ret at 0x00867D13, then int3) and coverage.tsv.
- "Change the level" was considered and does not apply: structure, callee visibility, identities and flags all already give retail's
  bytes; the target ends inside an `add esp,imm32`, so no compiler output can equal it.
- Coordinator: set the row length to 468 (bytes in `shadow/batch/wf9fix_oracle.tsv`), then land `ready/payload.json` + `ready/oracle.tsv`
  with the `hdr/` overlay. Do not send a fourth author at the 465-byte target.

## 0x00762BA0


## 0x00762BA0 CTCShapeManager::OnSerialise -- RELOCATION_MATCH (wf9 author, attempt 1, 4 grades incl. strict regrade)
- What made it match: the port onto the shared headers with the dossier's identities (signature `(CPersistContext&, bool)`, C2DCoordI offset,
  `PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos))`, two-argument `Transfer(name, value)` named defaults, typed vector iterator +
  CCountedPointer copy) left only the arm order: `if (!context.IsSaving()) { load } else { save }` with ONE trailing
  `SelectedShape = PersistantShapes.end();` (the compiler duplicates it into the four exits by itself) and the bounds written
  BRX, BRY, TLX, TLY as four plain stores. `if (IsSaving()) { save } else { load }` gave 456v468 with the slots swapped.
- Headers / flags: no flags. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CTCSHAPEMANAGER_ONSERIALISE (engine/CTCShapeManager.h),
  FABLE_RETAIL_NSHAPE_CSHAPE_STATONSERIALISE (engine/NShape_CClosedShapeBase.h). Row defines GetDefaultVal<bool>/<long> specialisations
  (as the landed 0x00761C00 row does for long/float).
- Inferred: the first IsSaving if / else has the same Transfer statement in both arms (retail keeps two call set-ups).
- Names proved wrong in the old draft (not landed spellings unless the callee rows use them): 0x004C73D0 is CThing::GetPos (landed file is
  named CThing_GetPosition_004c73d0.cpp; PDB ?GetPos@CThing@@QBEABVC3DVector@@XZ); 0x00686D20 is CTCBase::PeekWorldMap (not GetWorldMap);
  0x00762620 is NShape::CShape::StatOnSerialise (static __fastcall, Ego_r 0x007736A0 + FableWin), not a CTCShapeManager member.
- Made-up names: none (locals only).

## 0x00A12AB0


## 0x00A12AB0 CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume -- RELOCATION_MATCH (strict headers too; test vt.py PASS)
- What made it match: the twin's body (0x00A128E0: named `CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix)`, two-argument
  `TransformPoint(pos, local)`, `float scaled_radius = radius * OO_Scale;` after the transform, named counted copy
  `CIVCountedPointer<const CPhysicsMesh> mesh(Mesh);`) plus ONE float-specific fact: the call's result goes through a double-precision
  expression that folds away -- `return (float)(mesh->PeekRoughProportionOfSphereContained(name, local, scaled_radius) * 1.0);`.
  Without it the only residue is `call ; fst [esp+4Ch] ; test esi,esi ; je` (ours) v `call ; test esi,esi ; fst [esp+4Ch] ; je` (retail,
  0x00A12C63..69; the only such site in Fable.exe).
- REVIEW FLAG for the coordinator: `* 1.0` is an INFERRED spelling. The bytes prove an x87 tuple between the call and the result store
  (VC7.1 lets the integer test overtake an fp store only when an fp operation precedes the store: landed 0x006F9130 has
  `call ; fdiv ; mov ; test ; fstp ; je`); a float -> double -> float round trip is the one that emits no instruction. Which double
  expression the original used cannot be recovered from the bytes. Both PDBs give the callee and this function a `float` return (M).
- Spellings that leave the residue (do not repeat): plain `return mesh->...` (named copy / GetMesh() temporary / const-ref bound temporary);
  named `float result` (same scope, declared first, inner scope for the mesh); `return scaled_radius = ...`, `return result = ...`;
  result into `radius` / `scaled_radius`; address-taken or array result; `* 1.0f`; `(float)(double)` and `(long double)` casts;
  release spellings _RELEASE_INNER, _DTOR_DIRECT, _RELEASE_MEMBER + _RELEASE_DELETES; declaration-order changes; the twin's real body earlier
  in the TU; flags /G5 /G7 /Oa. A named `double result` gets the tail right but reorders the cofactor products (8 hunks); with the
  GetMesh() temporary it grades 466 v 468. `(float)(GetMesh()->...(...) * 1.0)` also matches; the named copy is kept to agree with the twin.
- Headers / flags: no flags, no pragmas. Opt-ins FABLE_RETAIL_DEF_CLASS_BASE, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX,
  _CIVCOUNTEDPOINTER, _CMATRIX3X4, _CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS (existing) plus the twin row's two declaration-only opt-ins
  FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME and FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME; $W/hdr holds unchanged copies of
  $S/w22/wf8/00a128e0/hdr/engine/{CPhysicsMesh.h,CPhysicsMeshObject.h} (one opt-in name for both rows; land the twin's overlay once).
- Names: Ego_r.pdb + FableWin.pdb for the function and the callee (0x00A6CA80 CPhysicsMesh::PeekRoughProportionOfSphereContained). The old
  draft's `long volume` parameter and callee name `...InVolume` on CPhysicsMesh were wrong (draft only, nothing landed). Parameter and
  local names are descriptive (no PDB locals). No landed name proved wrong.
- Grades used: 45 variant grades (ATTEMPTS.log) (incl. 2 probes: volatile result, by-value inline helper) + 1 retail byte-pattern scan.

## 0x00726160


## 0x00726160 CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature::operator()(CThing*) const -- RELOCATION_MATCH (wf9 author, 28 graded variants incl. 2 probe calls)
- What made it match: port onto the shared headers (CThing / CTCBase / CTCPhysicsBase / CTCSolidToThing / CWorld / CMainGameComponent /
  C2DVector / C3DVector / GFFloatRandom) + both solid-to-thing tests written as `CThing::GetTC<CTCSolidToThing>(&solid)` into ONE
  address-taken pointer local (`if (a.GetTC(&solid) && !solid->IsSolidToThing(x)) return;`). That out-pointer form fixed BOTH the
  2nd test's `mov ecx,[eax+4]` before the Player->ParentThing reload AND the stubborn radius-sum order (`fld [Player+34h] ; fadd
  [thing->PhysicsTC+34h]`, ecx loaded before edx), which no spelling of the sum itself moved (13 grades: operand order, +=, named
  locals, inline helper, direct members, early returns, const/non-const all left the same 3 hunks).
- Other facts: player position reference taken BEFORE thing->GetPos() (esi = Player+0Ch); C2DVector delta (2 floats); `dX*dX + dY*dY`;
  nested ifs (early returns swap the push / facing-temp slots); sibling 0x00726340's sign/angle/turns idiom; PeekWorld() const.
- Headers / flags: no compiler flags. Macros: FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CTCPHYSICSBASE,
  _CTCSOLIDTOTHING, _CTCSOLIDTOTHING_ISSOLIDTOTHING (NEW opt-in, overlay hdr/engine/CTCSolidToThing.h: declares IsSolidToThing
  0x00770DE0), _C3DVECTOR, _CMAINGAMECOMPONENT, _CMAINGAMECOMPONENT_WORLD.
- Names: manifest label `CApplyAdditionalImpulseIfInCombatAndTooClose::operator()` for 0x00726160 is WRONG (propagated); real name
  per FableWin.pdb ??RCApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature@@QBEXPAVCThing@@@Z + Ego_r _For_each
  instantiation / 4-byte PDB layout (CTCPhysicsBase* Player). Landed spellings proved wrong (see DOSSIER NAME_FIXES): the functor in
  std_foreach_00726660.cpp and in CTCPhysicsControlled_UpdateAdditionalImpulse_00725b70.cpp (member `CTCPhysicsControlled* Physics`
  -> `CTCPhysicsBase* Player`); 0x00726340 row's local vslot names GetCollidesWithThings / GetVelocity / AddAdditionalImpulse ->
  CanCollideWithThings (slot 57) / GetFacingVector (slot 70) / ApplyControlledImpulse (slot 81) per engine/CTCPhysicsBase.h.
- No made-up names. Lesson: an operand-order residue in an unrelated later expression can be decided by whether an earlier component
  lookup used the address-taken `GetTC<T>(&ptr)` form or a reference local -- try the out-pointer GetTC<T> first.

## 0x00A90900


## 0x00A90900 N3DPrimitive::CDeviceIndependent::AddTriangleList(const C3DMeshFileTrianglesChunk&) -- RELOCATION_MATCH (12 grades)
- What made it match: (1) the port onto the shared headers with the PDB shapes (private `const C3DMeshFileTrianglesChunk&`, adopting
  CIVCountedPointer(new CTriangleList), GetList/GetCount, FableWin inline accessors ResizeTriangleList / SetTriangle / ResizeStrip /
  SetStripIndex, an empty CCountedPointer strip + SetStaticCast(FindFirstSubChunkOfType(GetStaticChunkType())), the public inline
  AddTriangleList(const CIVCountedPointer&) overload = push_back) gave every instruction at the first grade, frame 0x1C v 0x20;
  (2) `#define FABLE_RETAIL_STLP_VECTOR` (STLport _M_fill_insert body visible, still called out of line) fixed the frame: the
  never-written C3DTriangle2() resize value then shares the slot that holds `this` instead of the strip pointer's slot.
- Needs: opt-ins FABLE_RETAIL_STLP_VECTOR, DEF_CLASS_BASE, CIVCOUNTEDPOINTEEBASE_CTOR, CIVCOUNTEDPOINTER, _FROM_RAW, _RELEASE_INNER,
  CCHUNKEDFILECHUNK, CCHUNKEDFILESUPERCHUNK, C3DMESHFILETRIANGLESCHUNK (+ new _ACCESSORS), C3DMESHFILEXTRIANGLESTRIPCHUNK (+ new
  _ACCESSORS); overlay headers hdr/engine/C3DMeshFileTrianglesChunk.h and hdr/engine/C3DMeshFileXTriangleStripChunk.h. No flags, no pragma.
- Local models (no shared header exists): N3DPrimitive::CTriangleList, N3DPrimitive::CDeviceIndependent (Ego_r member names; Groups is
  CArray<CCountedPointer<C3DGroup2> > per struct_layouts_egor.tsv + FableWin.pdb ?AddGroup@CDeviceIndependent@N3DPrimitive@@QAEXAAV?$CCountedPointer@VC3DGroup2@@@@@Z,
  via the shared engine/C3DGroup2.h with opt-in FABLE_RETAIL_C3DGROUP2).
- Repair (verifier finding 1): Groups retyped from the guessed CArray<CIVCountedPointer<CGroup> > to the PDB type, made-up N3DPrimitive::CGroup
  dropped; regrade RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), vt.py PASS.
- Did NOT change the residue (same 17 frame-offset hunks): resize called directly / with an explicit C3DTriangle2() / accessor with an
  explicit value; a named `C3DTriangle2 blank` at the top (own slot, frame 0x24); a raw `new_list` local; the lookup result as a named
  const local in a block (470 bytes); push_back direct / through FABLE_RETAIL_CARRAY_PUSHBACK; FABLE_RETAIL_CARRAY begin()-index.
- Name fix candidate: landed 0x00ABA370 `CWorldMap_DrawGetNoMaps` (free __fastcall) is C3DMeshFileTrianglesChunk::GetCount() const
  (see NAME_FIXES.tsv). Test: 00a90900_test.cpp (vt.py PASS). Strict-headers regrade: RELOCATION_MATCH.

## 0x008DC720


## 008dc720 NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect -- RELOCATION_MATCH (3 grades, also under LANDVERIFY_STRICT_HEADERS=1)
- What made it match: the landed twin's shape (CModeDragonFireBeam::StartBeamEffect 0x008DB620) without the early return, moved onto the
  shared headers: GetSubDef<CSpecialEffectsDef> through engine/CDefClassBase_GetSubDef.h (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE,
  the REAL body out of line) and GetEffectFromString through engine/CSpecialEffectsDef.h (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY,
  real body) -- no stand-in bodies (the twin still carries stand-ins; this spelling would let it drop them). The counted def temporary is
  `ParentThing.GetPBaseDef()` by value; GetTC(TCI_PARTICLE_EMITTER, &emitter) with FABLE_RETAIL_CTHING_GETTC_EXPANDED;
  operator= from FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN; LastExplodeFrame = CWorld::GetFrame() before Create; FlameActive = true
  right after the assignment.
- Headers / flags: repo headers only, no overlay, no pragma, no Flags line. Opt-ins: see the #define block of the source
  (CTCDPARTICLEEMITTER_FULL for Create, CMODEBASE_VIRTUALS + CMODEBASE_PARENTTHING for the shared CTCCreatureModeManager::CModeBase,
  CTCGRAPHICAPPEARANCE_DUMMYOBJECT). The dragon mode chain (CModeDragonFlyingBase / CModeDragonFlying / CModeDragonStrafeBurninate) is a
  local model: no shared header exists for them (same chain as the landed OnDeactivate 0x008DCB00), now derived from the shared CModeBase.
- Grades: 1 = twin source adapted with its stand-ins (baseline, RELOCATION_MATCH, not kept); 2 = shared-header port SRC_FAIL (protected
  PTCGraphicAppearance -> GetTCGraphicAppearance(); Create needs CTCDPARTICLEEMITTER_FULL); 3 = RELOCATION_MATCH; 4 = strict regrade.
- Name fixes (candidates, evidence Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z = 0x006E0880): landed rows that
  spell 0x006E0880 `CreateEffectThing` (e.g. CCreatureActionJackFloatUp_PerformAction_008c0fb0.cpp) should use CTCDParticleEmitter::Create.
  The old parked draft's Beam / BeamStartTime / BeamActive / GetWorldFrame were wrong (PBeamWeaponEffect / LastExplodeFrame / FlameActive /
  CWorld::GetFrame per struct_layouts_egor.tsv and the landed UpdateBeam 0x008DC900).
- Made-up names: none.

## 0x006F3A30

## 0x006F3A30 std::_S_sort<const CThingCreatureBase*, allocator, CThingCompare_Nearest> -- RELOCATION_MATCH (3 grades + 2 regrades)
- What made it match: port onto the shared STLport header (verbatim STLport 4.6 `_S_sort`, list::splice/swap/_Transfer, explicit
  instantiation, FABLE_STLP_FASTCALL) AND the `_S_merge` BODY visible in the TU (it is still called out of line). With `_S_merge`
  declared only the result is the old draft's 496v481 residue (carry head reloaded from the frame, `= 0` ctor store kept, frame
  0x108); with the body visible c2 knows the call leaves carry's head word alone -> carry._M_node._M_data in ebx, frame 0x10C,
  no null test on carry's final free. The comparator's operator() must be declared only (as retail: out-of-line 0x006EF290).
- Needs: overlay hdr/stlport/fable_containers.h (`FABLE_RETAIL_STLP_LIST_SORT`) and hdr/engine/CThingCompare_Nearest.h
  (`FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE`); no flags, no pragmas. Strict-headers regrade passes. Test: 006f3a30_test.cpp
  (uses the shared header's list::sort / _S_sort with a same-shape comparator; needs the overlay, FABLE_HDR_OVERLAY=$W/hdr).
- Name: `_S_sort` is STLport's own name (no PDB carries it; the mangled leaf is constructed from the STLport signature + /Gr);
  element/comparator types from both PDBs' `??$sort@VCThingCompare_Nearest@@@?$list@PBVCThingCreatureBase@@...` (Ego_r 0x0071A120,
  FableWin 0x02433550). Manifest label `std::list::sort` is the caller-side member (the real member is the forwarder 0x006F4040).
- Landed spellings that look wrong (from the dossier, evidence above; see NAME_FIXES.tsv): _S_merge 0x006F04C0 models
  SMarkerList/CThingMarker (real: list<const CThingCreatureBase*>); caller 0x006EBA60 spells the comparator CCompareByDistance
  (real: CThingCompare_Nearest) and the target `list::sort`; 0x006F2BB0 `CRetailList_node0xc::Initialise` is the list default-ctor closure.
- Side observation (not graded): the `_S_merge` instance this TU emits is the same STLport body as landed 0x006F04C0.
- Repair (verifier finding 1): header comment no longer claims nearest-first ordering; it states the retail quirk
  (CThingCompare_Nearest::operator() 0x006EF290 returns the nearer T*, _S_merge only null-tests it at 0x006F04E1..E8, so any two
  non-null creatures compare true). Test header and comparator comment now call the bool predicate a stand-in that does not copy
  the retail result type. Regrade RELOCATION_MATCH (also strict headers); vt.py PASS; addmatch rerun.

## 0x0085D180

# 0x0085D180 NThingMap::CThingMap::InitMapwhoLevels -- RELOCATION_MATCH (wf9 author, first attempt)

Rows matched: 1 (0085d180). Row in progress: none. Rows untouched: none. Probes: 13. Grades: 55 (ATTEMPTS.log).

## What made it match
- Ported onto the shared headers (engine/NThingMap_CThingMap.h MAP_VALIDATE branch, C2DBoxI::GetExtents, STLport vector) with the real
  globals NThingMap::MaxLayers / NThingMap::LayerInfo[]. All four resizes are the ONE-argument `resize(n)`.
- `#define FABLE_RETAIL_STLP_VECTOR` (visible _M_fill_insert body, as in the retail TU): c2 then drops the destructor of the
  `vector<CThingMapCell>()` temporary after the Cells.resize call (declaration-only keeps a `cmp / je / free`).
- The inline split (resize<CThingMapCell> expanded in the `size == -1` arm, called at 0x0085D317 in the other) is a site-frequency
  fact, not a device: the second arm is written `else if (size != -1)`. The test folds away (no code), but c2's static frequencies
  put that arm lower, so the two-argument resize (cost about 80 at frequency 1: probe 7) stays a call there, while the constant-count
  site in the first arm is expanded with erase's copy loop. With a plain `else` everything is expanded (531v481).
- Loop top: `long size = LayerInfo[i].Size;` declared BEFORE `MapwhoLevelInfo[i].NoThings = 0;` (retail loads the size first and
  stores -1 as an immediate; with the store first VC7.1 stores the size register instead).
- No flags, no pragmas. Strict-headers regrade passes. Test: tests/0085d180_test.cpp (vt.py PASS).

## Headers / flags it needs
- Flags: none. Defines: FABLE_RETAIL_MAP_VALIDATE, FABLE_RETAIL_C2DBOXI, FABLE_RETAIL_STLP_VECTOR and the two new opt-ins in
  hdr/engine/NThingMap_CThingMap.h (FABLE_RETAIL_CTHINGMAP_INLINE_CTORS, FABLE_RETAIL_CTHINGMAP_LAYERINFO; hdr/README.md).

## Ideas tried that did not give the split (do not repeat)
- FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (noinline resize): both Cells[i] sites called (378v481).
- Explicit two-argument resize at one or both sites; resize spelled through STLport's insert() wrapper; raw BRX-TLX extents: all 531.
- `while` loop, `continue` in the first arm, inverted `!=` arms, `switch (size)`: all expand both sites (531 / 534).
- `if (size == -1 || size == -1)`: right split but the arms are laid out in the other order (470v481).
- /d2inlT sweeps 40..130 on the plain-else form: T43-44 expands resize in the first arm only but leaves erase called; T57 expands
  the first arm fully but calls the one-argument wrapper in the second; no value gives retail. In-body `#pragma inline_depth(1)`
  has no effect here; `inline_depth(0)` also stops operator[] / the constructor at that statement.
- Probes (ATTEMPTS.log): no caller-size budget at this size (extra resize sites all expand); an earlier user or explicit
  instantiation of resize<CThingMapCell> changes nothing; resize<CThingMapCell> two-argument at frequency 1 is called up to
  /d2inlT80 (n variable) / T78 (n = 1); in a `for` body it is expanded in if/else arms and called under `a && b` or a nested else.

## Landed names proved wrong
- 0x0085E6C0 is landed as `vector<CUnprovenEnum4_0085dc30>::_M_fill_insert`: its element is NThingMap::CThingMapCell (called at
  0x0085D280 on Cells[i] with a CThingMapCell temporary; NAME_FIXES.tsv).

## 0x005BE8F3


## 0x005BE8F3 NInventory::CTCInventoryBase::CreateInventoryObject -- RELOCATION_MATCH (2 grade calls, 4 variants)
- What made it match: the port onto the shared headers (expanded out-parameter CThing::GetTC, same-TU IS_THING_ALIVE<CThing*> real body under auto_inline(off), `CThing* thing = 0` at the top, CThingObject::Create / GetPos / GetPlayerNumber) left only the min: `long max = augmentations->GetMaximumNumberOfSlots(); long count = std::min(max, (long)data->Augmentations.size());` -- the maximum is a LOCAL evaluated before the vector size (as a direct argument VC7.1 evaluates the size first, right to left, 480 v 482).
- Needs: optimize("y",off) + ("s",on); no extra flags. Header overlay ($W/hdr, see hdr/README.md): FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_SLOTS (engine/CTCObjectAugmentations.h), FABLE_RETAIL_CTCINVENTORYITEM_CONNECTTOINVENTORY (engine/CTCInventoryItem.h). Strict-headers regrade passes.
- Local model: CTCTrophy (no shared header; Ego_r.pdb members PDef +0x0C, NonMinigameDeedHandle +0x10, MinigameInProgress +0x14, BestWitnessesAheadToDate +0x18) -- real PDB names, nothing made up. A shared engine/CTCTrophy.h would be the follow-up.
- Landed names proved wrong (from the dossier, used here in the PDB spelling): 0x005D8E40 is ?ConnectToInventory@CTCInventoryItem@@QAEXAAVCTCInventoryBase@NInventory@@@Z (Ego_r 0x00559DA0, directly before ReturnToInventory; landed as CObservable::AddExclusiveObserver / SetMember_dword0x10_005d8e40); 0x005BE8D3 is ?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z (landed as GetBoughtPrice).

## 0x008C6860


## 0x008C6860 CCreatureAction_TrollThrowProjectile::PerformAction -- RELOCATION_MATCH (wf9 author, first attempt)
- What made it match (in order): (1) port onto the shared headers with the dossier's PDB spellings (483v482 at once: `PeekBaseDef().GetSubDef`,
  `GetWorld().PeekDefinitionManager().GetDef`, expanded `GetTC(&carrying)`, `IS_THING_ALIVE(PShot)`), with the GetSubDef<T> / GetDef<T> template
  bodies visible but out of line (shot def read back once into ebp, its frame word reused); (2) CThingShotDef::ShotDefinitionClassName as a real
  CDefString read through `.GetString()` into a block-scoped CCharString -- brings back the shot def's null tests (a direct `shotDef->..._TablePos`
  read lets VC7.1 drop them) and puts GetString ahead of AddTC's pushes; (3) the null melee-ability CDefPointer is a named local of EACH branch
  (v12). One pointer ahead of the `if` (block or not) swaps the weapon-def / shot-def frame words and edi/ebp (481v482, 38 hunks); a temporary
  argument per branch leaves the zero store inside each branch (490v482, 3 hunks).
- Headers / flags: no flags, no pragmas. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE,
  FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION, FABLE_RETAIL_CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE, FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING.
  Strict-headers regrade: RELOCATION_MATCH. Test 008c6860_test.cpp PASS (vt.py). No local class models, no stand-in bodies (the two template
  bodies are the shared-header ones).
- Ideas graded (12 variants, 8 fg.py calls): v1 temp-arg GetString on the flat TablePos member (483, no shot-def null tests, frame 0x18);
  v2 block-local name via the CDefString conversion (512, extra copy); v3 conversion as a temp argument (489); v4 block-local `.GetString()`
  (489, frame 0x14); v5 ability in its own block (481, slots/regs swapped); v6 temp ability argument per branch (490, 3 hunks);
  v7 v5 + FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (497, worse -- not needed, the default Release gives the melee release shape here);
  v8 const-ref-bound temp, v9 `ability(NULL_DEF)` (both 489); v10/v11 the same two inside a block (481); v12 named local per branch MATCH.
- Made-up names: none for functions/members; locals only (carrying, weaponDef, shotDef, accuracy, shot, name, ability, shooter). Parameter
  names of the Init position overload follow the landed thing overload (descriptive).
- Landed spellings seen wrong (NAME_FIXES.tsv): siblings 0x008F3630 / 0x008C0900 / 0x008F3100 / 0x008C64F0 call 0x00692940 `GetPWorld` and the shot
  def's +0x60 member `TCName`; PDB: ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ and CThingShotDef::ShotDefinitionClassName (Ego_r layout).
  engine/CDefStringTable.h's line-3 comment says the global table is `g_DefStringTable`; the retail global at 0x013CA828 is `GDefStringTable` (Ego_r egor_pdb_names.tsv 0x0101AC00 `dynamic initializer for 'GDefStringTable'` + FableWin _fablewin_api.tsv 0x0400AB10 / 0x040125D0 initializer/atexit destructor for 'GDefStringTable'; engine/CDefString.h already reads `GDefStringTable.GetString`). Comment-only; no code symbol to respell.
- Callee 0x00663B10: landed row CThingCreatureBase_GetShotAccuracyPercentage_00663b10.cpp has leaf `?GetShotAccuracyPercentage@CThingCreatureBase@@QAEJXZ`; both PDBs spell it const `QBEJXZ` (Ego_r 0x0053E950; FableWin 0x017CE205 / 0x01D8C0B0) -- in NAME_FIXES.tsv.
- Dossier corrections: FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF is NOT wanted; no forceinline/by-value GetDef wrapper is needed for the shot
  def in ebp (the visible template body does it); the weapon def's missing null test needs no hand release.

## Repair pass (verifier findings)
- Overlay CThingCreatureBase.h: `long GetShotAccuracyPercentage() const;` with the true QBEJXZ citation (Ego_r + FableWin addresses); hdr/README.md fixed; test model method made const. Regrade RELOCATION_MATCH (also strict headers); vt.py PASS.
- NAME_FIXES: 0x00692940 now cites Ego_r 0x005192A0 + FableWin 0x017BA372 / 0x01DB6460; new row for 0x00663B10 (QAEJXZ -> QBEJXZ). GDefStringTable comment fix now carries address 0x013CA828 + two sources.

