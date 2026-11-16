# wfA5 notes

## 0x00B555A0 CEngineWeatherRenderer::Render -- RELOCATION_MATCH, test PASS, addmatch done
- Matched from a hand-written draft (the dossier's outline) on the DEFAULT SetRenderState text: NO SETRENDERSTATE_DESIRED, NO
  CRENDERSTATECAPTUREBLOCK_INLINE (the RenderClouds levers make it differ: DESIRED 11 hunks, DESIRED+BOOKMARK 15, ADDSTATE_DIRTY 11).
  `if (!PeekRenderEnabled()) return;` (FableWin accessor) and direct `RenderEnabled` under CENGINECOMPONENT_PROTECTED both match; Peek kept.
- Scope objects capture, shaders(true, true), layout(1). Opt-ins: CENGINECOMPONENT, CENGINEWEATHERRENDERER(+_BUFFERS, _RENDERRAIN,
  _RENDERSNOW, new _RENDER), CENGINEWEATHERSETTINGS_TAIL, CRENDERMANAGERCORE. No flags. Header: new opt-in in hdr/engine/CEngineWeatherRenderer.h.
- vland (read-only): RELOCATION_MATCH PASS. Warnings: RELOC_CONFLICT D3DAlphaFuncModeTable -> 0x01396DD8 here (symbol + 0x10 for
  ALPHA_TEST_GREATER), 0x01396DE8 in landed 0x00BBC460 (that landed row binds the element address to the symbol name: coordinator check);
  RELOC_NAME D3DTextureUVModeTable 0x01396DC8 labelled g_textureUVModeToD3D (label vs PDB name; ours is the PDB static member name).
- EARLY_RETURN opt-in variants spun cl until timeout (probably an unbalanced #if in that branch): not used.

## 0x00598A1C NUISystem::CFrontEndManager::BuildComponents(bool) (catalog "Init2") -- RELOCATION_MATCH, test PASS, addmatch done
- Name proven: FableWin.pdb ?BuildComponents@CFrontEndManager@NUISystem@@QAEX_N@Z + the sole caller 0x0042EA62 in landed
  CNewFrontendGameComponent::Init2 0x0042E98F. Length 4928 confirmed (code + 17-entry jump table; next function 0x00599D5C).
- Matched at grade ~8 from a hand draft in the twin's (OpenPCOptionsMenu 0x0058AF10) spelling. The two closers: (1) the first-screen tail is
  `if (!m_bQuitFromGame) { if (show) InitScreenStack(map[-1]); else InitScreenStack(map[20]); } else { LoadProfile(..); InitScreenStack(map[0]); }`
  (a ternary key gives neg/sbb; quit-first order lays the arms wrong); (2) the redefine-keys block uses two named locals `accept` then
  `redefiner` (accept alone named sits in ebx; both inline in one call fixes the frame but not the block). FITTED: `redefiner` (made-up name).
- `#pragma optimize("s", on)` (twin), no flags. Opt-ins: twin's set (CCHARSTRING, CWIDESTRING, STLP_LIST, STLP_MAP, CCOUNTEDPOINTER_CALLED_CTOR,
  _RELEASEDATA, CREDEFINERLIST_SETACCEPTBUTTON, CUSERPROFILEMANAGER_DISPLAYMODES + _SAVENAMES (GetProfileName), CDISPLAYMANAGER(+_ANTIALIASING),
  CTEXTURE, CPIXELFORMAT, SCOPED_PTR, C2DBOXF, C2DVECTOR, DEF_CLASS_BASE, CENGINEPERSISTENTPRIMITIVEBASE), CFRONTENDMANAGER_DESTRUCTOR + new _BUILDCOMPONENTS.
- vland: RELOCATION_MATCH PASS; RELOC_NAME warnings only (landed labels GetAnimFilter for GetDisplayModes 0x00406880, SetMember_dword0x18c for
  SetAcceptButton 0x00556570, list ctor/dtor spelled as _List_base by the header) -- the PDB spellings are ours.

## 0x00AA0FA0 C3DAnimationManager::CalcBoneTransformation -- RELOCATION_MATCH, test PASS, addmatch done
- Built from the landed CalcMeshTransformations 0x00A9F540 (helper block verbatim) + ApplyParentTransformations' opt-ins; ~70 grades.
- Closers in order: SetPosition through direct stores / Initialise instead of a C4DVector temporary (-192 bytes), `scale.Initialise(1,1,1)`,
  SWAP(current, other) BEFORE keyframes[j].ToMatrix, `bone_index` named before the keyframe reference (IsBoneEnabled SIB), the parent matrix
  as ONE function-scope aligned local (frame 0x214 and the in-loop store scheduling), and finally SetPosition = `Position.Initialise(position,
  0.0f)` (FableWin-size-supported; closes the BlendWith dot / slerp and x87 AlignedSetMultiply operand orders, which are keyed on the IL of
  the unrolled scaling loop -- dead-local rulers 1..48 never moved them; a diagnostic without the matrix section did).
- Overlay opt-ins: C3DANIMATIONMANAGER_BONETRANSFORM, C3DBONE2_GETNAMECRC, CMATRIX4X4_COPYTO, C3DKEYFRAME_SETPOSITION, new header
  engine/C3DSkeletalMorph.h; also C3DVECTOR_GETSCALED_NAMED_RESULT exists in hdr but is NOT used by any matched row (FableWin 34i v 32i).
- No flags, no pragmas beyond the twins' `#pragma intrinsic(sqrt/fabs)`. Retail-asm lines cite the twins' per-site tables (coordinator: confirm
  that is enough for these new sites, else a per-site table run is needed).

## 0x00AA2110 C3DAnimationManager::CalcBlendedBoneTransformation -- RELOCATION_MATCH, test PASS, addmatch done (successor agent)
- CLOSED (successor): under the twins' shared helper texts (SetPosition = Initialise, plain GetScaled) the row matches with ONE row-local
  spelling: `keyframes[0].ToMatrix(matrix_a, false);` instead of `(*current, false)` (FITTED: same code; c2 record-id keys). A manual
  `CMatrix4x4* previous = current; current = other; other = previous;` instead of SWAP also matches (not used: SWAP has FableWin evidence).
  Both spellings flip the twin 0x00AA0FA0 to 13 hunks, so the twins keep different spellings here. Measured on the way: head named-local
  sweep (64 variants: blend_count / start_time / start_alpha / relative / keyframe refs / index): all 13 (named locals never move these keys);
  scaling loop with `keyframes[j - 1]` indexed twice: BlendWith closes, AlignedSetMultiply stays, remainder-loop schedule changes (6 hunks);
  W-first SetPosition + double index: the reverse. The residue was two independent order bits (BlendWith, AlignedSetMultiply); the matrix
  section's spelling toggles both. ~150 grades total in c4/ (sweeps s1..s6). vland RELOCATION_MATCH PASS. Old differ in tmp/00aa2110.differ_old.cpp.
- (history of the first agent follows)
- Draft = row 3's text with CalcBlendedMeshTransformations 0x00AA0090's blended head; matched at the 2nd grade WITH SetPosition written as
  W-first direct stores (`Position.W = 0.0f; X; Y; Z;`) -- that exact file is kept as tmp/00aa2110.matched_with_Wfirst.cpp.
- Twin conflict (one helper text for both rows): SetPosition x GetScaled sweep (I = Initialise, W = W-first, X = XYZW direct; P = plain,
  N = named-result GetScaled): I/P r3 MATCH r4 13; I/N r3 13 r4 MATCH; W/P r3 13 r4 MATCH; W/N r3 3 r4 12; X/P r3 18 r4 5; X/N r3 8 r4 17.
  Chosen I/P (Initialise has FableWin size support; GetScaled plain is the landed 0x00A89740's text). Residue under I/P: BlendWith Dot order
  (retail Y,Z,W,X; ours X,Y,Z,W, 0x00AA2750..0x00AA2771), slerp products 0x00AA292C..0x00AA296F, x87 AlignedSetMultiply first column
  0x00AA31CA..0x00AA31E7 -- all keyed on the function's IL amount. Tried for row 4 under I/P: scaling statement forms (direct, named scaling
  ref, named position): no match. Next ideas: HasSkeletalMorph text fit (FableWin 24i/72b; ours 26i/78b; candidates 25i/75b tried by fwsize
  only), GetPosition / CopyTo alternatives (both SAME already), row-4-only natural names in the blended head (start / end state refs).
- Later sweep (HasSkeletalMorph texts H2 `Data.Get() != 0 && Data->SkeletalMorph.Get() != 0`, H3 `if (Data.IsNull()) return false; return
  !Data->SkeletalMorph.IsNull();`, H4 `!Data.IsNull() && Data->SkeletalMorph.Get()`): I+orig r3 M / r4 13; I+H3 M / 13; I+H4 13 / M;
  W+H3 13 / M; W+H4 M / 13; H2 breaks both. Every shared-helper lever flips BOTH twins (a parity-like key): the twins need a ROW-LOCAL lever.
  Row-4-only spellings tried without effect: named PeekSkeletalMorph reference, named chain bone in the scaling-target loop, scaling statement
  forms, dead locals; dropping the end_state reference changes code (54 hunks). Next: row-local IL inside the unrolled scaling loop or the
  partial-alpha loop of row 4 only (e.g. `const C3DVector& scaling = scales[j]` was tried: 13), c2 record-id trace (r3010 model.py) if available.

## 0x00B9AC30 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive -- successor: DIFFER(4229v4229) 19 hunks, near-miss (differ/00b9ac30.cpp, work in b9/)
- Successor compile fixes: GetRepeatedMeshDrawDistanceFactor() (+0x88, landed RenderPrimitive's spelling), GetRenderInfo()->Mesh->GetMaterial,
  bank->PeekGraphicInfoFrame(index, 0) (landed 0x00BA4A40 body, defined in the row), new opt-ins CENGINECAMERA_PEEKWINDOW (FableWin
  PeekWindow), CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS, CENGINEVSCONSTANTLAYOUTREPEATEDMESH (retail class branch) +
  CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTREPEATEDMESH (replaces the draft-only REPEATEDMESH_BATCH stand-in, now deleted from hdr);
  existing opt-ins SUBPRIMITIVEBASE_PARENTBASE, RESOURCEMANAGER_GETSTIPLEFADETEXTURE (STIPPLE dropped), CSHADERRENDERMANAGER_INLINE_BLOCKDRAW,
  LIGHTINGMANAGER_SETFADED_SHADOWGUARD (replaces PEEKGLOBALSHADOWMODE), C2DBOXF + C2DBOXF_COPY; `// Flags: /GS` (cookie at 0x00B9AC36).
- Structure closers: duplicated assignment blocks (count == ObjectCount arm and the no-subsections arm, no flag), `++n, ++j` in the copy
  loops, batch loop `for (done...) { remaining = count - done; n = remaining >= batch ? batch : remaining; ... }`, window copied to a local
  `const C2DBoxF window = PeekWindow()` (member-wise copy: fixes frame size 0xC4 and the stipple x87 code).
- Then: C3DPrimitive2 accessors (FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH: GetMaterialIndex / GetVertexBuffer / GetIndexBuffer / GetVertexCount /
  GetTriangleCount) closed the draw loop (late imul) and FlushRender (37 -> 19 hunks). Names: NEngineInternal::GPOcclusionManager / GPLightingManager /
  GPCamera (reloc_bindings PDB names) instead of the RenderPrimitive sibling's g_pEngine*; CClipListElement fields Start / Size (Ego_r).
- Residue (19 hunks, equal length): (1) copy-loop bound re-read `clip.Start + clip.Size` loads Size first (0x00B9AEB5 / 0x00B9B001) + ebx/ebp
  restore order after the inner loop; (2) the inlined GetTexture / TouchResource temps (0x00B9B11E..0x00B9B1C7, 0x00B9B2C4): ours edx/eax where
  retail (and the landed out-of-line 0x00B66C10, same text) eax/ecx -- allocation order; (3) SetRenderState `Flags |= mask` operand order at the
  2nd and 3rd sampler setters (0x00B9B4C3, 0x00B9B534; 1st and 4th right). 0x00B9B26B / 0x00B9BC87 are relocation-only.
  Tried without effect: dead-local rulers 1..20 (24+ adds spill hunks), bound spellings (pointer, indexed, cast, swapped), bank / frame / mesh
  locals, `bool loaded`, texture declared earlier, GetTexture text with GetBank() (kept: = landed text), CGraphicFrameData::GetTexture via
  PTexture.Get() (probe hdr only), wrapping the rest in `if (GetTexture)` (frame +4: worse).

- Name: brief/dossier right (Ego_r 0x00A531E0 + FableWin + vtable 0x012A3E9C slot 5); the workflow task text calling it a constructor is wrong.
- Draft written from the dossier outline over the landed RenderPrimitive 0x00B9A4A0 preamble (its define list), the FlushRender text of the
  landed 0x00BA1B00, and new overlay opt-ins: REPEATEDSTATICMESHES_RENDERSUB (RenderSubPrimitive + 5 private helper decls, the Ego_r
  CSIMDVariableBlock, NEnginePrimitiveManagerRepeatedStaticMeshes::CClipListElement), OCCLUSIONBASE_PREPARETEST, SUBPRIMITIVEREPEATEDSTATICMESH_
  GETPARENT, OCCLUSIONMANAGER_TESTS (Start/EndOcclusionTest), CGRAPHICINFOFRAME_GETTEXTURE_INLINE (the landed 0x00B66C10 text as a forced
  inline), CENGINESUBPRIMITIVEBASE_PEEKRENDERFLAGS (FableWin PeekRenderFlags), CENGINERESOURCEMANAGER_REPEATEDMESH_BATCH (DRAFT stand-in only).
- Compile items left: see the differ file's Grade line. The resource-manager VSConstantLayoutRepeatedMesh needs a retail-offset typing
  (retail reads the ObjectMatricies count at GPResourceManager+0x3034; header layout is the Ego_r 0x30F8 one) -- FableWin accessor
  ?PeekVSConstantLayoutRepeatedMesh@CEngineResourceManager@@QAEABVCEngineVSConstantLayoutRepeatedMesh@@XZ.
- Expected residue classes after it compiles (dossier): /GS frame packing of 0xC4 bytes with heavy slot reuse, copy-loop forms (countdown
  over clip entries, bound re-read), batch-loop pointer strength reduction, faded-shadow bool spelling (`!(flags & 0x200)` zeroed unless
  ShadowMode == 1: SetEnableFadedShadowFactor's own guard), x87 stipple constants (float * then + double 0.015625).

### 0x00B9AC30 -- wfA5 pass 3 (b10/, 2026-11-15): 93 raw fg hunks -> 7 (differ/00b9ac30.cpp)
- Frame slots (the 93 -> 22): the earlier "19 hunks" counted frame-masked hunks only; every slot of the clip section was permuted. Packer probe
  (tools/pk/pk.py + pkslots.py; what-if sort tools/pk/whatif.py / search.py) showed one cause: `layout` (CScopedVertexShaderConstantLayout)
  joined clip_end's slot. Retail = clip_end conflicts with layout = clip_end declared at FUNCTION scope (`CClipListElement* clip_end;` beside
  clip_list, assigned inside the if). That alone made the SubSections test reload; naming `const CSubsectionElement* sub_sections =
  primitive->SubSections` restores retail's single load. fm (frame-map lines) = 0 since.
- GetTexture inline (the 22 -> 7): the opt-in body is now the sibling GetFrame text (TouchFrame(mip) unless flags & 8, if/else finer-level arm,
  IsLoading): FableWin /Od /EHsc 149i/471b v FableWin 149i/470b (the goto text 203/661); 0x00B66C10 RELOCATION_MATCH with it (b10/gt5.cpp).
  TouchFrame's GetBank()->GetCurrentFrame() read twice is what fixed the eax/edx picks at 0x00B9B11E..0x00B9B1C7 / 0x00B9B2C4.
- Globals renamed to NEngineInternal::GPShadowRenderer / GPPrimitiveRenderer (Anniversary PDB S_GDATA32 + the TLC NEngineInternal::GP* family in
  reloc_bindings; no TLC PDB record found): code-identical.
- Left: (1) copy-loop bound re-read `clip.Start + clip.Size` loads Size first (0x00B9AEB5, 0x00B9B001); (2) test/or operand order at the
  SetTextureUVModeU / V expansions (0x00B9B4C3 / 0x00B9B534): retail's flip point is two setters later than ours (Min right in both, Mag right,
  U/V wrong, later sites right).
- No effect on (1)/(2): dead asserts 0..140 at top/tail/inside FlushRender (only other sites move), 1..12 initialised unused locals (byte-identical),
  SETRENDERSTATE_DESIRED / BOOKMARK / ADDSTATE_DIRTY combos (byte-identical), TOUCHTEXTURE_GET / CURRENTFRAME_DIRECT (byte-identical).
- Measured for residue (1): comparator 0x1076A394 is `cmp [a+0x10],[b+0x10]` (full 32-bit key, unsigned). Keys at the bottom bound (b10/lk_v3.json,
  tools/lk/lka.py = wfB6 lk without the float filter): Start conv tree 0x00028024 / 0x0002E024, Size 0x010303CC / 0x0103042C (2nd loop): Size's
  top byte 0x01 (its IND(ADD(clip,1))) always outranks; low16 = f(clip leaf id). So no record shift can flip it; retail's Start tree must carry
  a binary node too. Tried without effect: clip_list[c] indexed, pointer `&clip_list[c]` / `clip_list + c`, copy `CClipListElement clip`
  (frame change), `j - Start < Size`, Size + Start, while-loop / ++n in the body forms, k-offset loop (LFTR idea, 4170 bytes).
- Measured for residue (2): sites in address order b21d (specular), b3e5 Min, b454 Mag, b4c3 U, b534 V, b6ef / b75b / b7ca (cull, alpha test, alpha
  ref): ours flip after Mag, retail after V. Byte-identical under: all count devices (above), 31 named-v-inline combos (faded, remaining, parameters,
  bank, occlusion constant; b10/nm/), FlushRender texts (b10/fr/), PeekGraphicInfoFrame texts (double PeekGraphic is required: p1/p2 4197 bytes).
  Comparator log late pairs (ids mod 2048, near the wrap): (328,332) (368,406) (510,622) (548,694) (582,774) then (1840,1862) (1840,1858) (1828,1866)
  / (1848,1796) (1840,1794) (1828,1792) -- 5 + 3 sites; decoding which temp is which needs one more trace with record names of the CSE temps.
- Next ideas (not tried): identify the two CSE temps per site in the lk log (dump records for kind 0x250 '#' leaves with their tuple line) and
  compute the id window retail needs; a helper text with fewer c2 temps BETWEEN the U/V expansions and the end (retail needs the wrap two sites
  later = fewer records, which no additive ruler gives); the stipple-block statement shapes (the four setters through a helper / loop).
- Grades this pass: ~190 (b10/, ATTEMPTS.log). Row parked: equal length/calls/frame, 7 fg hunks (2 relocation-only).

### 0x00B9AC30 -- wfA5 pass 4 (b11/, 2026-11-15): copy loops CLOSED; 2 sites left (U/V bookmark test/or)
- Copy-loop residue (Size-before-Start at 0x00B9AEB5 / 0x00B9B001) closed by a named pointer IV `const CClipListElement* clip = clip_list;`
  `for (c...; ++c, ++clip)` with `clip->Start/Size` (base-form references / indexed / pointer `= &clip_list[c]` all give Size first: that order
  is structural there; with a named IV symbol it is id-keyed and comes out Start first in both loops). The init then sat before the count test
  (jle moved, 0x00B9AD98 / 0x00B9AEFA): a redundant `if (clip_count > 0)` guard round each loop puts it after, as retail. FITTED (device a).
  Probes: b11/s/s10 (IV, init before guard), b11/u/k1 (one spare local fixed loop 2 before the guard existed; not needed with the guard), b11/x/x1n.
- fg.py now shows 5 false hunks (`mov [x], 0` lines: its norm() turns ours' literal 0 into REL when the bytes hold 00000000); real residue =
  sw11.sh / sd2 list: b4c3 b4c7 b534 b538 (+ relocation-only b26b bc87).
- U/V measurements this pass: k = 1..16 asserts / spare locals BETWEEN Mag and U, or at 7 other positions, change nothing (records are not
  positional for these leaves); 24..128 asserts there move other sites (copy loop af60, late setters b6e6.., sampler load order at 64).
  Top asserts (`FABLE_RETAIL_ASSERT(primitive)`) 1..40 on the new base: U/V never flip; late setters flip at >= 31, af60 at >= 27, batch loop at 13..15.
  Wrap model (lk ids are id mod 2048): Mag (510,622) U (548,694) V (582,774) -> a uniform shift flips U/V but not Mag only for
  delta in [1354,1426) or (if these ids are already in the second 2048 block) [-694,-622): ~650 fewer records before the sampler sites than ours.
- U site CLOSED (b11/rf/m03): `const unsigned long render_flags = sub_primitive.PeekRenderFlags();` named after `layout` and used for the
  stipple test and its inner 0x200 test only (code-identical; also using it at the else-if is the same; using it for `faded` / the shadow-setup
  test / ProcessLightingSW changes code). FITTED. Mechanism: the test operands at each SetRenderState site are two c2 temps (lk_v3: spec (194,20),
  Min (248,184), Mag (288,258), U (328,332), V (368,406), cull (510,622), alpha test (548,694), alpha ref (582,774); higher id goes first; flags
  temp ids +40/site, mask temp +74/site): the order is decided by temp numbering, which statement shapes BEFORE the sites move (x87 constant
  statement through the C4DVector overload also flipped U, but changes calls), while dead asserts / spare locals / setter-text asserts do not.
- Left: V site only (0x00B9B534 / 0x00B9B538). No effect on it: spare locals 1..4 before render_flags, render_flags type (unsigned/long/const),
  `!= 0` spellings, named stipple texture / shadowed bool / camera / x87 constant locals, SetRenderState opt-ins DESIRED / BOOKMARK / DIRTY (all combos),
  4 dead asserts inside a probe SetRenderState opt-in (b11/hp, probe only, not handed back).
- Further V-site attempts on base4 (all byte-identical, V still swapped): top dead asserts 1..24 (`FABLE_RETAIL_ASSERT(primitive)`; 13..15 move the
  batch loop instead), unused locals 1..7 inside the stipple block and 1/3 at top (5 at top re-breaks the id-keyed copy loop 0x00B9AEB5: keep
  the top of the function as it is), named material_index / diffuse_index / loaded / stipple texture (ref and pointer) / uv_mode / filter_mode /
  v_stage, stipple_stage at function-ish scope, x87 constant commutations (x1..x4), render_flags declared before capture / specular (code change).
- lk trace tool (tools/lk/lk.py) returned only 200 entries twice on base3 (gdb session ends early; machine was loaded); not retried (loop rule).
  For the next agent: one good trace of base4 + the pair list per site (cmp_sites method in b11/cmp_sites.txt: the test operands are the
  type-0x20 leaf pair logged right after each site's `251|c2814` comparison) would give the V pair and the needed shift directly.
- Hand-back state: differ/00b9ac30.cpp = b11/base4.cpp; fg shows 7 hunks of which 5 are fg false positives (`mov [x], 0`), real residue = V
  site 2 hunks. No overlay header changed this pass (the SetRenderState assert probe lives only in b11/hp, not handed back).
- render_flags subsets (b11/rf): replacing ANY two (or all three) of the three pre-V PeekRenderFlags() uses fixes U (m03 272+274, n5 272+291,
  n6 274+291, m07 all three); one alone (m01 / n2) does not. So the lever is the NUMBER of inline PeekRenderFlags expansions before the sites
  (c2 temps of each expansion shift the later test-operand temp ids). V needs a larger shift in the same direction (lk: V diff +38 before,
  U diff +4) -- about 10x more expansions removed before the sites, and no code-identical source of that many was found (the later uses at
  `faded` / shadow-setup / ProcessLightingSW change code because render_flags then lives across calls). Parked.

### 0x00B9AC30 -- wfA5 pass 5 (successor, b12/, 2026-11-15): V-site mechanism MEASURED; still 1 site (2 lines); parked
- Base = differ/00b9ac30.cpp (unchanged; residue b534/b538 + relocation-only b26b/bc87). ~275 grades + 3 c2 traces this pass (b12/).
- c2 trace of base (b12/lk_base.json, tools/lk/lka.py, full 25.9k entries; tools/lk/pairs.py prints leaf pairs with record index):
  the operand key holds (2*rec) mod 2048, i.e. operands compare by RECORD INDEX MOD 1024. Per SetRenderState site the pair is
  (flags temp f, mask temp m): spec (3172,2057) Min (3199,2139) Mag (3219,2176) U (3239,2213) V (3259,2250) cull (3330,2360)
  alpha test (3349,2396) alpha ref (3366,2436). m = phase-A records (37 per site), f = a later phase (20 per site).
  Rule: with G = f - m, the flags temp goes first iff G > 1024 (for these m mod 1024 values). Retail: spec..V flags first,
  cull..ref mask first. Ours: G_U = 1026 (right, margin 2), G_V = 1009 (WRONG, needs +15), G_cull = 970 (right, margin 54).
  So the fix needs G_V +15..+53 (if added after the last site) or >= +15 (if added between V and cull) -- a DIFFERENTIAL shift.
- Uniform shifts do not work: 26 top dead asserts = +288 to both f and m (~11 records each, b12/lk_t26.json); V never flips
  (t12..t30, t41..t96, t73..t77 measured), other mod-1024 comparisons (af60 / late-setter load order) break first.
- What DOES raise G_V (diagnostics, all change code): live inline member expansions with FIELD STORES through a global pointer placed
  AFTER V in program order: one more SetEnableFadedShadowFactor (sf1, cull ok), AttachTextureToStage, SetFogEnable between V and
  cull (m1), SetAlphaTestEnable at the end (s1: V fixed, cull broken = too many), `GPLightingManager->StateUpdateFlags |= 1` x4 (b4;
  x3 not enough: each ~4 records). The same stores BEFORE the sites do nothing (t4/u4); out-of-line calls do nothing (n1/g1).
- What does NOT add those records (all measured, no V change): dead asserts anywhere (incl. conditions with inline calls on globals:
  b12/e), unused locals, renames (b12/r), 13 code-identical named-local / constant / ternary toggles (b12/t; also on top of a +12
  bias b12/tb), brace scopes after V (27 placements, b12/br), live accessor expansions on local pointers (16x IsTwoSided, b12/d/p*),
  C3DMeshMaterial::IsTwoSided (FableWin) for material->TwoSided (probe opt-in, reverted), redundant reads after V (g3), GFLimitUpper /
  min forms of the batch clamp (code change), SetEnableFadedShadowFactor text variants (b12/hp2, PV1..5), FlushRender texts (f1 =
  cloth-row SetStreamSource text lowers G: breaks U), stipple_stage type, same-TU callee bodies visible (opaque noinline bodies:
  byte-identical; empty bodies change reloads).
- Lowering G (breaks U, measured): an extra redundant `PeekRenderFlags()` read inside the stipple test (h1/h2: f -5 at ALL sites,
  m unchanged, b12/lk_h2.json) or at the function top (top2); removing the named `block` local (T15).
- Next ideas: (1) a natural retail statement AFTER V that adds >= 4 global-pointer field stores which c2 later deletes (none found;
  the code between V and the end is fully pinned by calls); (2) remove >= ~6 CSE-able reads between spec and V code-identically
  (the render_flags trick that closed U is this lever; no further candidate found); (3) any helper text change after V with more
  field-store records but identical code (texts there are all FableWin SAME by fwsize: PrepareForRender, CScopedVertexShaderConstantLayout,
  GetStipleFadeTexture, PeekVSConstantLayoutRepeatedMesh, CColorBufferDepthPackParameters, RestoreDefaultPrimitiveParameters).
  None of the owner's last-resort devices (fitted named local / guard, inline control, unused locals, dead asserts) reaches this
  record class: unused locals and dead asserts shift f and m together.

### 0x00B9AC30 -- wfA5 pass 6 (successor, b13/, 2026-11-15): FableWin helper-text pass (+1 f so far); still V site
- Anniversary 360 PDB: no CEnginePrimitiveManagerRepeatedStaticMeshes procedures at all (class absent from that build): no local list.
- Ruler (tools/rul.sh + b13/mkr.py, diagnostic only): x copies of `GPLightingManager->StateUpdateFlags |= 1` after UpdateGlobalLightingStates
  add +5 f each (m unchanged); base needs x=4 (f_V +16), x=3 short. tools/lk/sites.py prints the 8 site pairs of a trace (tools/trace.sh, 5.5 min).
- FableWin-evidenced texts applied (all code-identical, fwsize SAME / count SAME): CIVCOUNTEDPOINTER_RELEASE_ISNULL + CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
  (+ CASTTOBASE replacing RESET_FORCEINLINE) in GetTexture's Data.Reset path (the RenderClouds family texts; retail `dec [ecx+4]; jne; call [edx+4]`
  at 0x00B9B304), CRESOURCEBANK_CURRENTFRAME_DIRECT, CMANAGEDTEXTURE_TOUCHTEXTURE_GET + CIVCOUNTEDPOINTER_GET, NEW overlay opt-ins
  CENGINELIGHTINGMANAGER_SETFADED_SHADOWMODE_DIRECT (FW 24i/80b) and C3DMESHINFO_PEEKTEXTUREBANK_DIRECT (FW 10i/25b). Trace c5: f +1 (V needs +15 more).
- No f gain: C2DBoxF GetWidth/GetHeight/GetTLX/GetTLY in the stipple constants (x1..x3), render_flags in the else-if (e1), the 13 b12/t toggles,
  dead loads / dead local structs after V (b13/d D1..D4); EARLY_RETURN SetRenderState opt-in does not compile alone (nested opt-in, unusable).
- Traces (b13/lk_*.json; tools/lk/sites.py): base V f/m 3259/2250; c5 3260/2250 (+1); c7 = c5 (SHADOWMODE_DIRECT / PEEKTEXTUREBANK_DIRECT +0);
  c8 (+ CRENDERSTATECAPTUREBLOCK_INLINE) 3261/2250 (+2 total). Retail needs f_V%1024 > 202, i.e. f_V >= 3275: +14 still missing (cull keeps up to +51).
  3 ruler RMWs after V (c7x3): f +14 at ALL sites (spec too), m +10 only at cull..aref, early (IL) records +32: f moves with live code anywhere.
- Measured to add NO f (all code-identical): merged RMW chains (`StateUpdateFlags |= 1; |= 2; |= 4; |= 8` for `|= 0xf`, c2 merges them early),
  duplicate dead stores in FlushRender (x8), dead loads / dead local struct stores / empty-class locals (incl. one taking &count) after V,
  visible non-inlinable (recursive) ClipSubSections / ClipSubSectionsSIMD bodies (b13/v2, v3), SetRenderState operand-order texts (probe/sr p2..p4),
  window copy spellings (w2 Initialise, w3 direct-init, w4 non-const), stipple-constant division spellings (y1..y3), object_count named (o3; o1 moves
  the copy-loop bits instead), /d2inlT40..250 and /d2inlS10..400 (code identical, V unchanged; T<=35 changes code).
- TouchFrame text: FableWin 69i/228b v ours 64i/209b (+5i unexplained; the single-read form changes code at 0x00B9B11E..; probe/tf b1..b3 measured).
- Hand-back: differ/00b9ac30.cpp = b13/c8.cpp (same 2-line residue; previous pass-5 source kept as b13/differ_prev_pass5.cpp). ~170 grades + 6 traces.
- Next ideas: find ~3 more live-code-equivalent f sources (each live RMW = +5): only codegen-surviving IL counts; the FableWin row body is 825i v
  ours 687i (/Od): 138 debug instructions unexplained (asserts ~63..99 each, or real statements c2 folds late) -- a natural late-folded statement
  in the row body is the remaining candidate class.

### 0x00B9AC30 -- wfA5 pass 7 (successor, b14/, 2026-11-15): RELOCATION_MATCH, test PASS, addmatch done, vland RELOCATION_MATCH PASS
- What closed the SetTextureUVModeV bookmark test/or (0x00B9B534/0x00B9B538): the matched same-family twin's stipple-block idiom
  (CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants 0x00BA2350 / RenderMeshPrimitive 0x00BA5260, wfB3 src):
  `C2DBoxF window = GPCamera->PeekWindow(); float left = window.TLX; float top = window.TLY; float right = window.BRX; float bottom = window.BRY;`
  then the constant from left/top/right/bottom (our row's own retail-forced arithmetic), plus `TEXTURE_FILTER_MODE filter_mode = TEXTURE_FILTER_POINT;`
  for Min/Mag. The four named floats alone close it (b14/v/w01, w03 const window also matches); filter_mode kept for twin fidelity (w02 = src).
  Code-identical spelling, EVIDENCED by the twin (no device, no flag beyond /GS, no header change: hdr/ untouched, no regrade needed).
- Earlier measured "named-local toggles" (pass 4: filter_mode / uv_mode / window copy spellings) never included the four float locals.
- Other b14 measurements (all V unchanged even with the 2-RMW ruler detector, tools: b14/sw.sh, b14/mkv.py): params temporary, inline faded,
  while-form batch loop, &x[done], ternary forms, const n/remaining, alpha_ref named, 1..16 initialised unused locals after V, 1/4 dead asserts
  after V (4 conditions), loop guard / pointer / reverse count-loop spellings (code changes), duplicated cross-jump tail (not merged: +275 b).
- Test: test/00b9ac30.cpp (portable model: clip-list count / compaction, texture gate, stipple stage + constants + states, shader arms, faded,
  wind, cull, occlusion, shadow states, batching 32+23 / 32+32+6). vland WARN RELOC_NAME lines are DB labels (NAME_FIXES.tsv covers them).
