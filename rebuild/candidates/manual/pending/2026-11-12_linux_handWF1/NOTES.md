## 0x00A114B0


## 0x00A114B0 CPhysicsMeshObject::SetPositionMatrix -- wf1 author pass 1 (2026-11-12): DIFFER(338v338), 6 hunks, ONE cause
Grades: 83, probes: 14 (ATTEMPTS.log). Best source: differ/00a114b0.cpp (all shared headers, real PDB helper names, no local models).

### Exact residue
Same 107 instructions, same frame, same operand order everywhere. One scheduling decision differs: the store of the cross
product's Z (`fstp [esp+14h]`). Ours: 3 integer stores after the last `fsubp` (0x00A11509). Retail: after all twelve identity
stores and `mov eax,[esp+34h] ; mov edx,[eax+4]`, immediately before `fld [eax]` (position.X) at 0x00A1152A. Because of that
`fld [eax]`, `fadd [ecx+2Ch]`, `fstp [ecx+2Ch]` each sit one integer instruction later in retail. The twin 0x00A357B0 has the
identical retail schedule, so it is a property of the source, not of this TU.

### What is proven (each by a grade)
- Port to shared headers alone took the parked 319v338 draft to 338v338.
- up / forward: `C3DVector up = axes.GetUp(); C3DVector forward = axes.GetForward();` before the cross (declaration order = frame order).
- Cross: `const C3DVector& right = axes.GetForward() ^ axes.GetUp();` gives retail's commutative operand order in all six products.
  A named `C3DVector right = axes.GetRight()` flips the Z term's first product; `const C3DVector& right = axes.GetRight()` is right
  only while no other temporary follows (with the PostTranslate copy it flips Y/Z terms again). The order of these `fld a ; fmul b`
  pairs is NOT a spelling of operator^ (three spellings graded, no change): it follows the temporaries around it.
- Identity: FableWin ?SetIdentity@CMatrix3x4@@QAEXXZ as chained zeros then ones (new opt-in).
- Translation: `PositionMatrix.PostTranslate(C3DVector(position));` with the EXISTING header PostTranslate gives retail's
  `fld pos.X ; fadd E41`, the Y/Z words copied to the frame, and keeps the E33 integer store behind the E43 store (0x00A11559).
  Written directly in the row (`PositionMatrix.E41 = p.X + PositionMatrix.E41`) the E33 store is hoisted into the translation.
  By-value float parameters (PostTranslate(x,y,z)) shrink the frame to 24h: the copy is a C3DVector.
- Rows: direct `E11 = (-right.X)` ... in row order; the three parentheses on the Right row are no-code PAREN tuples that move the
  scheduler's region cut (the `mov eax,[esp+3Ch]` for Scale lands at 0x00A1159F only with exactly 3; 0/2/4/6/9 graded).
- Scale: header PreScale(float); Scale / OO_Scale as written.

### Ideas tried for the last residue (none moved the right.Z store; do not repeat)
named right / const-ref right / const local / direct-init; right copied from the temp after SetIdentity or after the position copy
(always coalesced, store stays at the cross); cross as three scalar floats with `C3DVector right(rx,ry,rz)` after SetIdentity
(the scalar is stored at its definition instead); by-value C3DVector parameters of a forceinline builder (probe: axes by value,
position by value / by reference, identity inside or outside); rows through unary-minus temporaries (`const C3DVector& n = -right`,
header Set(4 vectors), header Set(12 floats), a 9-float probe helper) -- larger code or no change; translation through a float* /
C3DVector& row view, Data[3][k], `row += p`, X read straight from position, memberwise `p = position`; early PAREN tuples
(CROSS_XY_GROUP, parenthesised copy sources); row order (E33 first, up row first: 28+ hunks); a block boundary before the
translation (do/while(0), label, for(;;){break;}); flags /G5 /G7 /Oa (same 6 hunks), /Op /Os (worse).

### Working model of the residue (for the next agent)
The scheduler keeps x87 tuples in IR order and pulls up to 3 ready integer tuples behind a 3-cycle x87 op. Ours = "`fsubp`, 3
ints, `fstp`": the store tuple directly follows the cross. Retail = the store tuple sits AFTER the identity stores (or is a
spill created when position.X is pushed onto a 7-deep stack). So in retail the cross Z is still an x87-stack value across
SetIdentity. Every source shape tried makes VC7.1 put right.Z in memory at its definition (X and Y are enregistered, Z is not).

### Best next ideas
1. Find what keeps the cross Z on the x87 stack across SetIdentity: a shape where the Z consumer comes first (retail evaluates the
   Right row's arguments Z-first?) without materialising the negated temporaries -- e.g. the real ??GC3DVector body may differ
   from the header's `C3DVector(-X,-Y,-Z)`; check FableWin's debug ??GC3DVector@@QBE?BV0@XZ 0x01797688 if a donor image is at hand.
2. The debug twin FableWin 0x031D1440 (320 bytes) / 0x031F67F0 names every helper this function calls; with the donor exe one
   disassembly settles the call list (SetIdentity / PostTranslate / Set / PreScale / operator-) and the argument forms.
3. Solve the twin 0x00A357B0 together: the same source must match both (it has no Scale stores, so its region cut differs).

### Headers / flags
hdr/engine/CMatrix3x4.h (FABLE_RETAIL_CMATRIX3X4_SETIDENTITY), hdr/engine/CPhysicsMeshObject.h
(FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX). Flags: none. No landed name proved wrong; manifest label is correct.
Made-up names: none (the parameter names position / axes / scale are descriptive, the PDB extracts carry no parameter names).

## 0x00A114B0 -- wf1 author pass 2 (2026-11-12): RELOCATION_MATCH (also with LANDVERIFY_STRICT_HEADERS=1)
Grades this pass: 9 (+1 probe). Matched source: matched/00a114b0.cpp (= differ/00a114b0.cpp now), test matched/00a114b0_test.cpp (vt.py PASS).
- What made it match: the residue was the FIRST scheduling-region cut (81-tuple cap), not the IR. New header opt-in
  `FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP` = operator^ with each of the three components parenthesised (three no-code PAREN tuples
  inside the cross product, i.e. before the first cut) and NO parentheses on the Right row (pass 1's three row parentheses were
  the same three tuples in the wrong region: they fixed the second cut only). One spelling now puts both cuts where retail has them.
- Why the Z store sat late in retail: the Z member of a C3DVector local is never kept on the x87 stack (probe A1: with the up copy
  removed the freed slots go to the position copy's X / Y, not to any Z), so right.Z is always a frame store. That store has no
  consumer inside its own region when the cut falls between the position.Y load (0x00A11527) and the position.X load (0x00A1152E);
  the scheduler then emits it last in the region (0x00A1152A). With the cut elsewhere its reload is in the same region and it is
  emitted three integer instructions after the `fsubp`.
- Sweep graded: 1 or 2 cross parentheses + 2 or 1 row parentheses = the old 6 hunks; 3 + 0 = match. Parentheses around a class-type
  expression (`(axes.GetUp())`) add no tuple (8 hunks = only the second cut moved).
- Headers / flags: hdr/engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP, new), hdr/engine/CMatrix3x4.h
  (FABLE_RETAIL_CMATRIX3X4_SETIDENTITY), hdr/engine/CPhysicsMeshObject.h (FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX).
  Flags: none. No landed name proved wrong. Made-up names: none (parameter names position / axes / scale are descriptive).
- For the twin C3DMeshObject::SetPositionMatrix 0x00A357B0: same source shape with the same opt-in should carry over (same retail
  schedule up to the rows; not graded here).

## 0x007FF190


## 0x007FF190 CTCNamedUserActionPoints::GetFreeNamedActionPoint -- RELOCATION_MATCH (5 grades, first authoring attempt)
- What made it match: (1) the dossier's model fixes (CThing& thing, find's end pointer as a named local, noinline-but-visible
  push_back, visible out-of-line std::__find body); (2) the STLport erase spelled as the real chain
  `if (position + 1 != end()) __copy_ptrs(...)` -> `__copy_trivial` (the null-range ternary layer). The hand-written
  `next = position + 1; if (_M_finish != next) memmove(...)` erase of the sibling 0x007FEDE0 left two residues that looked like
  register ties (`mov ebp,[start] ; cmp ebp,[esi+24h]` loop entry without the eax copy + missing loop-head alignment, and
  `lea edx,[eax+4]` before `mov ecx,edi`); both vanished with the real erase. Grades: v1 DIFFER(333v343), v2 DIFFER(346v343)
  (pos+1 != end() with a bare memmove), v3 DIFFER(333v343) (finish in a local), v4 RELOCATION_MATCH (local model + __copy_trivial),
  v5 RELOCATION_MATCH (shared headers), final + strict RELOCATION_MATCH.
- Headers / flags: no compiler flags. Shared headers only: fable_string.h (FABLE_RETAIL_CCHARSTRING), stlport/fable_containers.h
  (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE), stlport/fable_algorithm.h (FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH: inline find +
  declared __find; the row file carries the STLport __find body copied from the landed row 0x007FEC30 as noinline __fastcall), engine/CThing.h (FABLE_RETAIL_CTHING + CCLASS + DEF_CLASS_BASE), engine/CTCBase.h
  (FABLE_RETAIL_CTCBASE), engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER + _ASSIGN), engine/C3DVector.h,
  engine/CRightHandedSet.h, and the OVERLAY hdr/engine/CTCNamedUserActionPoints.h (new opt-in
  FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS: real CTCUserActionPointBase : CTCBase chain + Names; default path unchanged).
- No made-up symbol names (parameter / local names only: position, orientation, name, index, start, checkBlocked, element, last, pos, owner).
- Name fixes proved (same ABI, wrong spellings in landed callers/siblings): callee 0x007FF190 is declared with pointers
  (CThing*, C3DVector*, ..., long*) in CAIStateGroupTavernKeeper_GetOldJugPoint_00968860.cpp -- both PDBs say references
  (QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@VCCharString@@AAJJ_N@Z). (The earlier 0x0050E540 respelling was withdrawn in repair, see below.)
- Lesson: a sibling that matched with a hand-written vector::erase does not prove that spelling; the STLport __copy_trivial
  layer changes register scheduling around the memmove and even the loop-entry copy of the index.

### Repair (verifier findings applied)
- 0x0050E540 NAME_FIXES line withdrawn: its 112 bytes are byte-identical to 0x0040E170 (checked in Fable.exe), whose landed row
  std_find_0040e170.cpp already carries ??$__find@PAJJ@std@@YIPAJPAJ0ABJABUrandom_access_iterator_tag@0@@Z; two unfolded copies mean
  two different instantiations, and no PDB extract (egor_pdb_names.tsv, _fablewin_api.tsv) names either, so which one is long* is
  unproven (one source: the 0x007FEC30 'as bound' note). Left for a coordinator-level resolution; the row only calls it (relocation).
- Source comment now names the row the __find body is copied from (0x007FEC30, itself the 0x006BBFA0 model), not 'its landed body'.
- Regrade after repair: RELOCATION_MATCH, strict headers RELOCATION_MATCH; test PASS.

## 0x00BEAB40


## 0x00BEAB40 CWaterPatchMesh::GetWaterType -- author pass 1 (wf1), DIFFER(345v345), 101 grades

Best source: differ/00beab40.cpp (= v41.cpp). 7 hunks, all inside the per-vertex block 0x00BEAC00..0x00BEAC63.
Everything else matches: frame 0x14, slot order, spill-store order, loops, six calls, x ebx / y esi / this edi / theme0 ebp.

What moved the row from the parked 325v345 to 345v345 / 7 hunks (keep all of these):
- Shared headers: FABLE_RETAIL_CWATERPATCHMESH, _CARRAY, _CENGINELANDSCAPERENDERER (+ new overlay opt-in _PEEKTHEME),
  _CENGINEMAP_PEEK, _CDEFINITIONMANAGER_GETDEFCLASSINDEX; local CEngineLandscapeMap model (no shared header) with PeekEngineMap().
- Leaf is `?GetWaterType@CWaterPatchMesh@@ABE?BW4EWaterType@@XZ` (const EWaterType return); global is g_pEngineDefinitionManager.
- Frame slot order (theme1, theme2 before startY): each theme id variable is REASSIGNED with its index
  (`theme1 = Mgr->GetDefClassIndexFromGlobalIndex(theme1)`). Declaration order of locals does not move slots.
- Spill-store order endX, startY, waterType: declare `startX; endX = startX + 16; startY; waterType` in that order.
- Re-read of T[i].WaterType for the store in layers 1 and 2 (retail 0x00BEAC35 / 0x00BEAC5B): only a by-value inline
  helper holding the conditional (`waterType = CombineWaterTypes(waterType, T.WaterType)`, ternary or if/return, any
  parameter spelling) keeps the second load. Every direct spelling CSEs it (327 bytes): plain if, direct ternary,
  IsUnset()/IsWater() predicates, const-ref read helpers, two-statement accessors, themes pointer/reference locals,
  early-continue chain, whole block as an inline (by value or by reference).

Residue (retail v ours): themes base ecx v edx; layer 0 `mov edx,ebp ; imul edx,edx,38h ; mov ebp,[edx+ecx+30h]` v
`imul ebp,ebp,38h ; mov ebp,[edx+ebp+30h]`; layer 0 store `mov eax,ebp ; mov [wt],eax` v `mov [wt],ebp ; mov eax,ebp`;
retail keeps the running type in eax through layer 2 (`mov [wt],edx ; mov eax,edx`), ours re-reads [wt] into ecx.

Proven about the residue:
- v60.cpp reproduces retail's layer-0 bytes and the eax tracking exactly (modulo registers): layer 0 written as
  `if (T0.WaterType) waterType = CombineWaterTypes(none, T0.WaterType)` with `EWaterType none = WATER_TYPE_NONE` a LOCAL
  variable. So retail's layer 0 is a second (argument) load whose guarding unset-check folded LATE in the optimizer; the
  reload then became `mov eax,ebp` after register allocation. A literal / static const argument folds early and gives the
  v41 bytes again. But v60 permutes the callee-saved registers (theme0 ebx, x ebp and split, theme1 enregistered in ebp):
  24 hunks. Retail has v60's block with v41's allocation.
- The same layer-0 shape (`mov ecx,edi ; imul ecx,ecx,38h ; mov esi,[ecx+edx+30h] ; test ; je ; mov ebp,esi`) and the
  same layer-1/2 re-reads are in CEngineMap::PeekWaterType 0x00B40670 (tail 0x00B40A6A..), where the water type starts
  at 0: there three uniform `if (T.WaterType) wt = Combine(wt, T.WaterType)` calls fold layer 0 naturally. That row
  (pending DIFFER in 2026-10-23_linux_hand316) should be retried with the by-value helper.
- VC7.1 normalises: six v three theme variables (apart from slots), helper parameter spellings, bool predicates, block-level
  inlines, symbol declaration order, a user variable for the tested value. None of these changes the bytes.

Ideas tried (do not repeat): see ATTEMPTS.log v1..v92. Families: F1 direct (327), F2 = v41 best, F3 all three layers
through Combine (346: layer-0 unset check is NOT folded under the outer check, registers permuted), F4 = v60.
Also no effect / worse: /Oa, optimize("t"), C2DCoordI copy, long theme[3] (frame 0x18), reference locals (pointer is
materialised with `add`), iterator-variable accessor, explicit `current` copy (coalesces into a callee-saved register),
ternary layer 0, PickWaterType / CopyWaterType by-value helpers for layer 0 (single load again), bool-valued unset.

Best next idea: find the natural spelling whose layer-0 check folds late WITHOUT a second use of theme0 raising its
rank over x: (1) match CEngineMap::PeekWaterType 0x00B40670's tail first with uniform by-value Combine calls (its water
type is a constant 0 at layer 0), then lift whatever shared inline that row needs into GetWaterType (the two functions
are the same code; a shared header inline taking the three indices is likely, e.g. on CEngineLandscapeRenderer);
(2) on v60, find what keeps theme1 out of ebp (retail spills theme1 and reuses ebp for the three tested values).

Headers: hdr/engine/CEngineLandscapeRenderer.h (FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME, see hdr/README.md). Flags: none.
Made-up names: CombineWaterTypes (helper), WATER_TYPE_NONE / WATER_TYPE_OVERRIDABLE (enumerators 0 / 7), g_pLandscapeRenderer
(0x01436EA8, majority landed spelling), CEngineLandscapeMap::PatchTree.. model fields from Ego_r layout.
No landed name proved wrong (draft-only fixes: leaf `?BW4`, g_pEngineDefinitionManager).

## 0x00BEAB40 CWaterPatchMesh::GetWaterType -- author pass 2 (wf1): RELOCATION_MATCH (final.cpp, 155 grades in total, 4 c2prio probes)

What made it match (two facts, both needed):
1. Layer 0 is `if (T0.WaterType) waterType = CombineWaterTypes(vertexType, T0.WaterType)` with `EWaterType vertexType = WATER_TYPE_NONE`
   a local of the per-vertex block (pass 1's v60): the unset check folds late, the second load survives register allocation
   and is folded to `mov eax,ebp` afterwards. That gives retail's block (index copied to edx, base in ecx, water type in eax).
2. The inner loop end is a NAMED local at function top: `long endY = startY + 16;` declared after startY (order startX, endX,
   startY, endY, waterType = retail's entry spill-store order). It costs no code and no frame slot (VC7.1 still recomputes
   `startY + 16` at the latch), but it is one more candidate referenced in the entry block and removes the hoisted-temp
   preheader block: c2prio showed x at priority 255 against 256 for the three theme-id candidates with `y <= startY + 16`
   (theme0 took ebx, x was split into ebp), and 259 with the named endY (x ebx, theme0 ebp = retail).
How it was found: tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py --blocks on v41 and v60 (probe/prio_*.txt,
probe/blk_*.txt): the only difference was one extra referenced candidate in the layer-0 arm block (K 3 v 2), which costs every
live-through variable 4 and dropped x from 259 to 255; then six loop-bound spellings were probed for x's priority (w1..w6).

Headers / flags: hdr/engine/CEngineLandscapeRenderer.h (FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME); shared opt-ins
FABLE_RETAIL_CWATERPATCHMESH, _CARRAY, _CENGINELANDSCAPERENDERER, _CENGINEMAP_PEEK, _CDEFINITIONMANAGER_GETDEFCLASSINDEX; local
CEngineLandscapeMap model (no shared header; passes LANDVERIFY_STRICT_HEADERS=1). No flags, no pragma.
Made-up names (found in no PDB, FSE or sibling extract): CombineWaterTypes (by-value inline helper), vertexType (the layer-0
constant local), WATER_TYPE_NONE / WATER_TYPE_OVERRIDABLE (EWaterType 0 / 7), g_pLandscapeRenderer (0x01436EA8, majority landed
spelling), startX / endX / startY / endY / theme0..2 locals.
Ideas tried in pass 2 that did not move the row (all graded, see ATTEMPTS.log a1..u3): theme reference local at layer 0 (334),
helper taking the theme index or theme reference, C2DCoordI start copy and implicit bounds (same v60 allocation), 0..16 counter
loops (333, count-down loops), literal NONE with nested / if-return / local-result helper bodies (fold early = v41 bytes),
const-ref `current` bound to a literal (= v60), select / two-return predicate helpers, void out-parameter helper and direct `||`
on the constant (loads CSE again), themes pointer local (327), early continue, six direct no-helper spellings (all 327),
by-value predicate for the outer check, x merged with startX (336: x then outranks `this`), endX from Offset.X (x 269 > this).
Side finding (not landed here): retail's out-of-line CEngineLandscapeRenderer::PeekTheme is 0x00B43CE0 (18 bytes, `mov eax,[esp+4] ;
mov edx,[ecx+4B8h] ; imul eax,eax,38h ; add eax,edx ; ret 4`); the overlay's PeekTheme compiles to the same instructions out of
line (probe/pt.asm). FableWin.pdb has no function taking EWaterType by value apart from the Peek* methods, so the helper name stays
descriptive. No landed name proved wrong.

## 0x00BEAB40 CWaterPatchMesh::GetWaterType -- repair pass (wf1): RELOCATION_MATCH (final2.cpp)

Verifier finding applied: g_pEngineDefinitionManager (0x01436E28) is a made-up name (no PDB data symbol; its only landed use,
rebuild/src/compiled/00/b2/CEngine_Initialise_00b26360.cpp line 108, marks it "descriptive, chunk 727"). It is now listed as
descriptive in the row's header comment and on its extern line (g_pLandscapeRenderer too). Code unchanged.
Made-up names (complete list for this row): CombineWaterTypes (by-value inline helper), vertexType (layer-0 constant local),
WATER_TYPE_NONE / WATER_TYPE_OVERRIDABLE (EWaterType 0 / 7), g_pEngineDefinitionManager (0x01436E28, only landed spelling),
g_pLandscapeRenderer (0x01436EA8, majority landed spelling), startX / endX / startY / endY / theme0..2 locals.
Regrade: final2.cpp RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1; test PASS (vt.py). No flags.

## 0x00BFA920


## 0x00BFA920 CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer -- RELOCATION_MATCH (wf1 author, 2026-11-12)
- What made it match: the port onto the shared headers plus the type's PDB inline accessors (FableWin.pdb Peek*, with
  PeekReceivesShadows / PeekFadeEndBeforeVideoOptionModifier / PeekAlphaMipBias returning float). That alone fixed the old "register tie" (ebp=0, ebx=0xFF,
  edi=flags, mip bias spilled through a frame word). The rest was an inline-budget split: CEnginePrimitiveHandle::operator= and the
  result temporary's destructor are expanded in retail; default threshold calls both (199v346), /d2inlT100..118 expands only the
  destructor (267v346), /d2inlT119 is the smallest value that expands both (bisected). Source carries `// Inline-budget:` on line 2.
- Needs: Flags `/d2inlT119`; defines FABLE_RETAIL_DEF_CLASS_BASE, _CENGINEPERSISTENTPRIMITIVEBASE, _CENGINEPRIMITIVEHANDLE,
  _CENGINEPRIMITIVEHANDLE_CLEAR_DTOR, _CIVCOUNTEDPOINTER, _CIVCOUNTEDPOINTER_ASSIGN, _CENGINEINTERNALPRIMITIVEBASE_ATTACHED,
  _CMOVABLERESOURCE; overlay opt-ins (hdr/README.md): FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR (engine/CEngineFadeDistance.h),
  FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH (engine/CEnginePrimitiveRenderer.h). Strict-headers regrade passes.
- Row-local classes (no engine header exists): CLocalDetailObjectCollectionType, CLocalDetailPrimitiveBase,
  CLocalDetailPrimitiveMesh -- member names from Ego_r.pdb (struct_layouts_egor.tsv), accessor names from FableWin.pdb. No made-up names.
- Names: the old draft's `LocalDetailMeshView` / `g_PrimitiveRenderer` / `CollectionType` were draft-only (not landed); no NAME_FIXES.
- Grades: 17 (ATTEMPTS.log). Probes: 0. differ/00bfa920.cpp now holds the matched source (the parked draft is draft_original.cpp).
- Repair (2026-11-12): the fade pair is read through FableWin.pdb's raw inline ?PeekFadeStartBeforeVideoOptionModifier /
  ?PeekFadeEndBeforeVideoOptionModifier@CLocalDetailObjectCollectionType@@QBEMXZ; PeekFadeStart 0x00BE2740 / PeekFadeEnd 0x00BE2720 are
  out-of-line retail bodies (video-option modifier 0x00B597E0) and are no longer defined in the row. CLocalDetailObjectCollectionType
  now derives from CBaseClassNonCopyable (engine/CBaseClassNonCopyable.h; vtable 0x012A7D94, as landed ctor 0x00BE26C0) instead of the
  PDB placeholder `int _padding_`; size stays 84. Still RELOCATION_MATCH (also strict headers); test updated (polymorphic type,
  modified PeekFade* present but unused). Pre-repair source kept as pre_repair.cpp.

## 0x009E3DF0


## 0x009E3DF0 CPixelFormat::ConvertPFDDataToRGB -- RELOCATION_MATCH (grade 1 of 2; strict-headers regrade also RELOCATION_MATCH)
- What made it match: the port onto the shared headers with the SAME opt-ins as the landed sibling 0x009E3B60 ConvertPFDToRGB
  (FABLE_RETAIL_CPIXELFORMAT + _CONVERT + _PFDTORGB + _POSSHIFT_FORMATREF): GetRPosShift / GetAPosShift as the FableWin debug bodies'
  local-`const CD3DFormatInfo&` field sums are what stop VC7.1 sharing the B/G(/R) loads between mask and right shift (the old
  "CSE residue"). Body = `unsigned long pixel=0; memcpy(&pixel,data,GetByteColourDepth());` then R, G, B through
  GetXMask/GetXPosShift/GetXScaleShift, `if(GetNoABits()) A=... else A=0xff`. Default flags, no pragma.
- Headers: overlay hdr/engine/CPixelFormat.h adds FABLE_RETAIL_CPIXELFORMAT_PFDDATATORGB (declaration only); engine/CRGBColour.h,
  engine/CD3DFormatInfo.h unchanged. Variants a2 (long pixel) / a3 (chain pos shifts) were written but not graded (fg.py stopped at a1).
- Source: src/009e3df0.cpp, test: tests/009e3df0_test.cpp (vt.py PASS). differ/009e3df0.cpp is the old parked draft, superseded.
- For the coordinator: (1) twin 0x009E3C80 (not in functions.tsv) is byte-identical = ?ConvertPFDDataToRGBLong@CPixelFormat@@QBEXPBEPAK@Z by
  Ego_r / FableWin order; the same body with `unsigned long* colour` cannot be byte-identical unless it casts to CRGBColour -- not tried.
  (2) 0x009E3F60 ConvertRGBToPFDData (hand1014 differ, same "re-reads" residue) should be retried with _POSSHIFT_FORMATREF.
  (3) NAME_FIXES.tsv: landed CTexture_CalcAlphaHistogram_009fbbd0.cpp declares this callee `(unsigned char*, CRGBColour*)` + a local CRGBColour.
- No made-up names.

### Repair (2026-11-12, verifier findings)
- Test: the "0xEE trailers" claim was untested (a 4-byte memcpy mutant passed). Added synthetic model entry 4 (BitDepth 16, R/G/B 8/8/8):
  red's mask covers byte 2, so R == 0 only if exactly BitDepth / 8 bytes are copied. vt.py PASS; the 4-byte mutant
  (verify_evidence/m2_repair.cpp) now FAILS. Source unchanged; regrade RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1.
- NAME_FIXES 0x0129BA40: now cites PDB evidence -- the Fable Anniversary test PDB's S_PUB32 ?GD3DFormats@@3QBVCD3DFormatInfo@@B +
  S_GDATA32 GD3DFormats (independent PDB) and the chunk-680 raw Ego_r record next to ?DELTA_NOTIONAL_ZERO@@3MB (0x0129BA3C).
  Caveat for the coordinator: the chunk-680 Ego_r record came from a ChatGPT raw dump (pass10 AUDIT.tsv marked it single-source);
  the Anniversary PDB is the second, independent source. The mangled form matches the landed rows' extern const CD3DFormatInfo GD3DFormats[] (0x009E3640, 0x009E3B60).

## 0x007F5150


## 0x007F5150 CTCSoundAtmosphereVillage::GetGain -- RELOCATION_MATCH (author pass 1, 2026-11-12)
- What made it match: (1) the port onto the shared headers fixed the whole "register allocation" residue by itself (339 v 355, only
  block layout left): `CTCVillage* village; GetParentThing().GetTC(&village);` and `CTCCreatureModeManager* modes;` declared outside
  the loop + `it->Get()->GetTC(&modes)` (FABLE_RETAIL_CTHING_GETTC_EXPANDED + HASTC_FORCEINLINE), STLport list iterator loop over
  `village->VillagePeople`, `modes->GetCurrentMode().GetType() != NCreatureMode::MODE_SLEEP_BED`. (2) Block layout: the body sits
  inside `if (name == "STREETLIFE") { ... } return -1.0f;` (not an early `if (!(..)) return -1.0f;`), and the clamp assigns
  (`if (gain < 0.0f) gain = 0.0f; else if (gain > 1.0f) gain = 1.0f; return gain;`) instead of returning literals (literal
  `return 0.0f` is tail-merged with the early `return 0.0f`).
- Headers / flags: no flags, no pragmas, no overlay header (hdr/ is empty). Defines: FABLE_RETAIL_CCHARSTRING, CCLASS,
  DEF_CLASS_BASE, CTHINGAICREATURE, CTHINGCREATUREBASE, CTHINGGAMEOBJECT, CTHINGPHYSICAL, CTCBASE, CTHING, CTHING_GETTC_EXPANDED,
  CTHING_HASTC_FORCEINLINE, CINTELLIGENTPOINTER, CTCVILLAGE, CTCVILLAGE_TCI, CTCVILLAGE_PEEKVILLAGEPEOPLE (the last is unused by
  the final loop but harmless). Strict-header regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Local models (no shared header exists): CTCSoundAtmosphere / CTCSoundAtmosphereVillage on the shared CTCBase, Ego_r.pdb members.
  PDef is modelled as one `const CSoundAtmospheresDef*` (Ego_r: CDefPointer<const CSoundAtmospheresDef>, 4 bytes); not read here.
- Names: all PDB spellings; no made-up names. Old draft spellings proven wrong (draft only, nothing landed under them is claimed):
  `CTCVillage::VillageMembers` -> `VillagePeople` (engine/CTCVillage.h, Ego_r), `CMode::GetModeType` -> `CModeBase::GetType`,
  `CInterfaceMap::LowerBound` -> CVectorMap<..>::LowerBound. The landed sibling 0x007F50B0 OnInitialActivate still uses a local
  CThing / GetVillage() helper; it would likely re-spell the same way (`GetParentThing().GetTC(&village)`), not checked here.
- Grades: 1 TIMEOUT (cl hang on CDefPointer.h #error: needs FABLE_RETAIL_DEF_CLASS_BASE), 3 SRC_FAIL (CThingAICreature chain
  defines missing), 3 DIFFER(339v355) (v1/v2/v3: modes inside or outside the loop and iterator vs const_iterator Peek all give the
  same bytes), a1 RELOCATION_MATCH, final strict RELOCATION_MATCH.
- Both layout changes are needed, each graded alone after the match: early return + clamp by assignment = DIFFER(351v355) (a2);
  nested `if (name == ..)` + literal clamp returns = DIFFER(343v355) (a3). Final source regraded strict: RELOCATION_MATCH. 12 grades in all.

## 0x00863CA0


## 0x00863CA0 CThingBodyReorienter::FillBodyReorientationPossibleIntersecting -- RELOCATION_MATCH (wf1 author, 2026-11-12, 21 graded variants)
What made it match (each step removed one class of residue):
1. Ported onto the shared headers (CThing/CTCPhysicsBase GetPhysicsTC()->GetDiameter(), CMainGameComponent::Get()->GetWorld().GetThingSearchTools()
   behind the private inline GetThingSearchTools() const, STLport vector from stlport/fable_containers.h, engine/CIntelligentPointer.h copy ctor).
2. Frame 0x30: `C2DBoxI box` and `CThingFilter_HasPhysicsMesh filter` are NAMED locals, and the filter has FableWin's user constructor
   (??0CThingFilter_HasPhysicsMesh@@QAE@XZ, empty) -- no zero byte stored, and it gets its own word (retail F+0x10). A temporary filter / box packs the frame to 0x20 / 0x28.
3. Iterator register (start loaded straight into ebp): the search template body visible, FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (shared header,
   the landed 0x00863E70 body, noinline) + FABLE_RETAIL_CTHING_STATTYPEFLAGS + FABLE_RETAIL_CTCMAPWHO + FABLE_RETAIL_CTCMAPWHO_SEARCHID.
4. Last residue (box corners stored as computed, `lea ecx,[eax+edi]`): the centre is `pos.ToC2DCoordI()` (C3DVector inline, FableWin
   ?ToC2DCoordI@C3DVector@@ sits beside ??0C2DBoxI@@QAE@ABVC2DCoordI@@J@Z as COMDATs), not C2DCoordI((long)pos.X,(long)pos.Y) in place.
Headers / flags: no overlay, `// Flags: none`. Defines: CCHARSTRING, CCLASS, DEF_CLASS_BASE, C3DVECTOR, CTCBASE, CTHING, CTCPHYSICSBASE(+_DIAMETER),
CMAINGAMECOMPONENT(+_GETWORLD,_WORLD), CWORLD, CTHINGSEARCHTOOLS(+_ROUGHAREABODY), CTHING_STATTYPEFLAGS, CTCMAPWHO(+_SEARCHID),
C2DBOXI_RADIUS_INITIALIZERS, CINTELLIGENTPOINTER(+_COPY). Local classes (no engine header exists): CThingBodyReorienter, CThingFilter_HasPhysicsMesh.
Ideas that did not matter / were wrong: declaration order of box/filter/things (same bytes), 4-long box ctor, Initialise-form radius ctor,
_M_insert_overflow declaration-only (worse), STLport uninit helpers out of line (no change). No made-up names. No landed name proved wrong
(the dossier's draft spellings GetPosition / <const CThing> were draft-only; landed callees already use GetPos / <CThing>).

## 0x00688D40


## 0x00688D40 CInputProcessZTarget::AddToInterface -- RELOCATION_MATCH (grade 1 of 2; second = strict-header regrade)
- What made it match: the port onto the shared headers alone. The old draft's this/def register swap (362v360) was a model error
  (local CIntelligentPointer / CDefPointer / CThing stand-ins with __forceinline ctor/op=); with engine/CIntelligentPointer.h
  (ASSIGN + COMPARE_NULL_INT), ThingAlive.h IS_THING_ALIVE on both PCreature and the raw target, CThing::PeekTC<CTCTargetingBase>
  (GETTC_EXPANDED), the DEFAULT CDefPointer Release (no COUNTREF / POINTEE needed) and the PDB callee names, the first grade matched.
- Headers / flags: no pragma, no compiler flag. Macros: CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHINGPHYSICAL, CTHINGGAMEOBJECT,
  CTHINGCREATUREBASE, CTHINGCREATUREBASE_HEROCOMBATDEF, CINTELLIGENTPOINTER (+_ASSIGN, _COMPARE_NULL_INT), IS_THING_ALIVE,
  CTHING_GETTC_EXPANDED, UPDATE_NEAREST_INTERESTING, PLAYER, INPUTPROCESS, and the NEW opt-in FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
  (overlay hdr/engine/CAInputProcess.h, CInputProcessGameBase.h, CInputProcessZTarget.h: declarations of the virtual AddToInterface, slot 1).
- Test: test.cpp (vt.py PASS). Strict regrade (LANDVERIFY_STRICT_HEADERS=1): RELOCATION_MATCH. No made-up names.
- Landed spellings that disagree with the PDB (NAME_FIXES.tsv): 0x00687A70 landed as CInputProcessGameBase_Methods::AddToInterface(CInputProcessManager*)
  + `Helper` for 0x00A0D2B0 (PDB: CInputProcessGameBase::AddToInterface(CInputProcessManager&) / CAInputProcess::AddToInterface); sibling
  0x00688EB0 calls vtable slot 33 `GetTargetThing` (PDB ?GetPTargetedThing@CTCTargetingBase@@UBEPAVCThing@@XZ).

## 0x006EA0C0

## 0x006EA0C0 CCombatManager::UpdateDefenderAttackPattern -- RELOCATION_MATCH (24 grades incl. 2 probes; strict headers OK; test PASS)
- Source $W/006ea0c0.cpp, test $W/006ea0c0_test.cpp, leaf `?UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z`, no extra flags.
- What made it match: (1) the port onto the shared headers with the dossier's identities (326v360); (2) FABLE_RETAIL_STLP_TREE_CONSTFIND
  (const map::find + tree _M_find inline); (3) FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (GetDef<CAttackPatternDef> 0x006F0E80
  out of line but body visible -> the handle is read once after the call; 378 -> 360); (4) the last residue, header/found-node in
  edi/edx swapped (360v360, 8 hunks, stable over 13 spellings), went away with a named loop local
  `CThingCreatureBase* attacker = it->second;` before `attacker->GetCombatTypeDef()` (probes: that local alone matches; an
  `attacker_pattern` local alone does not).
- Headers: overlay $W/hdr/engine/CCombatManager.h adds FABLE_RETAIL_CCOMBATMANAGER_DEFENDERATTACKPATTERN (private decls of
  UpdateDefenderAttackPattern 0x006EA0C0 and ResetDefenderAttackPattern 0x006E8C30, both PDBs). Row defines: CCHARSTRING, CCLASS,
  DEF_CLASS_BASE, CDEFSTRINGTABLE, CTCBASE, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE(+_COMBATTYPE), JACK_STAGE1_UPDATE
  (declares GetCombatTypeDef), CCOMBATTYPEDEF, CWORLD, CMAINGAMECOMPONENT, CWORLD_PEEKDEFINITIONMANAGER, CDEFINITIONMANAGER, CRESOURCELIST,
  CGAMEDEFINITIONMANAGER, STLP_TREE_CONSTFIND, CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY.
- Local model: CAttackPatternDef (no shared header, only engine/_quarantine) as `: public CParentDefClassBase { std::vector<long> AttackPattern; }`.
  A shared engine/CAttackPatternDef.h would let this row and 0x006E8600 / 0x006E86A0 / 0x006E97F0 / 0x006E9D90 drop their local copies.
- Names: no made-up names. Manifest label `CDefendingCombatantInfo::InitializeDefenderFromCreature` is wrong (see LABELS.tsv); the landed
  ResetDefenderAttackPattern row 0x006E8C30 spells its handle CDefPointer<CAttackPatternDef>; the PDB mangling is
  V?$CDefPointer@$$CBVCAttackPatternDef@@@@ (const T) -- the overlay declaration uses the const form.
- Ideas that did NOT move the edi/edx residue (do not repeat): template-body const find, forceinline _M_find
  (FABLE_RETAIL_AUGMENTATION_PARTICLES), nested if, `end() != found`, `!(found == end())`, found declared outside the loop,
  const_iterator over the attackers, while loop, def as pointer + end() local, continue-style, find twice (393), map reference local (353).

## 0x004B4260 (held back)


## 0x004B4260 CQuestManager::ActivateMultipleQuests -- RELOCATION_MATCH (wf1 author, first attempt)
Rows matched: 1 (this one). Grades: 20 grade lines in ATTEMPTS.log (incl. 1 cl TIMEOUT and 3 SRC_FAIL fixes, 3 of them the probe variants p1-p3); probes: 2.
What made it match (three source facts, all found by grading the row):
1. The progress text is a NAMED block-scoped local (`{ CCharString text("QuestManager: Activate Quest", -1); DisplayProgress(text, -1.0f, false, false); }`):
   the ctor then runs before DisplayProgress's arguments are pushed (a temporary in the argument list is built after `push 0 ; push -1.0f`).
2. `activated = true;` is written in BOTH arms of `if (info) ... else ...` (each arm: push_back of its own CActivateQuestInfo temporary, then the store).
   One store after the if/else keeps the index in edi (336 v 359).
3. CALLEE VISIBILITY: the private ActivateQuests 0x004B3CE0 is defined earlier in the retail source file. With its body in the TU VC7.1 knows the
   local `quests` array is not captured: `activated` goes to bl, `this` to edi (spilled), quest_names to ebp, the index stays in the frame, the zero
   register esi is re-zeroed on the back edge, and the array start stays in ebp across the destructor loop. Declaration only = 349 v 359.
   Proved first with two probes (a reads-only body; a body that passes `this` + element addresses to unknown code): both RELOCATION_MATCH.
   The row file therefore carries a reconstruction of ActivateQuests' body from the 0x004B3CE0 disassembly (two passes: resources, then
   one CActiveQuest per inactive quest). It is UNPROVEN (never graded against 0x004B3CE0; the verify_mechanical/aq grade ended in ERROR;
   no landed row backs it) and NOT landed: row 0x004B3CE0 (1285 bytes) stays open;
   whoever lands it should start from this body and, if its spelling changes, regrade 0x004B4260 with the landed body in place.
   vector<CActivateQuestInfo>::push_back 0x004BB720 is visible-but-out-of-line through FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE.
Headers / flags: no compiler flags. Defines in the row file: FABLE_RETAIL_CCHARSTRING, _CWORLD (+ _GAMESCRIPTINTERFACE, _MESSAGEEVENTMANAGER), _NSCRIPT,
   _STLP_VECTOR_PUSHBACK_NOINLINE, _STLP_SET, _STLP_TREE_CONSTFIND, _STLP_TREE_ERASE, _STLP_TREE_ERASE_KEY, _CSCRIPTINFO_FASTCALL_ALLOC (overlay),
   _CQUESTMANAGER + _LOADGAMESTATE + _ACTIVEQUEST + _SAVEGAMESTATE + _ACTIVATEMULTIPLEQUESTS (overlay). Overlay: hdr/engine/CQuestManager.h, hdr/engine/CScriptInfo.h.
   Strict regrade (LANDVERIFY_STRICT_HEADERS=1): RELOCATION_MATCH. Test: 004b4260_test.cpp, vt.py PASS.
Local declarations (no shared header exists): CScriptInfoManager (GetPScriptInfo 0x00CB5AD0), CCodeSectionManager (ctor 0x009F0320, 0x009F0570,
   GetHandleFromLongName 0x009F0500, LoadSection 0x009F02C0; derives the shared CTBaseSingleton<T>).
Made-up (descriptive) names, found in no PDB / landed source: g_QuestScriptsEnabled (bool 0x01375454, one reader: ActivateQuests 0x004B3EED);
   parameter names quest_names / game_script /
   load_resources (the PDB has no parameter names; arg2 is stored to CActivateQuestInfo::GameScript, arg3 to LoadResources).
Ideas tried that did NOT move it: int index, index declared outside the for, `activated` declared first, index made address-taken through a
   reference alias / an inline `const unsigned int&` helper, `*(begin() + i)`, vector push_back inline (483 bytes) vs declaration-only vs NOINLINE.
Name findings (NAME_FIXES.tsv): the landed ActivateQuest 0x004B4A10 spells 0x004B00C0 `GetActiveQuestInfo` (it is IsQuestRegistered); the landed row
   0x009F0570 `CNameRegistry_009f0570::Contains` is a CCodeSectionManager member (candidate IsSectionRegisteredLongName); engine/CQuestManager.h names
   this function's parameters (names, load_resources, from_saved_game) -- the bools are (game_script, load_resources); engine/CScriptInfo.h spells
   AllocFunc __cdecl where retail calls it __fastcall.

### Repair pass (verifier findings, 2026-11-12) -- still RELOCATION_MATCH (normal + LANDVERIFY_STRICT_HEADERS=1), vt.py PASS
1. CRuntimeQuestPersistence maps renamed to the Ego_r.pdb names PersistScripts (+0x00) / MasterDataPersistScripts (+0x10)
   (struct_layouts_egor.tsv lines 81653-81655; Ego_r's maps are 0x14 each, retail's 0x10 -- offsets follow retail). Overlay hdr/engine/CQuestManager.h.
2. CPersistInfo::QuestName (+0) / PersistScript (+4) are Ego_r.pdb names (struct_layouts_egor.tsv 81640-81642): no longer listed as made-up;
   overlay comments corrected.
3. The carried ActivateQuests 0x004B3CE0 body is no longer called the REAL body: the comment now says it is an unproven reconstruction, not
   claimed byte-exact, and row 0x004B3CE0 must regrade this row when it lands.
4. The hand-expanded section load is now `sections.LoadSectionLongName(info.QuestName);` with LoadSectionLongName declared inline in the local
   CCodeSectionManager (PDB ?LoadSectionLongName@CCodeSectionManager@@QAEXVCCharString@@@Z, landed COMDAT 0x004AF000, same body); the made-up
   local `section_name` is gone.
Verifier findings not applied: none.

