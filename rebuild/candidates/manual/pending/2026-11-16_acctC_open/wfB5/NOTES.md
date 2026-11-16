# wfB5 notes (author agent, parked rows batch)

## 0x007BC38A NThingShot::CTCShotDefinitionThrowPredictive::InitMovement -- MATCHED (RELOCATION_MATCH, test PASS, addmatch done; 9 grades)
- Start state: prev/007bc38a.cpp regraded DIFFER(880v880), the one known instruction (`mov eax,[esi]` before v after `fsubp` 0x007BC64D).
- What closed it: the launch formula split with a NAMED double for the first product:
  `double level_range = (distance / sin(a)) * cos(a); float one_over_speed_squared = (float)((level_range - height) * (2.0f * sin_squared) / (distance * distance * 0.12f));`
  (the subtraction of `height` then starts a new statement tree, and the scheduler places the vtable load after the `fsubp`).
  No effect (same 1-instruction residue): assignment-in-call `(physics = ..)->EnableGravity`, a `parent` local, a named double for the
  whole `(..) - height` difference. FITTED: the name and the existence of `level_range` (natural named intermediate; no PDB local names).
- Names: Ego_r + FableWin both have ?InitMovement@CTCShotDefinitionThrowPredictive@NThingShot@@MAEXXZ; vtable 0x0123CE9C slot 33.
- Headers / flags: same opt-in set as the predecessor draft (all existing opt-ins); no flags; pragmas as landed sibling 0x007BC6FA.
- vland (read-only) on 007bc38a: WIN; one WARN RELOC_NAME: 0x0129BA3C bound `?DELTA_NOTIONAL_ZERO@@3MB` (C2DVector.h, landed spelling) but
  labelled `?g_NormaliseEpsilon@@3MB` in the label set -> coordinator: label fix (see NAME_FIXES.tsv), not a source change.

## 0x007EA580 CTCTargeted::GetAllTargetPoints -- MATCHED (RELOCATION_MATCH, test PASS, addmatch done; 3 grades)
- Start state: prev/007ea580.cpp regraded DIFFER(457v457), the two known displacements (element Y / Z reloaded from the pair temporary).
- What closed it (EVIDENCED: retail TU order 0x007EA2D0 / 0x007EA2F0 / 0x007EA490 / 0x007EA580): the landed text of
  NTargeting::GetTargetingPositionOnThing (0x007EA2F0, same TU, between GetNumTargetPoints and GetTargetPoint) DEFINED above the row.
  Reading of the residue: the three element values are forwarded temps; c2 re-reads a spilled temp from an equivalent memory home that
  no intervening store can kill. With the callee extern, its hidden result pointer makes `position` address-exposed, so the stores through
  `finish` kill it and Y / Z come from the pair temporary; with the callee's body visible c2 knows the result slot is not captured and
  re-reads all three from the returned position, as retail. (Earlier agents had searched pair / _Construct texts: 32 grades, no change.)
- Opt-ins added for the callee's text only (all existing): STLP_SET_EMPTY, STLP_MAP_EMPTY, CCLASS, DEF_CLASS_BASE, CTHING_STATEACCESS, CWORLD,
  CWORLD_BULLETTIME_CONST, CBULLETTIMEMANAGER(+_INTERPOLATE), CTCPHYSICSBASE(+_INTERPOLATED_POSITION(+_BODY)). Header:
  hdr/engine/CTCTargeted.h FABLE_RETAIL_CTCTARGETED_GETALLTARGETPOINTS (C37's declaration opt-in, unchanged).
- FITTED: nothing. The source emits three extra functions in the row TU (the three same-TU bodies) -- retail's own TU shape.

## 0x007E15C0 CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw -- PARKED, DIFFER (52 grades / probes here; C37 had 64)
Start state: prev alt 963v963 (18 raw hunks, 3 of them phantom identical-text hunks), prev main 967v963.
NEW LEAD (measured, EVIDENCED by addresses): CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh 0x0077BD70 sits between the
CTCGraphicAppearanceStaticMesh functions (0x0077BC80..0x0077BD20) and ..FadeOutOnProximity (0x0077BE30): it is a header-inline COMDAT, so its
BODY IS VISIBLE in this row's TU (never expanded: 15 calls both sides). c2 reads the visible callee and knows which reference arguments it
captures. With the ctor defined in the row source (differ/007e15c0_visible_ctor.cpp; plain non-inline, `inline`, or noinline: same bytes):
  * the x87 order of the inline C3DPositionInterpolator::GetInterpolated becomes retail's exactly (X/Y/Z sums stored at once, `fstp st(0)`
    0x007E1832) -- the position temporary is no longer address-exposed. The sibling 0x0077BA40.. (StaticMesh::OnAppearanceDraw, same TU as the
    COMDAT) shows the same x87 shape at 0x0077BB04. Residue 2 of C37 is CLOSED by this.
  * the dead `channel.ChannelIndex0 = 0` store is removed and the flag load / test come before the two remaining byte stores, as retail, with
    the natural source (`if (entry.OverrideLightingChannel)`, no named bool).
  * BUT retail reloads Meshes.begin after the channel block (`mov ecx,[ecx+28h]` 0x007E17A1, PDef.Object still in ecx, offset in edx) and ours
    keeps the entry pointer in eax (960v963, 47 hunks: loop-2 offset slot 10h v 1Ch, channel slot 18h v 10h, registers).
Probe facts (probe/*.cpp; NOT landable, they make the visible ctor store `&theme` in a global to force exposure):
  * probe/p_theme.cpp (ctor visible, `theme` captured, `pos` not): 967v963 with ONLY the old residue 1 left (3 byte stores before the flag
    load v retail's flag load + 2 stores). So retail behaves as "position temporary not exposed, channel exposed for the begin reload".
  * which store kills the cached begin: the 4-byte struct copy `channel = tc_appearance.PeekLightingChannel()` alone (k2); the byte stores
    do not (k1). Which loads keep the dead byte store alive: any 1-byte load through a LOADED pointer (entry.X, PrimHandles[i], *draw_info.Pos);
    loads through a parameter (draw_info.X, tc_appearance.X) do not (m1 / m6). Wide-typed lc spellings / inline accessors / an inline
    by-value or by-reference filter predicate / ctor member-init v body assignment: no byte change.
  * The contradiction left: retail needs the struct copy to kill (channel exposed) AND the entry loads not to keep the byte store alive
    (channel not exposed). One of the two has another cause that I did not find. Ideas not tried: the real ctor text (Lit is a 1-bit
    bitfield at +49h in retail, `xor/and/xor` 0x0077BE01; the row's local class model has a plain byte); the static-mesh class as a shared
    header with the landed 0x0077BD70 text; an inline helper that returns the channel by value (hidden result slot = `channel`).
  * The sibling passes `&tc_appearance.LightingChannel` straight to the ctor and tests the filter on a 4-byte register copy
    (`mov eax,[esi+40h]` 0x0077BACD between the global test and `jl`): the filter test is probably an inline predicate taking the
    descriptor BY VALUE (same bytes as the open-coded test in this row, so not decisive here).
FITTED in the drafts: the `entry` reference used only for the first three members (C37). DECISION for the coordinator: a row source that
defines the engine ctor 0x0077BD70 above the row needs the landed ctor text on the row's class model (the landed row uses local classes).

## 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading -- PARKED, DIFFER(1269v1265), 58 hunks (7 grades here; C27 / C37 before)
Start state: prev regraded 1269v1265 / 58 hunks. differ/009fde80.cpp = prev unchanged.
Reading of the residue (measured on the hunks): the code is equal up to 0x009FDFC0 except ONE inserted spill `mov [esp+10h],ebx` (0x009FDE99):
in ours `header` has a memory home and ebx only caches it; ebx then goes to `depth` (0x009FE021, retail: edx -> [esp+40h]) and to
mipmap_level for the allocation loop (loaded before 0x009FE087, written back 0x009FE0D8); retail keeps header in ebx to its last use
(`add ebx,18h` 0x009FE202) and re-reads mipmap_level from its parameter home three times per loop iteration.
Tried here, no gain: GFLimitUpper<long,long> (FableWin ??$GFLimitUpper@JJ, inline, value by reference) for the two max_mipmap_level
clamps = SAME bytes as the plain ifs (so it is a free, evidenced respelling: retail homes max_mipmap_level at [esp+10h]); the same helper
for the mipmap_level clamps (67 hunks: fastcall argument register artefacts); offset loop unsigned (60), unsigned + `size -=` first (67),
depth declared first (1255, as C37). An inlined by-reference use does NOT pin a variable to memory (measured twice in this batch).
Not tried: what makes `header` a weak candidate -- it is `GraphicInfoFrame->GraphicInfo` + 0 (PeekGraphicHeader returns the member at +0,
`mov ebx,[edi+0Ch]`); a `const CGraphicInfo*` local instead of the header reference, or the header fetched again per use.

## 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision -- PARKED, DIFFER(1643v1648), 54 hunks (22 grades / probes here)
Start state: prev regraded 1643v1648 / 54 hunks; differ/007bfe30.cpp = prev unchanged.
FIRST WRONG FACT (new, measured): retail loads weaponDef.Object ONCE (`mov edi,[esp+28h]` 0x007BFE70, right after GetSubDef returns) and
uses edi for `[edi+0C6h]` and for the function-end release 0x007C0483 across ~25 calls with no reload and no null store: in retail
`weaponDef` is NOT address-exposed although `&weaponDef` went to CDefClassBase::GetSubDef<CWeaponDef> (0x0042B1A6, a template COMDAT).
Ours reloads it after the base-def release call (`mov ecx,[esp+28h]` 0x007BFE9A): exposed. The whole register permutation (this ebx v
ebp, zero ebp v edi, the && result in memory v bl) follows: with weaponDef.Object as a function-long edi web, `this` takes ebx.
What I measured about c2's callee scan (same mechanism as row 0x007E15C0 / 0x007EA580):
  * a plain function defined above the row that only STORES through the out pointer leaves the caller's variable unexposed (probe P2);
  * the real GetSubDef text leaves it exposed whether it is the header template (implicit), explicitly instantiated above the row,
    instantiated by an earlier function, or written as an explicit specialization above the row (S1): the SetDynamicCast / Assign /
    Release chain on `def` (compare, release of the old pointee through a virtual call, store, AddRef) is what c2 does not see through
    (S2 / S4: still exposed with only that chain; small bodies were expanded and tell nothing). FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD:
    no byte change. TU flag /Oa: 1645v1648 but 94 hunks (also tried on 0x007E15C0: 993, 0x009FDE80: 1248) -> not a whole-TU /Oa.
Next ideas (not tried): which statement of the Release / Assign chain exposes the pointer (bisect with noinline stand-ins: the old
pointee's virtual Delete call is the suspect); the other CDefPointer Release texts (RELEASE_POINTEE / RELEASE_COUNTREF opt-ins) inside
GetSubDef; a GetSubDef text that builds the result in a local CDefPointer and assigns once.

### 0x009FDE80 / 0x007E15C0 -- successor's checks (no source change; 20 grades / 1 priority trace on 0x009FDE80, 1 regrade on 0x007E15C0)
* Call identities (tools/idc.py: our call relocation names v retail's direct call targets through reloc_bindings.tsv):
  0x009FDE80 call 24 targets 0x009FC3D0, BOUND `?AddResource@CResourceBank@@QAEXPAUCResourceBase@@@Z`; the draft calls it
  `?AddToLoadingQueue@CResourceBank@@QAEXAAVCResource@@@Z` -> must be respelled to the bound symbol before the row can land (NAME_FIXES).
  Unbound (new names, need two sources): 0x00A69C20 AllocateVolumeTexture, 0x00A69B70 AllocateTexture (0x009FDE80); 0x004BC340
  RefreshPrimitivePos, 0x0077BD70 CEnginePrimitiveStaticMesh ctor (0x007E15C0). 0x007E15C0 regraded: 963v963 / 18 hunks, unchanged.
* 0x009FDE80 c2 colouring (tools/pr.sh .. BeginLoading): this 230 esi, width 136 edi, height 136 ebp, then `depth` (priority 82, 4 refs)
  takes ebx BEFORE `header` (79, 12 refs: split ebx / eax) and mipmap_level (78, byte-only: shift count). RETAIL: header keeps ebx whole,
  depth is computed FIRST of the three (`movzx edx,word [ebx+4]` 0x009FE016, then width edi, height ebp), shifted in edx and stored to its
  frame home [esp+40h] at 0x009FE032; mipmap_level is re-read from its parameter home (`movzx eax,word [esp+48h]`, also in ours).
  REQUIRED: prio(header) > prio(depth) (gap 3), i.e. depth must lose the callee-saved registers. Not the lever: declaring depth before
  width / height (1255, 78 hunks) or anywhere earlier in the function (sweep of every statement position: best 1265v1265 / 45 hunks before
  `bank = GetBank()`, but that MOVES the computation and the frame becomes 0x30 v retail 0x34 -> wrong). Next idea: a spelling in which depth
  has fewer references in its block or header more (e.g. `header.MipmapLevels - mipmap_level` / TextureType reads through `header` named
  earlier), read with `tools/pr.sh --blocks`.

### 0x007BFE30 -- successor's measurements (12 more grades / 5 c2 priority traces; differ/007bfe30.cpp now 1643v1648 / 52 hunks)
* The predecessor's "weaponDef is address-exposed in ours" reading is NOT the lever. Probe on the matched row 0x00735FD0 (same
  GetSubDef<CWeaponDef>): with the header body visible (either CDefClassBase_GetSubDef.h or FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) the
  out handle is NOT exposed (RELOCATION_MATCH); without a body 1013v973. This row includes CDefClassBase_GetSubDef.h, so the handle is not
  exposed here either: ours reloads Object after the release call only because it sits in ecx (no callee-saved register won).
* c2's own colouring (tools/c2prio_f.py = wfcommon C41 tool, `tools/pr.sh <src> UpdateCollision "<names>"`, 16 s): `this` has priority 56 and
  is coloured at step 9 with allowed {ebx, ebp}; ebx costs 5100 (byte-only neighbours: the two `&&` result temps, the destruction-flag word,
  useExtendedTrails) so it takes ebp; hitLocations (priority 52) is next and is left with ebx. RETAIL = the two swapped: hitLocations is
  coloured first (ebx penalised -> ebp, 0x007C01E8 `mov ebp,[eax+4]`), then `this` has only ebx left (0x007BFE3F `mov ebx,ecx`); with ebx
  gone the first `&&` temp stays in memory ([esp+12h], 0x007BFE7E), const 0 takes ebp at entry and weaponDef.Object gets edi.
  REQUIRED INEQUALITY: prio(hitLocations) >= prio(this) (a tie goes to the larger key = hitLocations). Gap today: 4.
  Where the numbers come from (--blocks): hitLocations +40 (block of the GetTC store / find call, line `if (thing->GetTC(..) && std::find`,
  w 2, K 10) +40 +28 +20 (the three intersection calls) and -28 for the loop-top blocks it is live through (it is live into the loop
  uninitialised, as in retail: `mov ebp,[esp+70h]` at the preheader 0x007C015E); `this` +120 in that same K-10 block (3 references:
  HitThingsList.begin / end / end). One candidate fewer in that block (K 9) gives this 44 v hitLocations 48 = retail's order; so does any
  change worth 4 points. Not found: which source fact it is (one-argument GetTC(&tc) for both components = FableWin's only instantiations,
  byte-identical, kept; declaration of hitLocations at loop scope: same).
* EVIDENCED and kept: declaration order `closestDistanceSquared` before `closestThing` (retail store order 0x007C0144 / 4C), 54 -> 52 hunks.
* differ/007bfe30_ignore_first.cpp: `CThing* ignore = ParentThing;` declared before `shooter` gives 49 hunks but raises prio(this) to 66.
* The residue is NOT a pure permutation: x87 operand order at 0x007BFF54 (`fld [esi] ; fadd [eax+0Ch]`, ours the reverse), the store of
  [esp+44h] scheduled later in retail (0x007BFFA3, 0x007C002D), the magnitude term order at 0x007C0089, `add edi,eax` 0x007C00AA. These
  are expression-shape facts of the centre / line / box block (lines `centre = (last.first + prev.second) * 0.5f` ..) still to be worked.
* Not levers (measured): FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY instead of the template header (1655, 82 hunks: different Release
  text at the exit), GETSUBDEF_NOINLINE (no change), OnCollision without ParentThing (1637), split pause test (1649), swapped rolling test.

## 0x00735FD0 CTCCombat::AttemptToStartParticle -- MATCHED (RELOCATION_MATCH, test PASS, addmatch done, vland WINS; 9 grades by the successor)
Name: Ego_r + FableWin ?AttemptToStartParticle@CTCCombat@@AAEXPAVCThing@@@Z, reloc_bindings 00735fd0. Start 993v973 / 72 hunks (register permutation).
What closed it (each step measured):
  1. EVIDENCED `IS_THING_ALIVE(FlourishParticles[i]) && IS_THING_ALIVE(blend_thing)` (engine/ThingAlive.h, FABLE_RETAIL_IS_THING_ALIVE +
     FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT; FableWin ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV..@Z and
     ..@PAVCThing@@@@): retail takes the element address ONCE for both GetPItem calls (0x0073619C..0x007361B3) and re-derives it for the
     GetTC after them, and blend_thing lives in a frame home (+0x1C): both are the by-reference template. This alone removed the whole
     register permutation (72 -> 30 hunks): weapon ebp, this / i edi with home +0x20, emitter ebx, the 0x800 mask in edi.
  2. EVIDENCED shared engine/CParticleEmitter.h (FABLE_RETAIL_CPARTICLEEMITTER + _INTERPOLATE) instead of the draft's local class model:
     `blend_particles->Sleep()` (`or byte ptr [edi+41h],2` 0x00736273) and `particles->SetAlwaysUpdate(true)` (load / or 10h / store 0x007362F5).
  3. EVIDENCED `std::max(range, DELTA_NOTIONAL_ZERO)`: retail takes the address of the global 0x0129BA3C (reloc_bindings
     ?DELTA_NOTIONAL_ZERO@@3MB, `mov eax,129BA3Ch` 0x007362D2), not a stack literal -> frame 0x58 (30 -> 2 hunks).
  4. FITTED-with-evidence: no `set` reference local; `weapon_def->FlourishParticles.<member>` spelled at each use. With the reference c2
     keeps the null test of weapon_def.Object in the two early-return releases; retail has none there (0x00736373 jumps past the test to
     0x0073634B; 0x00736387 `dec [ebx+4]` directly).
Purity gate: a local named `blend_emitter` contains `_emit` and vland says NOT_GENUINE: renamed blendEmitter.
vland: WARN RELOC_NAME only (0x0129BA3C bound ?DELTA_NOTIONAL_ZERO@@3MB, another label table says ?g_NormaliseEpsilon@@3MB; the bound name is the
PDB one used by landed rows). Test: model of the logic, 6 scenarios (no def / two pairs incl. dummy fallback and DELTA clamp / missing end /
missing start / effect 0 / dead things).

## 0x006823D0 CTCCreatureOpinionOfHero::FrameUpdate -- MATCHED (RELOCATION_MATCH, test PASS, addmatch done, vland WINS; ~35 grades / probes by the successor)
Name: vtable slot 9 (header) + Ego_r / FableWin ?FrameUpdate@CTCCreatureOpinionOfHero@@UAEXXZ. Start 960v962 / 34 hunks (frame 0x18 v 0x14).
What closed it, in order (each measured):
  1. FITTED-natural `float over = GFFloatAbs(RelativeOpinion[op]) - 2.0f;` then `GFFloatAbs(over / RunOut)` (instead of a named `peak`
     copy of the abs): the subtraction is deferred to its single use, the abs result stays in the helper's own parameter home
     (+0x18, `fld [esp+18h] ; fsub` at 0x00682558) -> retail's frame 0x14 with the FableWin GFFloatAbs text (34 -> 20 hunks).
  2. IDENTITY (masked by q.py, would have failed the lander): `ParentThing->PeekTC(&perceive)` with `const CTCPerceiveThing*` (retail
     calls the CONST LowerBound 0x004365B0 at 0x00682456); `DELTA_NOTIONAL_ZERO` global 0x0129BA3C instead of the 0.0001f literal
     (`fcomp [129BA3Ch]` 0x00682519 / 0x0068254B).
  3. EVIDENCED `GFLimit(RelativeOpinion[offset.Opinion], -2.0f, 2.0f)` (engine/GFLimit.h, both PDBs ??$GFLimit@MMM@@) for both clamps
     instead of the hand-written if / else-if: the element address is formed once per clamp (20 -> 5 hunks; the temp-register "rotation"
     of the clamp and run-out blocks was this). `-2, 2` (GFLimit<float,int,int>) is byte-identical: float literals kept.
     `RelativeOpinion[op]` through the new opt-in FABLE_RETAIL_COPINIONSET_INDEX (FableWin ??ACOpinionSet@@QAEAAMJ@Z) is byte-neutral v
     `.OpinionValues[op]`; kept because it is the PDB accessor (ONE source: FableWin only).
  4. FITTED `long fps = CMainGameComponent::GetConstantFPS();` named before the deed-sweep modulo: retail loads UniqueID.lo, ConstantFPS,
     UniqueID.hi in that order (0x00682651 / 54 / 5A); the nested call loads lo, hi, fps (5 -> 3 hunks).
  5. EVIDENCED two Kill statements, `if (stale) P->Kill(true); else if (perceived...) P->Kill(true);` instead of one `||` condition:
     retail's merged block starts `mov ecx,ebx ; push 1 ; call GetPItem` (0x006824A4) -- the ONLY one of 139 `P->Kill(true)` sites in
     Fable.exe with the mov before the push; it is the cross-jump of two copies (the single statement gives push ; mov).
Header: hdr/engine/CTCCreatureOpinionOfHero.h + FABLE_RETAIL_COPINIONSET_INDEX (README). No other matched row includes that header.
Test: model of the four parts (source kill / transient offsets incl. overshoot conversion / deed sweep period / times-hit decay), 20 checks.

## Read-only vland (successor, all four matched rows): WINS 4 -- 007bc38a, 007ea580, 00735fd0, 006823d0 RELOCATION_MATCH PASS; no RELOC_IDENTITY; WARN RELOC_NAME only (0x0129BA3C label table says g_NormaliseEpsilon, bound name ?DELTA_NOTIONAL_ZERO@@3MB).

### 0x009FDE80 -- third agent (wfB5 rerun, 2026-11-15): 1269v1265 / 58 -> 1265v1265 / 40 hunks (differ/009fde80.cpp)
* EVIDENCED: the c_mipmap offset loop is unsigned (`for (unsigned long c_mipmap = 0; c_mipmap < mipmap_level; ...)`): retail `test ecx,ecx ; jbe`
  0x009FE1FE and `cmp eax,1 ; jae` 0x009FE210; ours had jle / jge. Same length, fixes the jcc kinds.
* c2 priority (tools/pr.sh, --blocks): prio = sum over referencing blocks w*K*cost minus w*K per live-through block. header 79, depth 82
  (depth only in blocks 30/31, which header shares). Header gains +w*cost = +4 per extra candidate in the LOOP BODY block (line
  `if (texture_manager.AllocateTexture(...))`, w 2, K 6). FITTED: `long levels = header.MipmapLevels - mipmap_level;` as a named local in
  the loop body -> header 83, ebx for the whole function, depth / mipmap_level to memory as retail; length 1265 = retail. A named bool
  for the AllocateTexture result is forwarded (no change); `success = Allocate..` changes code (1249). `long depth = 0` early, a pointer
  header, a named graphic_info, no named bank, unsigned depth: no priority change.
* Left (40 hunks): frame 0x30 v retail 0x34. tools/pk (C15 packer probe, 7 s) + tools/slots.py: ours lets `entry` (CreateEntry result,
  8 bytes) JOIN depth's slot; retail keeps depth alone at the top (frame -4, 0x009FE032 / 0x009FE05C) with new_texture at -12, entry
  -20, texture -28. Loop body: retail computes levels in ecx (ours eax, the named candidate is coloured first); c_mipmap loop eax/ecx
  swapped and retail does `sub ebp,ecx` (size) before `add edi,ecx` (offset) -- source order `size -=` first swaps size/offset registers.

### 0x007BFE30 -- third agent (wfB5 rerun, 2026-11-15): unchanged 1643v1648 / 52 hunks (~20 grades / 2 priority traces)
* Confirmed with --blocks: our code is instruction-for-instruction retail's around the loop (GetTC / find / pause checks), only
  this (ebp) and hitLocations (ebx) swapped. Block 37 (line `if (thing->GetTC(&hitLocations) && std::find(...)`, w 2, K 10) holds
  the three inlined find-forwarder parameter locals (#55/#56/#60), temp #57, the shared && temp #40 (lines 89 and 130, byte-only,
  memory), thing, the CIntelligentPointer vtable constant, const 2 (destruction flag), this (cost 6) and hitLocations (cost 2): every
  one of them has a retail counterpart, so K-1 there is not a free respelling. Swing per extra candidate in the trail-intersection
  blocks 44 / 45 / 48 (hitLocations referenced, this live through) is +6 for hitLocations: any natural extra candidate there would
  close the gap of 4 -- none found (no unexplained retail register in those blocks).
* Retail facts recorded for a successor: GetThingManager() is called twice through `this` (0x007C0174 / 0x007C0186, matches ours);
  the line magnitude is summed X*X + Z*Z + Y*Y (0x007C0077..0x007C0093) and the centre sum loads last.first first (0x007BFF54): these
  are c2 operand-sort orders (record ids), expected to move once the colouring is right.
* Device sweep (owner's acceptance test c): k = 1..7 unused named locals at the top (`long rN = 0;` and `CThing* qN = ParentThing;`):
  51..54 hunks, length unchanged, no closer -- the residue is a priority gap, not a record-count tie.
* (cont.) Best now 1265v1265 / 39 hunks: `levels` named in BOTH branches (volume call too: symmetric, one fewer hunk). Frame: tools/pk
  shows ours packs items in sorted order (sizes 1/4 then 8) and slots are laid out bottom-up in creation order; retail's layout puts
  depth's slot LAST (above new_texture), i.e. in retail depth is not an ordinary packer item but a slot made after packing (a spill
  temp of a split candidate?) -- ours spills depth into its own home item (weight 2), which `entry` then joins. Not closed: depth as
  unsigned / const, levels at function scope, declaration orders of depth / success / texture: no frame change.
  Retail computes levels in ecx (ours eax: the named `levels` is coloured first, prio 112): a hint that retail had no named levels and
  the +4 for header comes from another source fact still unknown. `levels` stays FITTED.
* NAME (corrects the earlier wfB5 NAME_FIXES line): 0x009FC3D0 works on this+0x40 = LoadingQueue -> AddToLoadingQueue(CResource&),
  exactly the draft's spelling; the reloc_bindings "AddResource" names come from landed local stand-ins (0x00A42980). NAME_FIXES.tsv fixed.

### 0x007E15C0 -- third agent (wfB5 rerun, 2026-11-15): unchanged (differ/007e15c0.cpp 963v963 / 18; visible-ctor variant 960v963 / 47); ~12 grades
* Reading sharpened: in retail the flag load `mov bl,[eax+2Ch]` is scheduled ABOVE the two default-ctor byte stores and the index-0
  store is dead-store-eliminated: both need `channel` NOT address-exposed at that point (an exposed local pins 1-byte loads through
  loaded pointers below its stores, measured). So the Meshes.begin reload at 0x007E17A1 cannot come from channel exposure; some other
  kill or a CSE miss between 0x007E1705 and 0x007E17A1 is the open fact.
* Tried on the visible-ctor variant (all byte-identical, 960v963 / 47): by-value inline filter predicate, a by-reference inline setter
  for ChannelIndex0, a `CEnginePrimitiveThemeChannelDesc& theme = channel` alias for both arms, all `entry.` uses spelled
  `PDef->Meshes[i].` (the CSE still happens). On the exposed probe (p_theme): a named bool for the override flag declared before
  `channel` -> 971v963 (the index-0 store survives because the LightingChannel byte load in the if-arm may alias the exposed channel).
* Ideas left: what in retail kills or defeats the CSE of `PDef->Meshes._M_start` across the channel diamond while PDef.Object stays
  in ecx (a this-relative load is never killed by local-store aliasing, a pointer-relative one is); the real header text of the
  static-mesh ctor on a shared header (the landed 0x0077BD70 row is a local-class model, so a row that defines it needs a header
  first -- coordinator decision already recorded above).

### 0x009FDE80 -- fourth agent (wfB5 successor 2, 2026-11-15): best now differ/009fde80.cpp = 1265v1265 / 40 hunks but 14 register-structure hunks (was 21)
* EVIDENCED: the VOLUME call passes `header.MipmapLevels - mipmap_level` inline (no named levels): that branch 0x009FE044..0x009FE07F is
  then byte-exact (retail `sub edx,[esp+50h]`). Previous best (levels named in both) kept as differ/009fde80_prev_levels_both.cpp (39 hunks,
  21 register-structure hunks). tools/rx.py = diff with esp offsets and immediates masked (pure register/opcode structure).
* Diagnostic what-ifs (tools/fw.sh, C2_FORCEPAIR, NOT a hand-back device) on v/E1 (= inline levels in BOTH calls + `size -= skip` before
  `offset += skip`): forcing header 79->83, offset 58->59, c_mipmap 78->84 leaves ONLY 3 real hunks: (a) loop increment: retail loads
  mipmap_level into ecx right after the AllocateTexture test and compares with max as a memory operand (`inc ecx; cmp ecx,[esp+14h]`),
  ours loads max into ecx and does mipmap in eax; (b) the loop-alignment pad that follows from (a); (c) frame 0x30 v 0x34. So the true
  source differs from E1 by facts that give: header >= 83 (depth 82 wins ties on key), offset > size, c_mipmap >= skip_size (c_mipmap's
  key 330 > 326 wins a tie), and the loop-increment piece. Frame stays 0x30 under every forced priority set tried (depth 40/70/150,
  mipmap 85, max 100): the frame is not a colouring question.
* c2 block arithmetic (tools/c2prio_f.py --blocks): depth only lives in blocks 30/31 (costs equal to header's there), so only header-only
  blocks move the gap; header loses -w*K in ~30 live-through blocks. c_mipmap v skip_size: the gap is block 64 (`skip = CMS[c]`, K 2:
  skip +8, c_mipmap -4) v block 59 (+10 c_mipmap). offset v size tie everywhere (block 58 both cost 6).
* Tried, no priority change (prv.sh, 16 s each): depth as `depth = header.Depth; depth >>= m`, (long)(..), unsigned, const; loop levels
  casts (4 spellings); skip_size as ?:, nested if, compressed_size local, c_mipmap declared outside; offset declared first, offset in an
  else branch (code change), `size * FrameNum`; GetSmallMipmapLevel() accessor (FableWin ?GetSmallMipmapLevel@CGraphicInfoFrame@@QBEJXZ
  18 bytes = `return SmallMipmapLevel;`, byte-neutral, opt-in NOT kept); scope blocks around the allocation / entry (frame unchanged).
  while-loop forms change the loop shape (1233 / 1205 bytes). An inline identity around depth gives frame 0x34 but only because depth
  then wins ebx (diagnostic only).
* Open: the source fact behind the three priority gaps + the loop-increment piece (they may be one fact: all three sit in the success /
  loop region), and the frame (retail depth slot made outside the packer item list: in ours depth's split leaves the parent with a home
  item that `entry` joins).

### 0x007BFE30 -- fourth agent (wfB5 successor 2): 52 -> 18 hunks, 1647v1648 (differ/007bfe30.cpp; previous best kept as differ/007bfe30_prev52.cpp)
* CLOSER (EVIDENCED): retail compiles GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind3<..> > at 0x007C2D10 in this TU
  (the row's own notes say the filter ??R is compiled with it), so its template BODY is visible to the row: define
  FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (+ CTCMAPWHO, CTCMAPWHO_SEARCHID, CTCMAPWHO_STATTYPE, CTHING_STATTYPEFLAGS, C2DBOXI and the
  CTCMapwho.h / NThingMap_CThingMap.h includes, as landed 0x007A0D90 does). The this/hitLocations colour swap disappears (reg-structure
  hunks 58 -> 27). Same lesson as wfB5 0x007EA580 / 0x00969E70: c2 scans visible same-TU callee bodies.

## 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision -- MATCHED (RELOCATION_MATCH, test PASS, addmatch done, vland WIN; fifth agent, ~45 grades)
Start: differ/007bfe30.cpp 1647v1648 / 18 hunks. What closed it, in order:
* FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ (EVIDENCED: carried by landed same-TU sibling InitMovement 0x007BC6FA): line magnitude
  loads X, Z, Y (0x007C0077..0x007C008F). (The other same-TU sibling OnCollision 0x007BF510 carries _ZYX: the landed rows disagree.)
* Hit distance: retail keeps all three differences on the x87 stack and sums Z*Z, Y*Y, X*X from copies, then pops three
  (0x007C0399..0x007C03AD); `(hitPos - prev.first).GetSquaredMagnitude()` (any header text, named or not, XYZ_FLAT probe opt-in,
  Dot(offset): 1 hunk) consumes them. FITTED: `C3DVector offset = hitPos - prev.first; float distanceSquared = offset.X * offset.X +
  offset.Y * offset.Y + offset.Z * offset.Z;` -- also fixed the centre X operand order 0x007BFF54 (record numbering). 18 -> 13 hunks.
* Box: `C2DBoxI area(centre.ToC2DCoordI(), radius)` (EVIDENCED: FableWin.pdb ?ToC2DCoordI@C3DVector@@QBE?AVC2DCoordI@@XZ, landed
  0x007A0D90 uses it) instead of `C2DCoordI((long)centre.X, (long)centre.Y)`: closes the box corner order (`add edi,eax` in place,
  0x007C00AA) AND every remaining scratch-register hunk (Bind3 stores, pushes, 0x007C0162, 0x007C01A1, 0x007C01B9): RELOCATION_MATCH.
* Call identity (q.py masks it): the trail lookup is the const PeekTC (retail calls const LowerBound 0x004365B0 there; the hit-locations
  GetTC calls the non-const 0x0040F020): `const CTCWeaponTrail* trail; ParentThing->PeekTC(&trail)`. Byte-identical, identity fixed.
* Not levers (measured): k = 1..7 unused locals (long / float, top or block): 17..20 hunks; C2DBOXI_RADIUS_INITIALIZERS (identical);
  ADD_XY_GROUP / SUBTRACT_XY_GROUP / SCALE_X_PAREN / SCALE_NAMED_RESULT / SQUAREDMAGNITUDE_ZYX / MAGNITUDE_XY_THEN_Z.
* vland: WARN RELOC_NAME ??0?$CArray@VC3DVector@@@@QAE@I@Z -> 0x007C2AB0 labelled CSTLportVector (NAME_FIXES line added; FableWin PDB
  has only the @I@Z size ctor). The anonymous-namespace filter hash in the GetAllThingsInRoughArea symbol differs from Ego_r's
  (?A0x6dcc4763), unavoidable per file. No header change (a probe opt-in C3DVECTOR_SQUAREDMAGNITUDE_XYZ_FLAT was made and removed).

### 0x007E15C0 -- fifth agent (2026-11-15): unchanged, parked (differ/007e15c0.cpp 963v963 / 18; visible-ctor 960v963 / 47); ~10 grades
* Retail ctor 0x0077BD70 read: every base / member init is inline (CEnginePrimitive base: type 8, interpolation set, a zeroed theme
  desc at +8..+0xA), `theme` and `pos` are both COPIED (theme `mov ecx,[edx] ; mov [eax+40h],ecx`), nothing is captured: with the real
  body visible c2 has no reason to expose `channel`, so the retail Meshes.begin reload 0x007E17A1 needs another cause.
* fwsize: ??0CEnginePrimitiveThemeChannelDesc@@QAE@XZ ours 14i/34b v FableWin 13i/32b (d -1i -2b; the 3-member init list). Probe texts
  (body chain assignment 14i/34b, two members 12i/28b, short store + one member 12i/29b, four members 16i/41b): none 13i/32b. Open
  hint only (FableWin is a later build); the probe opt-ins were removed (no header change).
* c2prio on the visible-ctor variant: the entry CSE temp (#72, lines 98 / 109-110) has prio 48 and eax; forcing it to -60 still gives
  eax (spill > 0, free registers), so the reload is not a colouring loss; c2 does not un-CSE.
* Tried, no gain: channel via the 3-arg ctor in the if-arm (960 / 51), ?: initialisation (977), 3-arg with own members (960 / 51);
  `unsigned char lighting_channel` named before `channel` (e0 base 966 / 29, visible 960 / 47); an inline helper returning the channel
  by value (975 / 40 and 975 / 52).

### 0x009FDE80 -- fifth agent (2026-11-15): unchanged, parked (differ/009fde80.cpp 1265v1265 / 40); ~9 grades
* Device sweep (owner's test c): k = 1..7 unused `long rN = 0;` after the header binding: 40 hunks every k (priority gap, not a tie).
* Frame fact sharpened: retail `push ebp` at 0x009FDFF2 is the late callee-saved push, so locals are 0x34 bytes and depth's home
  [esp+40h] at 0x009FE032 is the TOP local slot (local offset 0x30); ours 0x30 bytes with depth at local 0x18. A top slot made after
  packing fits a split / expression temp rather than a named home. `header.Depth >> mipmap_level` written into the volume call (no
  depth local): 1238 / 76 (c2 does not hoist it above the GetTextureType call) -> retail computes it before the branch from a source
  statement there.
* Region 0x009FDFF2..0x009FE0DF is otherwise instruction-equal except slot offsets and the loop increment (`mov ecx,[esp+48h]` after the
  AllocateTexture test, `inc ecx ; cmp ecx,[esp+14h]`); retail then zero-tests with `xor edi,edi ; cmp eax,edi` 0x009FE0E0 (const 0 in
  edi), ours `test eax,eax`.

### 0x007E15C0 -- sixth agent (2026-11-15): unchanged, parked (differ/007e15c0.cpp 963v963 / 18); ~11 variant grades
* Global 0x013756F0 facts: .data, initialised -1, read only at 0x0077BAC7 and 0x007E1789, no code or data writer (debug-tweak global).
  `extern volatile long` for it: byte-identical on base and visible-ctor variants (not a CSE barrier) -> no volatile.
* NEW measured probe (differ/007e15c0_named_flag_lc.cpp, 962v963 / 37): extern ctor (channel exposed) + `bool override_channel` AND
  `unsigned char lighting_channel` read BEFORE `channel` (lesson 0x00BA6240): the dead index-0 store disappears, the +1/+2 stores follow
  the flag load, the Meshes.begin reload at 0x007E17A1 STAYS (retail's), but the lc load is hoisted above the branch (retail loads it
  inside the arm at 0x007E1727) and the loop-3 offset IV moves edx -> ebx. Flag alone (b1): 967 / 32 (+0 store kept); lc alone: 966 / 29.
  So under exposure the only open fact is "lc load inside the arm while the +0 store is dead"; under non-exposure it is the reload.
* Frame-slot fact: channel sits at [esp+10h] (bottom slot) in retail and in the exposed base, at [esp+18h] in the visible-ctor variant:
  a second hint (with the reload) that retail's channel IS address-exposed; the DSE / flag hoist are then the facts without a source.
* Visible-ctor variant, all byte-identical (960 / 47): by-reference inline filter predicate, `unsigned char Lit : 1` (the landed ctor's
  bitfield), a `const CEnginePrimitiveThemeChannelDesc& theme = channel` bound after the filter and passed to the ctor.
* Base: `channel = CEnginePrimitiveThemeChannelDesc(lc, channel.ChannelIndex1, channel.ChannelBlendFactor)` in the arm: 977 / 49.

### 0x009FDE80 -- sixth agent (2026-11-15): unchanged, parked (differ/009fde80.cpp 1265v1265 / 40); 3 variant grades
* Comma-increment loop forms `for (; m <= max; ++m, width /= 2, height /= 2)` (and halving first): byte-identical to the best (40);
  with the volume-style inline levels in the loop call too: 1269 / 60. The loop-increment piece (retail loads mipmap_level into ecx
  right after the AllocateTexture test, max as `cmp ecx,[esp+14h]`) is not a statement-order fact of the increment expression.
* Read: StopLoading 0x009FDC20 is out of line in the same TU and NOT called by this row (the abort block is open-coded / inline here);
  the row's only same-TU callee is CGraphicFrameData::Sizeof 0x009FCBF0 (through the inline CFrameData::Sizeof); not yet tried visible.

### 0x007E15C0 -- seventh agent (2026-11-15): unchanged, parked (differ/007e15c0.cpp 963v963 / 18); ~25 grades / probes (wk7/)
* New helper-text level: FableWin has ?Interpolate@C3DVector@@QBE?BV1@ABV1@M@Z and fwsize says ?GetInterpolated@C3DPositionInterpolator
  is 19i/45b in FableWin v ours 29i/73b, i.e. the real text is `return StartPosition.Interpolate(EndPosition, t);` (one call, ~18i/42b
  measured by hand). Spelled through the existing C3DVector::Interpolate (FABLE_RETAIL_CPARTICLE_SPRITE): byte-identical on the base and
  the visible-ctor variant (wk7/*_I.cpp) -- evidence for a future GetInterpolated opt-in, no byte effect here.
* EXPOSURE FACT (measured): a DEAD arm in the visible-ctor variant that passes `channel` to an extern function
  (`if (0) { ProbeCheckChannel(channel); }`, wk7/d1-d3, any position) or stores &channel to a global (e2) exposes channel for EVERY
  pass -> exactly the p_theme 6-hunk state (reload of Meshes.begin + bottom slot + retail x87 are right; only the dead +0 store and the
  flag-after-stores remain). A dead local pointer (e1) or a dead copy (e3) does not expose. So a release-dead assert whose condition
  passes channel to an out-of-line function would be the landable form of the 6-hunk state (device d), if a second fact closes DSE.
* DSE facts (exposed, p_theme config): the +0 ctor store is kept by ANY load through `entry` between it and the arm store -- the flag
  load (q1: param-based lc, flag through entry: kept) and the lc load (t1 named flag before the decl: kept), byte or dword (t4 `entry.Mesh>>3`,
  t5 movzx word: kept). Only loads through parameters / this do not block (m1). A POD local with late address-taking (u2) is identical.
* fwsize: ??0CEnginePrimitiveThemeChannelDesc@@QAE@XZ 13i/32b not reproduced by 11 ctor texts (3/2/4 members, chain, body, short/dword
  stores, Clear() call); 13i/32b = 3-arg prologue/epilogue + a 5i/18b body (2 byte stores + a 3i/12b member init, e.g. from a load).
* (cont., 007E15C0) Field / flag levels also measured: a dead arm passing only &channel.Pad / &channel.ChannelBlendFactor /
  &channel.ChannelIndex0 (wk7/f1-f3) exposes the WHOLE local (6 hunks, = p_theme): c2 exposure is not field-sensitive. Whole-TU /Ow:
  966/37 base, 965/26 visible, 970/27 dead-arm (worse); /Oa 993..997 (worse). So every exposure form gives "reload right, DSE wrong" and
  every non-exposure form "DSE right, reload wrong"; the retail combination needs a source fact outside both (not found).

### 0x009FDE80 -- seventh agent (2026-11-15): unchanged, parked (differ/009fde80.cpp 1265v1265 / 40, 14 reg-structure hunks); ~15 grades
* Device (c) re-swept at NEW positions: k = 1/2/3/5 unused `unsigned long rN = 0;` before the c_mipmap loop, k = 1/2 `long rN = 0;` before
  `success`: all byte-identical to the best (14 reg hunks) -- unused initialised locals leave no c2 record here.
* Loop forms: `max >= (long)mipmap_level` (15 reg hunks, jl/jge), `mipmap_level++` (identical), while with `++mipmap_level` last (1233).
* depth as `const long& depth = header.Depth >> mipmap_level` (reference to a compiler temporary, hoping for a post-packer slot), same
  with unsigned, and `long depth;` declared early + assigned after height: all identical (frame 0x30; c2 forwards the reference).
* PDB check: ?AllocateBestAvailableTexture@CTextureManager (Ego_r 0x00B7D490 / FableWin) is retail 0x00A69CE0 (uncalled, min-size loop):
  NOT this row's loop (the row calls AllocateTexture 0x00A69B70 per level).
* c2 trace (tools/c2prio_f.py --trace): mipmap_level is split (piece #76 ecx, prio 20, rest memory), max_mipmap_level piece #69 eax
  (prio 18); retail's loop increment wants the opposite in the increment block (mipmap_level piece in ecx, max a memory operand).
* (cont., 007E15C0) also measured: `entry` as a pointer variable (in-loop or function-scope, wk7/P1-P2) under exposure: identical (6);
  dead-arm exposure + named flag AND lc before the declaration (wk7/n1): DSE right but 966/40 (lc hoisted, offset IV moves to ebx).
  Best landable source unchanged: differ/007e15c0.cpp (963v963 / 18). Closest non-landable: wk7/d1.cpp (967v963 / 6, dead extern call).

### 0x007E15C0 -- eighth agent (2026-11-15): unchanged, parked (differ/007e15c0.cpp 963v963 / 18); ~20 grades / probes (wk9/)
* Device check (owner's test): dead release asserts / k unused locals cannot help: the residue is an alias-analysis contradiction,
  not a record-count tie. A dead arm that makes a CALL without channel (wk9/a1-a3, `if (0) { ProbeDeadCall(); }` after / before the
  channel block, visible-ctor base) is byte-identical (960 / 47): dead calls do not kill CSE; only exposure of channel does.
* Mechanism probes, all byte-identical on the visible-ctor variant (960 / 47): the ctor passing `theme` through an inline identity
  (t1: c2's capture summary is computed after inlining); the channel block as an inline helper taking `entry` as a PARAMETER (h1 on the
  dead-arm exposed d1: still 6 hunks, h2 visible: 47 -- inlined parameters are plain temps for alias analysis); the else-arm copy
  as memcpy / `*(long*)&channel = ...` / through a local pointer (m1-m3: c2 resolves them all to the local).
* Exposed (d1) configuration, other channel spellings: arm order swapped (r1) 967 / 7; copy first then 3-arg override (r2) 967 / 25;
  3-arg ctor in the if-arm (r3) 981 / 48.
* PROBE ONLY (not landable): a header copy whose default ctor leaves ChannelIndex0 uninitialised (probe/hdrN) + dead-arm exposure
  (d1) = 963v963 / 5 hunks: the only residue left is then the flag load scheduled after the two remaining byte stores (retail before).
  This contradicts evidence (landed SetUpPointLights 0x00BBC0B0 has three ctor stores; CEnginePrimitive base +8/+9/+A) and still needs
  the dead-arm exposure, so it is a measurement of where the residue lives, not a lead. A named flag on top of it (b1) breaks the
  register allocation (966 / 20).
* Named locals for the two interpolations (`CRightHandedSet orientation = ...; C3DVector position = ...;` before the ctor): visible
  variant 960 / 43 (from 47), extern-ctor base 963 / 24 (from 18). Not adopted (no gain on the landable base).
* FableWin size: OnAppearanceDraw 412i / 1516b v ours /Od+EH 269i (wk9/fwv.cpp) -> about two assert bodies in the debug build
  (FableWin is a VS2010-era build: STL _Iterator012 names), none of whose conditions could expose `channel` (the class has no
  out-of-line members in either PDB).

### 0x009FDE80 -- eighth agent (2026-11-15): 40 -> 39 hunks (differ/009fde80.cpp; previous best differ/009fde80_prev40.cpp); ~20 grades
* FITTED, measured: `level_size >>= 2` moved into the c_mipmap for-increment (`++c_mipmap, level_size >>= 2`) -> 39 hunks, 13
  register-structure hunks (the shr now sits in retail's increment run). With `size -=` first as well: 51; increment order swapped: 40.
* Device d (dead release asserts through fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM): k = 1, 2, 4, 8, 16, 32 asserts at the top
  (`FABLE_RETAIL_ASSERT(GraphicInfoFrame)`) and asserts naming loop locals (`size >= skip_size` in the c_mipmap loop, `mipmap_level <
  header.MipmapLevels` before it, `width > 0 && height > 0` in the allocation loop): ALL byte-identical (40). This row's residue is
  priority gaps, not record-count ties; no device of the acceptance list applies. FableWin BeginLoading 654i / 2614b v ours /Od+EH 385i
  (~4-6 assert bodies in the debug build), but dead arms leave no trace here.
* Also identical / worse: depth through an inline by-value identity (40), the three dimensions declared before `texture` (1251 / 48),
  depth declared just before `success` (72), skip_size declared outside the loop (40), c_mipmap declared outside (40), a
  `const unsigned long* compressed_sizes` local (1260 / 55), skip_size by if / else (1246 / 39, length off).
* Anniversary PDB (Xbox 360, weak source) locals of BeginLoading: async_entry (CCountedPointer<CBankFileAsyncEntry>), managed_texture
  (CManagedTexture), pixel_format (CPixelFormat), texture / texture2 (CCountedPointer<CTexture>), params mipmap_level, get_flags;
  scalar locals are not recorded. Names only (no byte effect); our spellings texture / format / entry / new_texture / flags differ.

### 0x007E15C0 -- ninth agent (2026-11-15): differ/007e15c0.cpp unchanged (963v963 / 18); NEW probe 963v963 / 5 (2 real lines) in differ/007e15c0_fade_exposed_probe.cpp; ~45 grades (wk10/)
* NEW FACT: the Meshes.begin reload at 0x007E17A1 does NOT need `channel` exposed. Visible ctor (differ/007e15c0_visible_ctor.cpp) + an
  out-of-line call receiving `&fade` in a dead arm (`if (0) {...}` or FABLE_RETAIL_ASSERT through fable_assert.h, any position, fade at loop or
  function scope: wk10/f1, m2, m6, g2, p1-p3) = 963v963 / 5 hunks: reload, channel block (no +0 store, flag load above the 2 stores), x87, every
  frame slot (channel at [esp+10h]) are retail. The 3 first hunks are the known phantom identical-text hunks; the ONLY real residue is
  `mov [esp+24h],eax` (fade = fade_distance) placed before the reload `mov ecx,[ecx+28h]` (retail sinks it after: 0x007E17A1 / 0x007E17A4).
  Exposing fade_distance or size (f2 / f3) or an else-arm temp copy (t1) gives nothing; j1 (entry reference re-bound before the fade
  store) removes the reload -> the kill is the store to the exposed `fade` itself.
* Why it is not closed: the same exposure that makes CSE kill also makes the scheduler keep the fade store above the pointer load (in the
  non-exposed visible variant the scheduler sinks the fade store below `mov cl,[eax+2Dh]`). Retail = "CSE kill without scheduler dependence".
  Not a tie: k = 1..7 unused locals and 1..7 extra dead asserts (wk10/ku*, ka*) never move it; /G5 /G6 same, /G7 worse.
* Exposure only comes from an EXTERN callee: the same probe function defined in the TU (above, `noinline`, or AFTER the row: r1-r3) removes
  the exposure entirely (c2's whole-TU capture summary). Inline by-ref helpers / local pointers (i1-i3) never expose.
* To land the probe one would need a real out-of-line function taking fade by reference in a release-dead assert (none known: FableWin's
  ~2 assert bodies in this function are unidentified) AND a source fact for the sink. Not landable as is.
* Also tried (no gain): entry through begin()+i / vector reference / begin()[i] (n1-n3, VN unifies), arm order swapped on the visible base (s1),
  a dead exposed dword local stored before the reload (q1-q3: the store is kept, 967).
* (cont.) Exposure kinds for fade all keep the scheduler dependence: &fade stored to a global in the dead arm (e1), passed as `const void*`
  (e2): 5 hunks; by value (e3): no exposure (47). fade through an inline helper (param / return temps, h1-h3): no kill (46-47).
  Scheduler-side reading: retail sinks `mov [esp+24h],eax` only past `mov ecx,[ecx+28h]` and stops at `lea eax,[edx+ecx]` (eax WAR),
  i.e. retail's scheduler sees NO memory dependence between the fade store and the pointer load; ours (exposed) does.

### 0x009FDE80 -- ninth agent (2026-11-15): unchanged, parked (differ/009fde80.cpp 1265v1265 / 39); ~10 grades (wk10/x*, y*, z*)
* NEW FACT (frame): exposing `depth` (extern by-ref probe in a dead FABLE_RETAIL_ASSERT, any position: x1/x2, z1-z3) gives retail's frame
  `sub esp,34h` (all earlier variants 0x30) -> retail's depth home at the TOP local slot is consistent with an address-taken (unpacked)
  depth. But depth leaving the candidate list drops header 83 -> 75 and mipmap_level 82 -> 78, so mipmap_level takes ebx (1259v1265 /
  75-76 hunks, 54-55 reg-structure hunks). With or without the FITTED `levels` local: same ranking. Exposing mipmap_level instead /
  as well (y1-y3): 1249-1272, worse.
* Not landable either way: no retail-plausible extern callee takes depth by reference (AllocateVolumeTexture takes it by value, PDB K;
  a visible callee never exposes -- measured on 0x007E15C0 r1-r3). Recorded as a measurement of where the frame residue lives.

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).

## 0x007E15C0 CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw -- MATCHED (RELOCATION_MATCH, test PASS, addmatch done, vland WIN; tenth agent, ~15 grades)
* What closed it: on the visible-ctor variant (differ/007e15c0_visible_ctor.cpp) the filter skip written as a GUARDED BLOCK
  `if (g_LightingChannelDrawFilter < 0 || channel.ChannelIndex0 == 0) { ...draw... }` instead of `if (... >= 0 && ... != 0) continue;`
  (wk11/n1.cpp, first grade). With the block form c2 no longer unifies `PDef->Meshes[i]` in the drawing block with the one before
  the channel diamond: Meshes._M_start is re-read at 0x007E17A1 with PDef.Object kept in ecx, the fade store sinks below it
  (0x007E17A4), and every other residue (channel block, x87, frame slots) is the visible-ctor's retail form. No exposure, no device.
* Then: ctor made `inline` (retail header COMDAT) with `unsigned char Lit : 1` (landed 0x0077BD70 text, retail 0x0077BE01) -> still
  RELOCATION_MATCH (wk11/m1); the FITTED `entry` reference removed, every access spelled `PDef->Meshes[i]` -> still RELOCATION_MATCH
  (wk11/m4 = src). Using `entry.` for the fade / ctor Mesh breaks it (m2 962 / 56, m3 961 / 24): the re-index is load-bearing.
  n2 (Mesh test as a guarded block instead) / n3 (both) not needed.
* Headers: no overlay change. Local models (no shared header exists): CMultiStaticMeshDef (only a generated header under
  engine/_quarantine/), CEnginePrimitiveStaticMesh (the landed ctor row also uses a local model), the row class itself.
* Names: g_LightingChannelDrawFilter 0x013756F0 stays DESCRIPTIVE (not in Ego_r / FableWin / reloc_bindings; Anniversary PDB globals
  searched for "channel": nothing). Anniversary PDB module 97 has this function but it is the UE3 rewrite (IActor, FVector locals): no help.
* COORDINATOR: the row source defines CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh (0x0077BD70) `inline` on its local model
  with the landed row's initializer list (base via the shared CEnginePrimitive FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR ctor, id unused);
  if the lander wants one text per address, the landed 0x0077BD70 row and this one should share it (decision recorded since agent 1).

### 0x009FDE80 -- tenth agent (2026-11-15): unchanged, parked (differ/009fde80.cpp 1265v1265 / 39); ~15 grades / 1 what-if (wk11/)
* Control-flow forms (lesson of 0x007E15C0), all no gain: allocation loop as guarded halving `if (!Alloc) {halve} else {success; break}`
  (c1) / if-else (c2): 1269 / 60 = the inline-levels baseline; success-driven while (c3) 1206 / 66; `if (!success) return;` instead of
  the success block (y1) 1265 / 47; abort block as if / else (y2) and the frame-count condition split into two ifs (y3): identical (39).
* Dead asserts reading `depth` inside the success block (x1-x3, to stretch depth's lifetime over `entry` and keep it out of entry's
  slot): identical (39) -- dead arms are gone before slot packing.
* What-if: forcing max_mipmap_level's priority to -200 changes nothing (its loop piece #69 is a separate candidate); diagnostic only.
* Frame layout read (retail, local offsets): 0x00 max, 0x03 success, 0x04 CPixelFormat temp, 0x08 loading texture, 0x0C format,
  0x10 BeginReadDataAutoAlloc result temp (8), 0x18 texture (8), 0x20 entry (8), 0x28 ?, 0x2C new texture, 0x30 depth. Ours: entry and
  depth share 0x18, texture 0x20. Retail's depth looks like a slot made after packing (a spilled temp, not a packed user local).
* Anniversary PDB (module 158) BeginLoading has no lexical blocks; locals async_entry / managed_texture / pixel_format / texture /
  texture2 only (no scalars, no helper): nothing new for structure. FableWin / Ego_r list no CFrameData helper that could hold depth
  as an inlined parameter.
* (cont., tenth) Dimension declaration orders (z1 depth first, z2 depth between, both identical 39; z3 depth before `success` 71).
  Same-TU callee visibility: CGraphicFrameData::Sizeof 0x009FCBF0 (called through the inline CFrameData::Sizeof 0x009FDBE0) given a
  visible noinline body above or after the row (s2 / s3, DIAGNOSTIC stand-in body): identical (39) -> not worth porting its landed
  local-model text onto the shared CGraphicFrameData. A visible inlinable stand-in (s1) is just inlined (1256).
  Two structural sweeps in a row moved nothing; parked again. Ideas not yet measured: none with a concrete lead.
* (cont., tenth) k1 guarded halving with the FITTED levels (39, identical), k2 nested c_mipmap condition (39), k3 inverted if / else skip_size (1246 / 39). Third sweep without movement: stopped.

### 0x009FDE80 -- eleventh agent (2026-11-16): NEW SOURCE -- the Anniversary 360 Test build's PPC code + line table of BeginLoading
* $S/anniv_dbg/WellingtonGame-Xbox360-Test.xex is enc 0 / comp 1: probe/anniv/mkimg.py rebuilds the image (probe/anniv/test.img),
  ppcdis.py disassembles (capstone PPC BE; text RVA = 0x440000 + section-5 offset). Line table: `llvm-pdbutil dump -modi=158 -l` on the
  4k PDB (probe/anniv/m158_lines.txt); annotated listing probe/anniv/bl.txt (lib_graphic_data_bank.cpp lines 3064..3205).
* Line -> statement (instructions of earlier statements get relabelled into later regions: trust ORDER of entries, not gaps):
  3071 header; 3072 clamp to MipmapLevels-1 (one line); 3073 max_mipmap_level = SmallMipmapLevel; 3074 clamp (one line); 3075 if IsValid;
  3077 if CurrentMipmapLevel <= m; 3079 TouchFrame; 3080 return; 3082 max clamp (one line); 3084 if IsLoading; 3086 if m >= Loading..;
  3090 max clamp; 3093..3097 bank / texture manager / format / Lock / Lock; 3100 managed_texture (+ success = false); 3103 if volume
  (dims scheduled here); 3110 pixel_format = CPixelFormat(..); 3113 success = AllocateVolumeTexture; 3119 alloc loop call; 3121 success = true;
  3127 width /= 2 (+ ++m); 3128 height /= 2 (+ cond); 3133/3134 Unlock; 3136 if success; 3141 IsLoading; 3143 Abort; 3145 get_flags & 0x10;
  3147 WaitUntilFinished; 3154 FileData release; 3157 texture2 = managed_texture.GetTexture(); 3159 LoadingManagedTexture = managed_texture;
  3160 managed_texture.Release(); 3165 (LoadingMipmapLevel +) texture = LoadingManagedTexture.GetTexture(); 3167 async_entry = CreateEntry;
  3168 size = GetEntryDataSize; 3171 FrameCount / IsFlat test (+ offset = 0); 3174 GetEntryDataType == 1; 3177 size /= ; offset = ;
  3181 level_size = FrameDataSize; 3182 ONLY `mr r6,r30` (a copy of size: working size?); 3183 for c_mipmap; 3185 skip = level_size;
  3186 if (c < 1 && sizes[c] > 0); 3188 skip = sizes[c]; 3190 size -= skip; 3191 offset += skip; 3192 level_size >>= 2; 3196 FileData =
  BeginReadDataAutoAlloc; 3197 SetResourceSize(Sizeof()); 3198 RemoveFromResourceList; 3199 AddToLoadingQueue.
* Names (Anniv360, third source, drafting hints): param get_flags; locals async_entry, managed_texture, pixel_format;
  CORRECTION: by the PPC frame offsets (PDB texture2 = 112 = 0x70, texture = 104 = 0x68) texture2 = the managed_texture.GetTexture() copy (3157), texture = the LoadingManagedTexture.GetTexture() copy (3165).
* PROGRESS (eleventh agent): differ/009fde80.cpp now = wk12/i6.cpp: 1265v1265, 36 hunks, ALL frame-offset hunks (reg-structure diff = retail
  except the two relocation-masked lines). What did it, in order (both earlier FITTED items dropped):
  (1) c_mipmap loop as the PPC lines say: `unsigned long read_size = size;` (3182, the `mr r6,r30` copy), body `skip = level_size; if (..)
      skip = sizes[c]; read_size -= skip; offset += skip; level_size >>= 2;`, BeginReadDataAutoAlloc(offset, read_size, ..); inline levels
      in the loop call (no named levels): 13 -> 5 reg-structure hunks (wk12/e10).
  (2) allocation-loop increment `++mipmap_level, width /= 2, height /= 2` (comma for-increment, body only the if/break): retail's
      increment block (mipmap_level piece in ecx loaded first, max as memory operand): 5 -> 2 (= relocation masks only) (wk12/i6).
      Same bytes: for-increment `width /= 2, height /= 2` with `++mipmap_level;` as the last body statement (i2), or `height /= 2` with
      `++mipmap_level; width /= 2;` (i5). Empty for-increment (i1) or while (i3) -> 1233 (RA permutation).
  Left: frame 0x30 v 0x34 (entry joins depth's slot in ours; retail depth alone at the top slot, entry and new_texture own slots).
* (eleventh, cont.) PPC 3136 (test) / 3138 (a line holding ONLY the branch to the exit) = `if (!success) return;` early-return form (y1,
  differ/009fde80_y1_earlyreturn.cpp): register-neutral (2 masked hunks), changes the slot joins: entry now gets its own slot (function-scope
  exposed object conflicts with every item). Small-frame layout = slot CREATION order bottom-up (verified on i6 and y1). Retail order
  success, max(+pf+loading), format, tm(+5798), texture, entry, new_texture, DEPTH -> depth's slot is created LAST, after every 8-byte item,
  i.e. retail's depth home is not an item of the packer's size-sorted list (in ours it is: size 4, w2, processed before the 8-byte items, so
  5798 joins it in y1 / entry joins it in i6). Depth exposure (dead-arm probe) gives retail's slot set with y1 (yx) but changes the volume
  block's scheduling (store not sunk below the GraphicInfo loads) and the header/mipmap priorities: retail's depth is NOT exposed.
  Not it (frame stays 0x30): unsigned / const / two-statement / one-declaration / uninitialised-then-assigned depth; dims order; CSE via a
  dead assert with the volume call's inline expression (1241); arms swapped (1255).
* FINAL (eleventh agent, ~55 grades / probes): differ/009fde80.cpp = y1 + Anniv360 local names (byte-identical to y1): 1265v1265, 44 hunks,
  ALL frame offsets; the rx (registers / opcodes) diff is retail's except the two relocation-masked lines. No FITTED items left (the
  `levels` local and the `>>= 2` in the for-increment are both gone). Fallback: differ/009fde80_i6_nested.cpp (nested success block, 36 hunks,
  same code). Previous best: differ/009fde80_prev39.cpp.
* The one open fact: retail creates depth's home slot LAST (top, 0x30; frame 0x34); ours makes it from the packer list (size 4, w2) before
  the 8-byte items, so the read temp $T (y1) / entry (i6) joins it and the frame is 4 bytes smaller. Measured and ruled out: exposure
  (code changes), depth spellings (types int / long / unsigned / unsigned short, const, two statements, one declaration, uninitialised),
  dims order, inline expression + dead-assert CSE, forced priorities (depth 0 / piece 0: no change; depth 200: 0x34 but other code).
  Layout of a small frame = slot creation order bottom-up (checked on i6 / y1 / yx listings). c2.dll slot array: [0x108a7890], 0x1c-byte
  records, packer 'new' at 0x1079145e (tools/c2d.py disassembles c2.dll). Next level if anyone continues: find which c2 phase after the
  packer appends a slot (another writer of the slot array), and which candidate kind gets one. No acceptance device applies to a
  frame-creation-order residue.

### 0x009FDE80 -- twelfth agent (2026-11-16): mechanism of the frame residue (wk13/)
* tools/efm.py fixed (L[st-2000:st] slice was empty for a listing whose PROC is near the top; now max(0,..)). Frame map of b0 (= differ)
  ours -> retail (offsets from OUR frame top; retail frame is 4 bigger): depth -24 -> top(0); 5798 -24 -> -32 (joins tm); texture -32 -> -44
  (joins max); texture2 -44 -> -8 (own slot); managed -16 -> -24; async -8 -> -16. Retail = our packer list with depth DELETED + a slot
  for depth appended last.
* /FAs listing of ours shows `tv1460 = 12` (the CompressedMipmapSizes pointer temp #96, split: ebx #62): a back-end TEMP's memory part
  gets its home AFTER the packer (here the dead get_flags arg slot), never via the packer list. c2prio --trace: depth (#13, user symbol)
  has no register left after header takes ebx; split pass makes piece #60 (edx, def -> first call) and the PARENT keeps memory = its
  packer home. Retail's depth home behaves like a temp's: so retail's memory holder of the depth value is a temp / piece, not the symbol.
* Measured byte-identical (44): depth from an inline helper return (h1, ProbeShift stand-in, diagnostic), do{}while(0) around the volume
  call (v1), an empty if between the volume statements (v2). Worse: ternary depth (h2 1274), if(Alloc) success=true (v3 1269).
  Mini-TU probes (wk13/m*.cpp, tools/mrun.sh): a scalar defined before a call and used in one arm stays `_c$` for plain shift, inline
  helper return, ternary, call result, unsigned -- c2 absorbs a copy from a single-use temp into the symbol.
* (twelfth, cont.) PACKER SIM (tools/pki.sh interference sets of b0, rule: items in list order, try slots latest-first, join when the
  item's iset holds no member of the slot (first-fit `f` items not counted as members), a slot never smaller than the item): reproduces
  ours exactly; with depth (item 7) DELETED it reproduces retail's whole map (5798 joins tm, texture joins max, texture2 new slot) and
  depth's slot comes last = top. So the only frame fact is: retail's depth value is NOT a packer item.
* POSITIVE CONTROL (mechanism found): the value becomes a back-end CSE temp (`tvNNN`, not in the packer list, homed after packing) when
  the same expression has a SECOND LIVE consumer before the first call: mini-TU p1 (`long c = p[2] >> l; long e = p[2] >> l;`, e used in
  the else arm) and p4 (second consumer = member store) give tv; dead consumers do NOT (q1 unused local, q2 `if (0)`, q3-q5 dead
  FABLE_RETAIL_ASSERT through fable_assert.h, before or after): they are removed before CSE. In the real row (diagnostic wk13/c2.cpp:
  `long depth2 = header.Depth >> mipmap_level;` + `if (depth2 < 0) return;` in the else arm) depth leaves the packer list and becomes
  tv1079 (homed post-packer; here it joins texture's slot because the extra code changed the list). Copy forms that keep the symbol
  (refs 4, shift done in place): inline helper return (h1), unsigned long / unsigned short / (unsigned long) cast (u1-u3), call result
  (m3), `depth /= 2` or `depth >>= 1` in the allocation for-increment (d1 / d2, dead def, byte-identical 44).
* CLEAN PROBE REFUTES the CSE theory (wk13/e1.cpp, diagnostic: `long depth2 = header.Depth >> mipmap_level;` + `depth | depth2` at
  the single use; e2 with `&` the same): 1256v1265, frame still 0x30. Listing: depth's value is tv1079, BUT the packer list still has 14
  items: depth's place is taken by a nameless item `None/4/w2/f0` (the spilled CSE temp's own packer item), whose slot tv1079 shares
  (-24, same place as ours' `_depth$`). So a CSE temp is ALSO a packer item; only the IV pointer temp (tv1460/tv1494, homed in the dead
  get_flags arg slot) is homed outside the list. The c2.cpp diagnostic (tv1079 beside texture) was the same thing with a changed list.
* Conclusion (twelfth agent): retail's depth holder is neither the user symbol nor an ordinary CSE temp; what c2 entity gets a NEW slot
  appended after packing is still unknown (not observed in any probe). The packer sim (list minus depth + one slot appended last) is
  the only firm fact. Hypothesis only, not measured: an IV / LICM-style back-end temp. No acceptance device applies to a frame-slot
  residue. No FableWin binary / PC PDB on this box. PARKED near_miss (~25 grades / probes); differ/009fde80.cpp unchanged.

### 0x009FDE80 -- thirteenth agent (2026-11-16): c2 packer read from c2.dll; what the packer can and cannot do (wk14/)
* CORRECTION to the twelfth agent: tv1460 (the c_mipmap pointer IV temp) IS a packer item (item 5, kind 3, tuple 0x5b4, no symbol),
  placed by the arg-slot first-fit path into slot 1 (get_flags' arg slot). In e1 the CSE temp t1079 (kind 3) fails first-fit and is
  placed normally in list order (p7 n6): a kind-3 temp is NOT deferred -> the whole "make depth a temp" family is closed.
  Tool: tools/pkk.sh <src> (list with kinds + events incl. first-fit slot).
* c2.dll 13.10.3077 packer, read statically: the item list is built by 0x10788642 (called per reference, ebx = weight increment):
  ascending by item size (+0x20), and inside a size run descending by weight (+0x38, bubbled forward by 0x10789a99 / moved by
  0x107a1065). The packing loop (0x10790c96..) walks that list: kind-5 params skipped, flag-4 items (set from flag 0x40) get an
  all-ones interference set (0x1075e771), every item first tries the arg slots, else place() 0x10791426 joins latest-first or
  appends a slot (0x1079145e, slot size = item size). Every slot comes from this one pass (the only other count writers are the
  init 0x10785b99, the arg-slot setup 0x10790c2e and the >=0x70 repack reset 0x10734b3f). Small frames (<=0x80, and option bit [opt+0x1ea]&1 clear -- inferred to be /Os from the /Os layout)
  are laid out in slot creation order bottom-up (layout loop 0x10790e15 from the last slot down); >0x80 or /Os sort by density.
  Dynamic check (tools/pk/pk2.py: hook on the insertion entry 0x1078865f): every item's size at insertion equals its final size.
* Retail frame read from the bytes (frame-relative): 0x03 success(1), 0x04 max/5797/texture(8), 0x0C pixel_format, 0x10 tm/5798(8),
  0x18 managed(8), 0x20 async(8), 0x28 texture2(8), 0x30 depth(4) -> creation order needs a 4-byte item to be placed after the
  8-byte texture2 item. Shown: the list is size-sorted, sizes are constant from insertion on, and the slot-count writers are the four
  above, so a 4-byte packer item cannot be placed after an 8-byte one. So retail's depth home most likely did NOT come from the packer
  list. OPEN EXIT (the successor's lead): c2's frame allocator 0x1077479d (size ecx, align eax) has 7 callers besides the layout loop
  (0x10790f25): 0x10702f53, 0x1076ee14 (stores the offset to sym+0x18), 0x10798331 (next reads [0x108a9220]), 0x107e2d4e, 0x108044d0
  (sets sym+0x20 |= 0x800 like the layout loop, then allocates), 0x10816825, 0x10831dd6 -- direct symbol homes outside the slot
  records. Anything allocated BEFORE the layout loop sits above every packer slot (= retail's depth position). tools/pka.sh <src>
  lists every allocator call with its return address: z0 (= differ), wk12/yx and wk13/e1 show ONLY the layout loop (0x10790f2a).
  Find which construct reaches one of the other 7 callers before the layout loop. Also: the insertion entry exempts by design the one
  symbol equal to [0x108a9220] (0x10788648 -> 0x10789ac3 = ret, never inserted); 0x10798331 allocates right before reading that
  global, so the exempt symbol has its own direct home: identify what [0x108a9220] is (one symbol per function).
  Negative control: yx's 0x34 frame is fully accounted for by its list events (8 slots, 4+8+4+8+4+8+8+8 = 52; no outside space).
* Measured, no frame change (all 1265v1265, sub esp 0x30): QFE 13.10.4035 c2 ($S/wine/vc71, byte-identical output); flags /Zi /Z7 /Zd
  /GS /Ge /Gy /GF /G5 /G6 /GA /Oi /Op /Ob1 /Ob2 /GR- /EHs- /arch:SSE /Gr /Zp4 /Zp16 /Ot /Ox (/G7 1260, /GX 1355, /Og- 2402, /Os and /O1
  947 with 0x34 but sorted layout and other code); k = 1..7 unused named locals (wk14/k1..k7); dead FABLE_RETAIL_ASSERT x1 / x4
  (wk14/a1, a4) and with 4 class temporaries right of && x1 / x8 / x9 (32+ conditional-temporary bits, wk14/t1, t8, t9, u8).
  Two device sweeps null -> stopped. ~45 grades / probes. differ/009fde80.cpp unchanged. PARKED near_miss.
* (13th, last) [0x108a9220] (the exempt symbol) is 0 at packing time in BeginLoading (tools/pk/pk3.py 'exempt' event): no exempt
  symbol in ours. Next step for a successor: hook each of the 7 direct-allocator callers in mini-TUs (address-taken arrays, alloca,
  setjmp, EH-bearing scopes, struct returns, /Od-style unpacked symbols) to learn which symbol kinds get a direct home, then test that
  kind for depth in the row. Do not resume spelling sweeps.

### 0x009FDE80 -- fourteenth agent (2026-11-16): where a pre-layout (top) home can come from, measured (wk15/; ~25 probes, 0 row grades)
* Tools: tools/scan.sh + scan.py (pk3 `--func @` over whole TUs; flags frames larger than their layout-loop allocations),
  tools/mev.sh (mini-TU events + frame symbols), tools/pk/pk5.py / pkg5.py (allocator hook NOT pipe-gated; logs symbol name, the
  frame counter [0x108a9218] at every allocation, at the pipe and at layout entry, plus the /GS option byte and [fn+0x50]),
  tools/c2x.py / c2xr.py (call / jmp / jcc rel32 xrefs into c2.dll).
* Pre-pipe phase: 0x1076ee14 gives EVERY symbol a provisional home before the packer (BeginLoading: 82 homes, _depth at -72,
  counter -348 at the pipe). Then 0x10785d2b resets the counter to 0 and re-homes only the specials on top, before the layout loop:
  (a) the exempt symbol [0x108a9220] (0x1079831e / 0x10798331, cold 0x107e2d44) = an EH record made by 0x10793fd7 with names
  `__$SEHRec` / `__$EHRec` / `__$ComEHRec` (call sites 0x10798c20, 0x10704131, 0x107f781d, 0x107fa037);
  (b) if [opt+0x1ea]&1 AND [fn+0x50]: the symbol [fn+0x50] at 0x107ae0f4. MEASURED (m5f.cpp, 7 flag sets): the bit is /GS and
  [fn+0x50] is `__$ArrayPad` (the cookie); the same pair selects the sorted layout path 0x107ae0b0. CORRECTION to the 13th agent:
  that bit is /GS, not /Os. m5g (/GS, char buf[32]): counter -4 at layout entry, cookie at -4, layout below it.
  BeginLoading (z0): counter 0 at layout entry, no exempt symbol, [fn+0x50] = 0.
* So the only symbols that get a top home outside the packer list are compiler-made specials (EH record, /GS cookie). Retail
  BeginLoading has neither (no fs:0 setup, no cookie check) -> depth cannot be one of them.
* 0x10734b33 is a REPACK path (frame >= 0x70 after a first pass with arg-slot first-fits): frees slots, count = 0, walks the same
  sorted list through place() again (second caller 0x1072ed8a). Same order. Layout loop 0x10790e15..0x10790f2c: slot count-1 down
  to the arg-slot count [0x108a789c], first allocated = top (z0: 8,8,8,8,4,8,1); no pre-pass, no deferred slot.
* Inlined-function parameters (m3 / m4, with register pressure) become $T kind-4 list items; int->float temps (m2) are list items.
* Positive-example scan (thin): 6 matched rows + 25 compilable style_*/src TUs = 1,871 functions of which only 35 have a frame;
  no frame larger than its layout slots, no 4-byte slot above an 8-byte one outside /GS (one false hit: 13 -> 16 rounding).
* Conclusion: no mechanism FOUND by which this function's 4-byte user local gets retail's top slot with 13.10.3077 c2 (packer
  list size-sorted; top homes only for EH records and the /GS cookie). differ/009fde80.cpp unchanged (1265v1265, frame-only
  residue). PARKED near_miss; it needs a compiler-state answer (a different c2 build / option state that we do not have), not a
  new spelling. Not tried: other c2 builds besides RTM 3077 and QFE 4035.
