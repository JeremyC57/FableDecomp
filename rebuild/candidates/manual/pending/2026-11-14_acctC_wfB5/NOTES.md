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
