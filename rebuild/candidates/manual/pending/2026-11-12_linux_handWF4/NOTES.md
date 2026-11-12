## 0x004B3CE0


## 0x004B3CE0 CQuestManager::ActivateQuests -- RELOCATION_MATCH (wf4 author, 2026-11-12; normal + LANDVERIFY_STRICT_HEADERS=1; vt.py PASS; no flags)
Files: 004b3ce0.cpp (source), 004b3ce0_test.cpp (test), payload.json / oracle.tsv (addmatch.py), hdr/ (overlay, see hdr/README.md).
COORDINATOR: row 0x004B4260 ActivateMultipleQuests ($S/w22/wf1/004b4260) carries an older, non-matching copy of this body. Regrade it with
  THIS body (and this overlay: hdr/engine/CQuestManager.h + CScriptInfo.h are supersets of wf1's; it also needs hdr/stlport/fable_map.h and the
  same-TU LoadResourcesForQuests / CBasicString<char>::operator< bodies and defines of 004b3ce0.cpp) before landing it.
What made it match (in the order found; start state = wf1's body on the shared headers, 1168 v 1285):
1. vector push_back inline (FABLE_RETAIL_STLP_VECTOR, no _PUSHBACK_NOINLINE) with STLport's _M_insert_overflow body VISIBLE (not _DECL_ONLY).
2. New fable_map.h opt-in FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: set::erase(key) = equal_range (out-of-line _M_lower_bound / _M_upper_bound)
   + distance + erase(first,last). Both arms are plain `erase(info.QuestName)`; VC7.1 calls 0x004BB270 in the scripted arm and expands it in
   the bare arm (the expanded distance loop is dead and removed), given the right static frequencies (point 5).
3. Same-TU REAL bodies: CQuestManager::LoadResourcesForQuests 0x004B2510 (the landed row's body re-spelled on the shared headers) and
   CBasicString<char>::operator< 0x00429950 (landed body, noinline). The first (with 1) makes the local array non-escaping: _M_finish / the
   set's end() / the singleton stay in edi, frame 0x34 (was the old "register roles swapped" residue). The second lets end() of the persist
   map be read once.
4. FABLE_RETAIL_CARRAY (operator[] through begin()): `lea esi,[edx+ecx]` operand order.
5. Both loops are `while (i < size) { ..; ++i; }` (second one `if (!IsQuestActive) { .. }`): the inline split of point 2 then needs no
   /d2inlT flag (with `for` loops only /d2inlT62..66 gave it).
6. CScriptInfo's inline PDB accessors (new opt-in FABLE_RETAIL_CSCRIPTINFO_ACCESSORS: GetAllocFunc / GetMasterScriptDataAllocFunc /
   IsMasterScript, both PDBs; allocators __fastcall, interface by reference): gives `mov eax,[ecx] ; call eax` AND tips c2's colouring so the
   constant-zero piece takes ebx from the persist lookup to the end of the bare arm (c2prio: zero piece 62 v the bare id temp 68 without them).
7. `master_data = script_data` before LoadMasterScriptDataFromGameState; AddMessage on the member `World->PMessageEventManager_Data`
   (the inline GetMessageEventManager() is evaluated before GetIndexFromRegisteredQuest: 7 hunks); `long id = AllocateQuestID()` (PDB inline;
   `CurrentQuestID++` matches too).
Not needed after all (tried, same bytes): FABLE_RETAIL_STLP_TREE_MFIND_DECL, a cached-end CVectorMap::Find opt-in, /d2inl flags.
Ideas that did NOT move it: find()==end() operand order, explicit `last` local / if-return spellings of Find, result variable initialised
  to 0 in GetQuestScriptPersistScript (worse), id read + separate increment, one id declared before the if, named new'd pointer,
  if(Data) counted-pointer ctor, init-list spellings of the 3-argument CActiveQuest ctor, bare-first `if (!a || !b)` order (same code),
  long / signed loop index, local function-pointer variable (call eax but eax/ecx swapped).
Probes (never kept): stand-in LoadResourcesForQuests (escape question), lookup removed (zero-register question), volatile bare id
  (diagnostic: everything else falls in place), c2prio x4 (ATTEMPTS.log).
Local declarations (no shared header exists): CCodeSectionManager (derives CTBaseSingleton<T>; ctor 0x009F0320, IsSectionRegisteredLongName
  0x009F0570, GetHandleFromLongName 0x009F0500, LoadSection 0x009F02C0, inline LoadSectionLongName), CTCDayOrNightOnlySupport's two statics.
Made-up (descriptive) names, found in no PDB / landed source: g_QuestScriptsEnabled (bool 0x01375454, one reader 0x004B3EED); locals /
  parameter names (quests, quests_to_load_resources_for, info, persist_script, id, script_data, master_data, script, quest).
Name findings (NAME_FIXES.tsv): landed 0x009F0570 `CNameRegistry_009f0570::Contains` is CCodeSectionManager::IsSectionRegisteredLongName;
  engine/CScriptInfo.h spells both allocator pointers __cdecl with a pointer interface parameter (PDB: __fastcall, `CGameScriptInterfaceBase&`);
  landed 0x004BAEF0 `OnDie_3` is CCountedPointer<CQuestManager::CActiveQuest>::DeleteData (stored in the count block at 0x004B3F94 / 0x004B40CD).

## 0x00A90040


## 0x00A90040 N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump -- RELOCATION_MATCH (7 fg.py calls, 12 variant grades; strict-headers regrade passes; test PASS)
- What made it match: the landed sibling NonBump 0x00A8FF50 idiom (named `index` local, the mesh vertex read as `mesh.Vertices[index]`, by-value getters into the CGenericVertex setters, bone loop + pad)
  plus the StaticBump tangent tail with BOTH tangent references on UTangents (retail never reads VTangents), and -- the last 4 bytes -- the C3DVector::Cross
  body with all three components parenthesised (existing opt-in FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION). The default Cross left DIFFER 399v403:
  retail `fld st(1)` (copy of normal.Y) at 0x00A9017A + `fxch st(1) ; fstp st(0)` at 0x00A90180, ours `fxch st(1)` consuming it. Same instructions otherwise;
  the PAREN tuples move VC7.1's scheduling-region cut (as the operator^ FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP note in C3DVector.h says).
- Dot: both the COLLISION body X*oX+(Y*oY+Z*oZ) and FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX match with the grouped Cross (e1 / d2); the row uses COLLISION alone.
- Did not change the residue (default Cross): named `const C3DVector normal` local, direct chain Cross().Dot(), const crossprod + float dot local, hand-written
  cross with the Z component commuted / negated / member-assigned, default Dot, FABLE_RETAIL_C3DVECTOR. Y-only and X-only grouped Cross: not graded (fg stopped at the first match).
- Headers / flags: no flags, no pragmas. Defines: FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION, FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS, and the two overlay opt-ins
  FABLE_RETAIL_CANIMATEDBLOCK_DATA (engine/C3DPrimitive2.h) and FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS (engine/CGenericVertex.h) -- see hdr/README.md.
  Suggestion for the coordinator: a neutrally named alias for the member Cross XYZ-grouped body (the COLLISION macro also switches Dot and, with
  NONZERO_SQUARED_XYZ, IsNonZero) -- not added here to avoid a C3DVector.h overlay.
- Local models in the row (no shared header exists): CTVertexNormBlend4Tex1BumpBase / CTVertexNormBlend4Tex1Bump (Ego_r 56 bytes) and the mesh's
  N3DPrimitive::CDeviceIndependent (members as the landed ctor 0x00A98100; TriangleLists / Groups / BoundingSphere opaque bytes). No made-up names: the earlier helper `CDeviceIndependent::PeekVertex` (copied from the landed siblings 0x00A8EFA0 / 0x00A8FF50) is NOT in Ego_r.pdb or FableWin.pdb -- N3DPrimitive::CDeviceIndependent there has only GetVertexCount / SetVertex / ResizeVertexList; the only PeekVertex is C3DMeshFileVerticesChunk::PeekVertex -- so the row now indexes `mesh.Vertices[index]` directly (same bytes).
- StaticBump 0x00A8F1B0 (parked 1469v1469 scheduler tie) is the same TU and the same Cross: try the XYZ-grouped Cross there too.
- Name fix (see NAME_FIXES.tsv): landed caller 0x00A91180 declares 0x00A90040 as CAnimatedBlock::TransformVerticesWithNormals(CAnimatedVertexWithNormals*, const CDeviceIndependent*);
  PDB (Ego_r 0x00BD1EC0, FableWin 0x017CE7D7) = N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump(CTVertexNormBlend4Tex1Bump*, N3DPrimitive::CDeviceIndependent&).

### Repair (verifier findings, 2026-11-12) -- still RELOCATION_MATCH (normal and LANDVERIFY_STRICT_HEADERS=1), test PASS
- Test: vertex 0 now uses N=(0.2,0.9,0.3), U=(1.3,0.1,1.3): the float-rounded Cross(N,U) dots with U to ~-1.4e-7, so the
  VTangentSign = -1 branch (0x00A901AE) is exercised and checked; VTangents is set to -Cross for the +1 vertices and +Cross for the
  -1 vertex, so reading VTangents would flip every sign. Both verifier mutants (VTangents read: $W/repair/m1.cpp; always +1:
  $W/repair/m3.cpp) now FAIL under vt.py.
- Source + test: dropped the non-PDB `CDeviceIndependent::PeekVertex` helper; `mesh.Vertices[index]` grades RELOCATION_MATCH.
- Coordinator: the landed siblings 0x00A8EFA0 / 0x00A8FF50 declare the same non-PDB `PeekVertex` on the mesh CDeviceIndependent;
  it should be removed (or marked descriptive) there too.

## 0x0064546F

## 0x0064546F NPlayerGui::CDrawQuickAccessIcons::DrawEffectAtPos -- RELOCATION_MATCH (21 grades, first authoring attempt)
- Final source $W/0064546f.cpp, test $W/0064546f_test.cpp (vt.py PASS), strict-header regrade RELOCATION_MATCH, in payload.json / oracle.tsv.
- What made it match: (1) port onto the shared headers with real STLport `std::vector` members Effects +0x7C / EffectPositions +0x88
  (fixes the this/index register swap of the raw-pointer draft); (2) the test spelled `pos.ToC2DCoordF().IsNonZero()` with
  GetSquaredMagnitude Y-first (FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX) and IsNonZero / C2DCoordI::operator!= force-inlined
  (under optimize("s") plain inline copies are CALLED); (3) create position = a named local direct-initialised through a named
  reference to the offset: `const C2DVector& offset = PeekGuiDef().QuickAccessEffectOffset; C2DVector position(pos.ToC2DCoordF() + offset);`
  (`C2DVector position = a + def.Offset` spills the float temp to memory and moves the slots: 412v406); (4) raw-pointer assignment
  through the implicit CIntelligentPointer(T*) temporary in all three places (`Effects[index] = 0;` and
  `Effects[index] = manager.Create2DEmitter(...)`): a NAMED emitter local dies after `EffectPositions[index] = pos` (dtor 2 instructions
  late), a braces block around it also matches; (5) zero arm `EffectPositions[index] = C2DCoordI(0, 0)`.
- Flags: none. Pragmas: optimize("y", off) + optimize("s", on).
- Headers / opt-ins used: FABLE_RETAIL_BASE_OBJECT, C3DVECTOR, C2DVECTOR (+_NORMALISE, _ISNONZERO_FORCEINLINE, _SQUAREDMAGNITUDE_YX),
  C2DCOORDI_TOFLOAT, CRIGHTHANDEDSET(+_CTOR), CINTELLIGENTPOINTER(+_CTOR_OUTOFLINE, _ASSIGN_OUTOFLINE), CPARTICLEEMITTER(+_KILL),
  CPARTICLEEMITTERMANAGER_CREATE2D, CPLAYERGUI(+_PEEKGUIDEF), engine/NUISystem_CManager.h (real EEngine2DLayer enum).
  NEW overlay opt-ins ($W/hdr, see hdr/README.md): FABLE_RETAIL_C2DCOORDI_NOTEQUAL (engine/C2DCoordI.h, FABLE_FORCEINLINE operator!=,
  adds `#include "fable_compiler.h"`), FABLE_RETAIL_CPLAYERGUIDEF_QUICKACCESSEFFECTOFFSET (engine/CPlayerGuiDef.h, C2DVector at +0x6F0).
- Local in the row (no shared header exists): class NPlayerGui::CDrawQuickAccessIcons (pad to 0x7C + the two PDB-named vectors);
  declaration of NParticleEngine::GetParticleEmitterManager() (FableWin name, same as the landed sibling 0x006451DD).
- Descriptive (PDB has no parameter names): `pos`, `effect`, `index`; locals `offset`, `position`.
- Name findings: 0x43 is `ENGINE_2D_LAYER_0` (= 67) in the PDB enum (engine/NUISystem_CManager.h / CEnginePrimitive2DSprite.h); the landed
  sibling 0x006451DD spells it with a made-up enumerator `ENGINE_2D_LAYER_HUD_EFFECTS` in a local enum (NAME_FIXES.tsv). Scout's finding
  kept: landed 0x00645605 "unattributed_GetTarget00645605" is ?ClearEffects@CDrawQuickAccessIcons@NPlayerGui@@QAEXXZ (NAME_FIXES.tsv).
- Old parked draft in differ/0064546f.cpp is superseded by the matched source.
- Repair (2026-11-12): hdr/engine/CPlayerGuiDef.h rebased on current main (keeps FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH for landed 0x00646580; diff vs main
  is only the QUICKACCESSEFFECTOFFSET include/member/assert lines); hdr/engine/C2DCoordI.h `#include "fable_compiler.h"` moved inside the NOTEQUAL opt-in;
  NAME_FIXES.tsv adds 0x00AEAAF0 GetGlobal -> NParticleEngine::GetParticleEmitterManager. Regrade RELOCATION_MATCH (also strict headers), vt.py PASS.
  Old overlays kept in hdr_old/ for reference.

## 0x008565E0


## 0x008565E0 CThingFilter_CombatIsStrikeToStrike::operator() -- RELOCATION_MATCH (wf4 author, 23 grades incl. 3 compile failures)
- What made it match: the port onto the shared headers (CThing / CDefPointer / CThingBaseDef / CThingCreatureDef / CTCPhysicsBase /
  C3DVector / NCreatureMode / NTargeting) with the PDB identities (anonymous namespace, `const CThing&`, member ThingStriking); a null
  `CDefPointer<const CThingCreatureDef>` local filled by `SetDynamicCast(thing.GetPBaseDef())` (the converting ctor is 364v406); the
  two mode tests joined by `||` (one shared `return false` tail); the thing's position as a named reference before the striker's
  GetPos; the dot written out as `facing.X * delta.X + delta.Y * facing.Y + delta.Z * facing.Z` (retail's X product loads delta.X
  first; every shared C3DVector::Dot form -- default, ZYX, GROUPED, shot-collision -- with either receiver loads facing.X or breaks
  Y/Z); the non-creature arm as `if (thing.GetPBaseDef()->AlwaysIncludeInObjectStrikeScans) return true; return false;`
  (`test bl,bl ; setne al`; a plain return / `!= 0` / `? true : false` gives `mov al,bl`, `== true` gives and/cmp/sete).
- Headers / flags: no flags, no pragmas, no header overlay (hdr/ is empty). Defines: FABLE_RETAIL_CCLASS, DEF_CLASS_BASE, BASE_OBJECT,
  CTCBASE, CINTELLIGENTPOINTER, STLP_LIST, CTHING, CTHINGBASEDEF, CTHINGCREATUREDEF_MEMBERS, CTCPHYSICSBASE, NCREATUREMODE_QUERIES,
  NTARGETING_MELEE. (Repair: the unused C3DVECTOR_DOT_ORDERED_ZYX define was dropped; still RELOCATION_MATCH, strict headers too.)
- `NCreatureMode::IsRolling(const CThing&)` 0x0085EEB0 is declared in the row (engine/NCreatureMode.h has no declaration of it yet;
  PDB `?IsRolling@NCreatureMode@@YI_NABVCThing@@@Z`, landed row NCreatureMode_IsRolling_0085eeb0.cpp).
- Names: no landed name proven wrong. The parked draft's Striker / GetPDef / GetPosition / GetFacing / Destroy were draft-only.
- Made-up names: none (locals `def`, `pos`, `delta`, `facing` only).
- Possible follow-up (not needed for the match): a C3DVector::Dot opt-in whose X product is `other.X*X` would let the row call Dot.

## 0x00AC8AE0


## 0x00AC8AE0 QuadricOptimiserInternals::COptimisedPrimitive::JoinAdjacentFaces -- RELOCATION_MATCH (1 grade + 1 strict regrade)
- What made it match: the port onto the shared headers alone. The old draft's local CArray declared push_back without a body; with
  the shared CArray + FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE + FABLE_RETAIL_STLP_VECTOR (body visible, out of line as retail's
  0x00AD5CE0) VC7.1 produces retail's frame (1Ch, &b and b.U[1] spilled, ebp pushed late). Function body text unchanged from the draft
  (field-by-field edge fill, default CArray operator[] -- FABLE_RETAIL_CARRAY not needed, v2/v3 variants were not reached).
- Headers / flags: no flags. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_JOINADJACENTFACES,
  FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_HASUNIQUEVERTEX, FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE; existing opt-ins used:
  FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE, FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_QUADRIC_COPTIMISEDFACE,
  FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE.
- Names: all PDB (JoinAdjacentFaces, HasUniqueVertex, COptimisedEdge members, CArray::push_back); no made-up names.
- Name fix for the manifest: 0x00AD5CE0 is `CArray<QuadricOptimiserInternals::COptimisedEdge>::push_back` (Ego_r 0x00C6AB60, FableWin
  0x034D16C0 ?push_back@?$CArray@VCOptimisedEdge@QuadricOptimiserInternals@@@@QAEXABV...), not `std::vector<COptimisedEdge>::push_back`;
  the row's module is missing the namespace `QuadricOptimiserInternals::`.
- Module passed to addmatch: `QuadricOptimiserInternals::COptimisedPrimitive` (as the landed siblings' file names spell it).

## 0x008F10B0

## 0x008F10B0 CTCTavernGameCardBase::ShuffleCards -- RELOCATION_MATCH (4 grades + 1 final + 1 strict; 1 probe)
- What made it match: (1) the port onto the shared headers; (2) the REAL bodies of GetCardDef 0x008F0B20 and GetPackPosition 0x008F0B80
  in the TU ahead of the row with no pragma at all -- VC7.1 itself keeps the GetCardDef call before the loop a call and expands
  GetPackPosition (and the GetCardDef inside it) in the loop; (3) the GetSubDef template body visible but out of line
  (engine/CDefClassBase_GetSubDef.h + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE): frame 0x2C -> 0x20, def slot reused for the height
  counter, inner add-ref/release folded in registers; (4) `pos += GetPackPosition()` (not `pos = pos + ...`: 430v410);
  (5) FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE for the count-store position at 0x008F116B (the last 1-instruction residue).
  The height is the plain `(float)(53 - (int)i)`; the compiler strength-reduces it into the counter in the def's dead slot.
- Probe: the same TU's GetCardDef and GetPackPosition grade RELOCATION_MATCH against their own retail addresses 0x008F0B20 / 0x008F0B80
  (q.py with the overlay), so the two in-TU bodies are the real ones, now on shared headers (the landed rows use local models and
  stand-in callee bodies -- they can be re-landed from this source: same defines, leaf GetCardDef / GetPackPosition).
- Headers / flags: no compiler flags. Overlay $W/hdr (5 opt-ins, see hdr/README.md): CTavernGameCardBaseDef.h (_CARDPACK, _TYPENAME),
  CTCTavernGame.h (_HIDEMESH), card_sorting_types.h (FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET), stlport/fable_containers.h
  (FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG). The row includes "stlport/fable_containers.h" FIRST: fable_game.h includes it relative to
  rebuild/include, which bypasses an overlay copy (irrelevant once the opt-in is in the repo header).
- Names: no made-up names. Landed spellings proved wrong (NAME_FIXES.tsv): 0x005E9B10 is the static
  ?HideMesh@CTCTavernGame@@SIXPAVCThing@@@Z (Ego_r + FableWin), landed as a free `HideMesh(CMeshEntity*)`; the landed random_shuffle
  row 0x007D7860 comment calls this+0x134 "Cards" -- it is Pack (this row reads it through the shared header's Pack member and matches).

## 0x00ADDBE0


## 0x00ADDBE0 C3DBoundingSphere::BuildFromSubSpheres -- RELOCATION_MATCH (wf4 author, 2026-11-12)
Source: C3DBoundingSphere_BuildFromSubSpheres_00addbe0.cpp; test: test_00addbe0.cpp (vt.py PASS); strict-headers regrade RELOCATION_MATCH.
Headers / flags: hdr/engine/C3DBoundingSphere.h opt-in FABLE_RETAIL_C3DBOUNDINGSPHERE_BUILDFROMSUBSPHERES (declaration only); no flags, no pragmas.
What made it match (in the order found):
1. Six scalar float bounds (minX..maxZ) and scalar centre (cx, cy, cz) stored member by member (`Center.X = cx` ...), not C3DVector
   locals: frame 0x14 with maxZ in st0 and the centre in the dead argument slots. C3DVector locals gave frame 0x20 / 0x24 and a
   `mov eax,ecx` second pointer for `Center = centre`.
2. Plain `for (i = 0; i < count; ++i)` over `spheres[i]` with each bound expression spelled twice (test and store). That body stays
   rolled. A `float v` temporary, a `const C3DBoundingSphere&` / walking pointer, or a `float r` local all make VC7.1 unroll by 4
   (1088..1104 bytes). The guarded do/while of the retry2 draft is not needed.
3. `spheres[i].GetRadius()` (the header's inline accessor) instead of `spheres[i].Radius` in loop 1: flips the upper bounds from
   `fld Radius ; fadd Center.c` to retail's `fld Center.c ; fadd Radius`. Writing `Radius + Center.c` does NOT flip it (VC7.1
   orders the two loads itself); neither does the compare spelling or the declaration order of the locals.
4. Distance spelled `dy*dy + (dx*dx + dz*dz)` on scalar dx/dy/dz: retail's Z, X, add, Y, add. Every left-leaning three-term sum
   (xx+yy+zz, xx+zz+yy, zz+xx+yy, and the header's GetMagnitude on a temporary vector) came out Z, Y, add, X, add here.
Ideas that did not work (do not repeat): vector min/max locals with header operators (430 bytes); `Center = C3DVector(...)` and
reading `Center` in loop 2 (loop 2 then reads [ecx]); `v > max` compare (403 bytes); `C - -R` (fchs); pointer do/while loop 1
(right fadd order only by accident, `mov edi,esi` scheduled before `lea edx`).
Names: all from the PDBs (BuildFromSubSpheres, GetRadius, Center, Radius); locals are descriptive. No landed name proved wrong.
Loop 2 reads `spheres[i].Radius` directly (not GetRadius()); only loop 1 uses the accessor in the matching spelling.
Note: differ/00addbe0.cpp is the OLD parked draft (superseded by the matched source in payload.json); a1..m5 .cpp/.out and sig.sh are scratch variants.

## 0x0065A41C


## 0x0065A41C CTCVillage::GetFineAmountForCurrentCrimes -- RELOCATION_MATCH (wf4 author, 5 grade calls / 8 variants)
- What made it match: ported onto the shared headers with the include block of the landed neighbour 0x0065C5BB (UpdateMirroredCrimeData),
  default CDefPointer release (NOT `FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF`: 421v415), and the map number and region number as NAMED
  `long` locals before `GetWorldMap().GetRegion(region).GetPRegionDef()`. As one nested expression (or with a named `CRegion*`) the
  ported form keeps the fine in ebx and spills `this` (411v415); the old local-model twin spelling matched nested, the header form does not.
- Needs: `// Flags: /GS`; `#pragma optimize("y", off)` + `optimize("s", on)`; overlay `hdr/engine/CTCVillage.h` opt-in
  `FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT` (declaration of the method only) plus the existing `FABLE_RETAIL_CREGION_PREGIONDEF`.
- Local model left: `CRegionDef` (no engine/CRegionDef.h exists): CDefPointeeBase base, `bool UseCrimes` at retail +0x9D (landed
  CRegionDef::Transfer 0x004715AB; Ego_r +0xA9). Made-up names: none (locals only: `map`, `region`, `def`, `fine`, `seen`, `effects`).
- Landed spellings proved wrong (callers' local models, not relinked here): sibling 0x0065A28F spells CWorldMap slot 16 `GetMapAtPos`
  (PDB `?GetMapNumberAt@CWorldMap@@UBEJABVC3DVector@@@Z`) and CRegionDef +0x9D `HasFines` (PDB member `UseCrimes`). See DOSSIER section 3/4.
- Ideas tried: v1 twin local models (matched, not landable under the shared-header rule); p1 COUNTREF release 421v415; p2 default release
  nested 411v415; p3 POINTEE release 411v415; p4 named `CRegion*` 411v415; p5 named longs MATCH; p6 not graded (fg stops at first match).

## 0x0065AA6F


## 0x0065AA6F CTCVillage::IsTimeToGoToTavern -- RELOCATION_MATCH (author pass 1)
Source: 0065aa6f.cpp, test: 0065aa6f_test.cpp (vt.py PASS), strict-header regrade RELOCATION_MATCH, in payload.json / oracle.tsv.
What made it match (in order):
1. Dossier fixes: IsTavernTime first, `speed == -1.0f`, `distance * 1.6f`, slot 79/81 GetMaxWalking/RunningSpeed, PhysicsTC->GetFacingVector
   (slot 70), GetPVillage, CWorld/TimeStep.
2. `CThing& parent = GetParentThing();` held in a local (retail loads this->ParentThing once into edi for the facing call and GetPos),
   and the sum written `parent.GetPos() + facing` (facing is called first: member operator+ evaluates its argument first).
3. FRAME (the real blocker, frame 0x30 v 0x3C): the school twin's layout (start/end/target all in one inner block) overlays target on
   position. Retail keeps target in its own slot => target is a function-scope local; only start/end sit in an inner block and the
   scaled start leaves it in a plain float (`float startFrames; { float start; float end; ...; startFrames = start / daySpeed; }`),
   which VC7.1 spills back into the dead `creature` slot [ebp+8]. No-block form: start gets its own slot (frame 0x40).
4. Shared-header port: one residue pair (`fld [eax] ; fadd [esi]` v retail `fld [esi] ; fadd [eax]`, X of the target sum) appears
   with the one-argument header GetTC<CTCVillageMember>(&member) (both GETTC_EXPANDED and an if-form copy of the school body) and
   goes away with the two-argument `GetTC(TCI_VILLAGE_MEMBER, &member)` (one inline level fewer). The operand order of that add is
   a tie that follows the number of inline expansions before it, not the GetTC body form.
Headers / flags: `#pragma optimize("s", on)` in the source, no extra flags. Overlay (hdr/README.md): FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES
(engine/CTCVillage.h), FABLE_RETAIL_CGAMETIMEMANAGER_DAYSPEED (engine/CGameTimeManager.h). Existing opt-ins used: the retail CThing /
CThingAICreature chain with FABLE_RETAIL_CTHING_ORIGINAL_VTABLE, FABLE_RETAIL_CTHING_GETTC_EXPANDED, ..._AICREATURE_WORKBUILDING
(inline GetWorkBuilding body in the row file, as the landed rows do), FABLE_RETAIL_IS_THING_ALIVE(+_NOINLINE), FABLE_RETAIL_CTCPHYSICSBASE.
Open points for the coordinator (not blockers for the bytes):
- GetTC spelling: FableWin.pdb has the one-argument ??$GetTC@VCTCVillageMember@@@CThing@@QAE_NPAPAVCTCVillageMember@@@Z; the row uses
  the two-argument member template (PDB-proven only for T = CTCBase) because the one-argument form leaves the operand-order pair above.
- GetDaySpeed <-> TimeStep (+0x10) is inferred: FableWin.pdb names the inline getter, retail ConsoleSetDaySpeed 0x006BB940 writes +0x10
  (AlterDaySpeed expanded in place); there is no out-of-line GetDaySpeed body to read.
- Made-up names: locals only (position, work, distance, member, village, start, end, startFrames, parent, target). No made-up callee names.
- Landed spellings proven wrong: 0x0065D624 is landed as the made-up `_global::IsThingAlive_ptr_0065d624` (leaf
  `@FableIsAlive_65d624@4`); Ego_r.pdb 0x0069F330 and FableWin.pdb 0x017A3A14 / 0x01B5E870 name it
  `??$IS_THING_ALIVE@PAVCThingBuilding@@@@YI_NABQAVCThingBuilding@@@Z` -> NAME_FIXES.tsv. (The old parked draft's IsCookTime /
  GetVillage / GetWalkSpeedTo names were never landed.)
- Repair pass: comment at the inline GetWorkBuilding definition names its source row (CAIStateGroupMinionWorkInInn::GetFreeBench
  0x0090BC60), why the body is in the includer (header only declares it; needs CThingBuilding complete) and the retail evidence
  (`lea esi,[ebx+170h] ; call 0x00A01B50` 0x0065AA8D/0x0065AA95); callee list updated. Regrade RELOCATION_MATCH (also strict).
Ideas tried that did NOT work (do not repeat): target inside the start/end block (any spelling: named facing, const-ref target,
copy-then-add, inline CThing helper returning the sum, positive `if` nesting, distance in its own block); target declared early and
assigned (sum temporary + movsd copy, 424 bytes); implicit C3DVector copy ctor (movsd copy); alive-test temporary narrowed by a block
or an inline helper with everything else at function scope (frame 0x40); header GetTC default / ORIGINAL_CHAIN forms (400 bytes).

## 0x00BBCC70


## 0x00BBCC70 CEnginePrimitiveManagerParticleGroup::SetupPixelShader -- RELOCATION_MATCH (2 grades: first port + strict regrade)
- What made it match: the port onto the shared headers, nothing else. The parked draft's flat free `SetState(state, value, core)` and
  invented texture holder were model errors; the real spelling is `GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP)` /
  `SetTextureUVModeV(1, TEXTURE_UV_WRAP)` (two-level member inlines -> block 2's `mov ecx,eax` + scoped ebp temp) and
  `GPRenderManager->AttachTextureToStage(1, &GPResourceManager->GetStipleFadeTexture())` (the `add ecx,2AACh ; je` null test comes from
  AttachTextureToStage's `texture ? texture->PD3DTexture : 0`).
- Headers / flags: no flags, no pragma. Defines FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS, FABLE_RETAIL_CENGINERESOURCEMANAGER and
  the NEW overlay opt-in FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE (hdr/engine/CEngineResourceManager.h, see hdr/README.md).
  Passes LANDVERIFY_STRICT_HEADERS=1. Local model only for CEnginePrimitiveManagerParticleGroup (no shared header; same packed model
  as landed sibling 0x00BBC460).
- Names: all PDB. SetupPixelShader: Ego_r.pdb + FableWin.pdb. GetStipleFadeTexture: FableWin.pdb only (inline, so absent from Ego_r's
  publics); second evidence is the Ego_r member StipleFadeTexture at +0x2AAC that the retail bytes address. Parameter names
  `reg`, `stipple_shader`, `normal_shader` are descriptive (the PDBs carry no parameter names; callers pass the group's stipple then
  normal shader).
- Landed name proved wrong: row 0x00989B00's `// Leaf:` says `?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXJMMMM@Z` (long);
  Ego_r.pdb (ego 00ae6350) and FableWin.pdb spell `...QAEXKMMMM@Z` (unsigned long). See NAME_FIXES.tsv. The shared header already has K.
- Files: source $W/00bbcc70.cpp, test $W/00bbcc70_test.cpp (vt.py PASS), payload.json + oracle.tsv written by addmatch.py.

## 0x007B8B10


## 0x007B8B10 CTCTavernTable::GetEatingPoint -- RELOCATION_MATCH (wf4 author, first attempt, 4 grades: 2 SRC_FAIL, 1 match, 1 strict regrade)
- What made it match: the port onto the shared headers by itself. The old ebp/ebx web swap + missing `mov edi,edi` pad came from the
  draft's hand-written GetTC; `ParentThing->GetTC(TCI_NAMED_USER_ACTION_POINTS, &points)` (FABLE_RETAIL_CTHING_GETTC_EXPANDED) with
  FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND, uninitialised `points`, explicit `start, true` arguments, range test spelled on
  `chair - 1` twice, `Food[chair - 1].GetPItem()` (CBaseIntelligentPointer, CBaseObject*). No flags, no pragmas (no /d2inlT62).
- Headers / opt-ins (overlay $W/hdr, see hdr/README.md): engine/CTCTavernTable.h FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT (new, nested in
  _SEATS), engine/CTCNamedUserActionPoints.h FABLE_RETAIL_CTCUSERACTIONPOINTBASE_SETFREE + FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_GETNAME
  (new); existing: FABLE_RETAIL_CCHARSTRING(+_SUBSTRING), CTCBASE, CTHING, CTHING_GETTC_EXPANDED, CVECTORMAP_INLINE_LOWERBOUND,
  CINTELLIGENTPOINTER(+_ASSIGN), CTCNAMEDUSERACTIONPOINTS, CTCTAVERNTABLE_SEATS. Strict-headers regrade: RELOCATION_MATCH.
- Local declaration only: `long __fastcall GFCharStringToInt(const CCharString&)` 0x0099E7F0 (no shared header declares it).
- SeatsCurrentlyUsed is opaque bytes in the new opt-in (retail vector<bool> is 12 bytes, one byte per seat per the landed
  0x007B8530 / 0x007B8AA0 models; not needed by this row).
- Names: all PDB spellings, none made up. Parameter names follow the header sibling GetServingPoint (PDB has no parameter names).
- Landed spellings proven wrong (NAME_FIXES.tsv): 0x007FEFC0 GetName landed as (long, CCharString*), PDB is (long, CCharString&);
  0x007B9090 landed leaf `FreePoint`, PDB / engine/CTCTavernTable.h say FreeServingPoint.
- v2 (a `long seat = chair - 1` local) was not graded: fg.py stopped at v1's match.

