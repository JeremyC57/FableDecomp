# Chunk 892 TRACE (agent892) -- scheduling / test-or family push

## Session 1 (2026-11-09)
- Read CHUNK_BRIEF, PROMPT, FAMILY.tsv, MATCHING_PLAYBOOK, C2_ALLOCATOR_PROBE, EXTRA_RULES, ASM_RULES.
- tools/gq.py + tools/gr.sh: q.py clone that honours Leaf/Flags lines, masks relocations in both images, reports md (masked
  differing instruction lines) and the first differing instruction index, logs to ATTEMPTS.log, side-by-side to out/.
- Step 1: baseline regrade of every prior source as-is (hdr/ overlay empty = current rebuild/include).
- Baseline (27 grades, ATTEMPTS.log tag `base`): no row matches as-is; md (masked differing instruction lines): 00ee2490 2,
  00f05a10 18, 005fd5e0 230, 008fc430 14, 00b21a50 12, 0096e830 78, 00b482a0 2, 005a887d 2, 00963880 45, 00783850 3, 00be1c10 17,
  005b6767 4, 0076e4d0 6, 00808aa0 22, 008a8010 42 (leaf names no symbol), 004bc800 6, 0090bc60 8, 008f99a0 18, 00807250 2,
  0052eaf0 4, 00beeda0 5, 00c6e790 2, 00c6e820 2, 00b6fa40 9, 00b5e0f0 3, 00bf01e0 3.
- tools/sched.sh (copies of hand890's t13.py/common.py/host.py/sim.py): dumps c2's list-scheduler regions, DAG edges, ready
  lists and picks for one function.

### 0x00807250 __unguarded_partition -- MATCHED (scheduler PAREN tuple)
- DAG dump (out/p807_base.json): region `push lea call fstp lea push lea call 0x269 fcomp ...`. After `call` (unit 0) is picked
  in cycle 2, `fstp` (unit 2, x87) is co-issued in slot 1 because every int node (lea 5 prio 0x4800) shares the call's unit.
  Retail has `call; lea eax,[esp+24h]; fstp`: needs the fstp NOT ready in the call's cycle = one more latency-0 node between
  call and fstp. The second call's result already flows through a no-code 0x269 (PAREN) node into fcomp.
- Inequality: a 0x269 (unit 0, prio (h(fstp)+2)<<10 = 0x4000 < lea 0x4800) between call 3 and fstp 4 -> cycle 3 = lea, cycle 4
  = 0x269 + fstp -> retail order.
- Lever: parenthesise the first value: `const float aValue = (Get(a.first));` RELOCATION_MATCH (v1); `return (Get(a)) >
  (Get(b));` also (v7) but changes 0x00808AA0's compare form (fcomp [mem] v retail's fld; fcomp st(1)); b-only paren (v8),
  `(aValue) > bValue` (v4), `(float)Get()` (v5), parenthesised whole compare (v6), plain expression (v3): no.
  Kept v9 (both initialisers parenthesised, symmetric; 0x00808AA0 unchanged with it) + shared engine/C3DVector.h.
- Test 00807250 PASS (g++ / VC7.1).

### 0x00808AA0 __linear_insert (same TU as 0x00807250) -- in progress
- Same comparator body kept (parenthesised initialisers do not change this row: md 22 both ways).
- Residue = the else arm's two by-value copies: retail pipelines loads one store ahead (no store->load alias edges in the
  DAG), ours pairs them. DAG (out/p808_v9h.json): every store [eax+k] has a lat-0 edge to every later load of the param.
- Lever 1 (proved): the TU's own GetCompareValueForPoint body (0x007F9660, landed logic) under auto_inline(off): `point`
  (= &val) is then known uncaptured and the VALUE copy pipelines exactly as retail (md 22 -> 15). The comparator copy
  stays paired: `this` (= &comp) still escapes into GetCompareValue 0x007F9480 (declared only).
- With GetCompareValue's body too (B2..B7, any order/placement) c2 also learns both callees keep edx, puts `last` in edx
  (retail edi) and the comparator copy is STILL paired (md 70). So comp-copy pipelining is not (only) capture. Open.
- PDB (FableWin): CVectorCompare_BestForTargeting has its own operator()(const C3DVector&, const C3DVector&) const
  (0x024DF1C0, right after the anonymous comparator's 0x024DF190): retail's comparator is two layers. Two-layer form +
  parenthesised initialisers matches 0x00807250 (L2p); plain two-layer forms do not (L1-L4).

### 0x008E8FF0 NCreatureMode::GetStrafingAnimationName(const CThing&, EMovementType) -- MATCHED (same-TU callee body)
- Residue: retail loads the physics vtable (`mov eax,[ecx]`) right after the first fld; ours just before the call.
- Diagnostic: a reading-only same-TU body for the (C3DVector, C3DVector, EMovementType) overload 0x008E8E50 -> RELOCATION_MATCH
  (the delta stores to `movement` no longer order the vtable load once c2 knows the callee does not capture it).
- Real body reconstructed from retail 0x008E8E50 (strings STRAFE_IDLE / _MOVEMENT_BACK/FORWARD/LEFT/RIGHT, 0.005f, 1/sqrt(2.0),
  AccurateNormalise 0x00A14440, operator=(const char*) 0x0099EFE0); our compile of it is 405 v 405 bytes (frame 0x24 v 0x2c;
  not graded). src RELOCATION_MATCH; shared fable_string.h + engine/C3DVector.h (FABLE_RETAIL_CCHARSTRING/C3DVECTOR);
  CThing/CTCPhysicsBase local (header debt). Test PASS.

### 0x0052EAF0 NUISystem::CComponent::ChangeTargetPositionDelta -- MATCHED (exposure lever)
- DAG (out/p52e.json): store [eax] -> load d.Y lat-0 edge (d is address-exposed: the old source passed `d` itself to ChangePosition).
- Lever: the landed twin 0x0052EA30's shape -- `ChangePosition(d + TargetPosition, ...)` (C2DVector::operator+ temporary in d's
  slot) -- d is never exposed -> both copies pipeline. RELOCATION_MATCH on the first grade; shared NUISystem headers; callees
  renamed to the PDB/bound spellings (IsUsingRelativePosition / ChangePosition / CUIScreenUnitConverter::UnConvertVector
  0x0052E530 -- the old comment's 0x0052E520 "AssignScreenFromVirtual" was wrong). Test adapted from the twin's: PASS.
- /Oa tried on 13 rows: no row moves toward retail (it is not the aliasing switch retail used).

### 0x00BEEDA0 std::__pop_heap (decal pairs) -- MATCHED (same-TU template bodies)
- DAG (out/pbee.json): store [eax] -> loads of value+4/+8/+C (value exposed: &value.second.OverlaidBlends goes to the
  declaration-only vector copy constructor).
- Diagnostic: a reading-only same-TU body for the copy constructor -> RELOCATION_MATCH.
- Natural spelling: the real STLport 4.6 templates over the shared stlport/fable_containers.h vector (+ FABLE_RETAIL_STLP_VECTOR_C
  for operator=): VC7.1 calls both vector members (as retail) and compiles their COMDAT bodies into the TU, so c2 sees them.
  RELOCATION_MATCH; symbols = the bound spellings (__adjust_heap 0x00BEEA00, vector copy ctor 0x005BB346, operator= 0x006CADE0).
- General rule: retail's TU always holds the bodies of the template members it calls (COMDATs); a declaration-only stand-in makes
  c2 assume capture and adds store->load alias edges that serialise by-value copies.

### 0x004BC800 CTCGraphicAppearance::GetPositionMatrix -- MATCHED (PAREN tuple, second family hit)
- DAG (out/p4bc.json): call(ctor) at cycle C; fld C, fmul#17 C+1, fmul#18 C+4, fstp C+7 (latency chain, fixed). Ours picks
  the vtable store #10 (prio 0x5400, ready since C) in the empty int slot of C+6; retail leaves C+6 visibly empty, so a
  no-code int node must take a slot earlier: a PAREN after fmul#17 (ready C+4, prio > xor 0x6000) pushes xor to C+5 and lea
  to C+6, and the store lands after fstp/push esi (C+7).
- Lever: `(MainGraphic.RenderSizeX * Scale) * 0.01f` -> RELOCATION_MATCH (p1, first grade).
- Then the source moved onto shared engine/C3DVector, CRightHandedSet, CMatrix3x4, CBaseClass, CCountedPointer<const
  C3DMeshStats>, CThing (GetPhysicsTC) and the landed same-TU 0x004BC670's physics/appearance models; the slot-71 virtual is the
  PDB/landed GetRHSet(float) (0x006B1450), not "GetInterpolatedOrientation". h1/src RELOCATION_MATCH; test PASS (g++, VC7.1).

### 0x0090BC60 CAIStateGroup_MinionWorkInInn::GetFreeBench -- MATCHED (out-pointer GetTC + shared headers)
- DAG (out/p90b.json): the vector's three zero stores carry store->load alias edges into BOTH list loads (`[owner+0Ch]` #5,
  `[node]` #6) and get #5's load priority; retail has #5 before them and only #6 after.
- Probes: same-TU for_each body (fe1), allocator dtor (ad1), functor by reference (r1, the FableWin ctor) -- inert; end()
  hoisted (fe2 214 bytes, reload), both iterators hoisted (n1: stores sink below the pushes); the landed siblings' flat
  `head = list.head` hoist (hh1/hh2/it1) -> the schedule matches, but the component lands in edx (md 2).
- Lever: the component through an out pointer, `PeekTC(TCI_THING_OWNER, &owner)` inside the if (FableWin's GetTC(id, T**)
  template shape; landed siblings 0x00938BF0 / 0x009431E0 note "out-parameter GetInterface ... component lands in eax"):
  pk1 = retail schedule, residue only the key compare (the copied key reloaded after the declared-only LowerBound); pk2
  (constant key) RELOCATION_MATCH. Natural form sh1: shared CThing (GETTC_EXPANDED PeekTC) + CVectorMap's same-TU noinline
  LowerBound (the key folds: `cmp dword ptr [eax],21h`), CThingAICreature::GetWorkBuilding, CTCThingOwner::GetObjectList,
  STLport vector/list + for_each from stlport/*.h -> RELOCATION_MATCH. Test PASS (g++, VC7.1).

### 0x008F99A0 LSocket::Receive(vector<char>&) -- MATCHED (same-TU LSocket.cpp bodies + shared STLport string)
- DAG (out/p8f9.json): one region; the string's start/finish/end stores carry WAW edges into the terminator store `[eax]`
  (both LAddress locals exposed: declared-only Receive(3)), lifting them over the literal pushes.
- s1: shared stlport/fable_string.h + fable_string_bodies.h (assign COMDAT in the TU): 231 bytes (the _String_base zero
  mem-initialisers survive the malloc: the locals still escape through the declared Receive(3)).
- s2/s3 (diagnostic): a reading-only Receive(3) body -> RELOCATION_MATCH (s3; s2's writing body also deleted stores).
- s4: the real Receive(3) body with its callees declared only -> 231 again (capture is transitive: CreateSocket(local),
  ConvertSockaddr2LAddress(.., from)).
- t1: retail's LSocket.cpp order -- ResolveHost(string) 0x008F86A0, ResolveHost(LAddress&) 0x008F86F0, CreateSocket 0x008F87A0,
  ConvertSockaddr2LAddress 0x008F8BB0, Receive(3) 0x008F9320 (landed bodies, under auto_inline(off)) + the shared
  engine/LSocket.h with new opt-ins FABLE_RETAIL_LSOCKET_LADDRESS (ctor + ClearData) / FABLE_RETAIL_LSOCKET_RECEIVE (member
  declarations, Ego_r access) -> RELOCATION_MATCH. Test PASS (g++, VC7.1).
- Gotcha: an overlay header whose relative `#include "X.h"` no longer resolves (X.h next to the repo copy only) HANGS cl.exe
  under wine (no C1083). Keep an unchanged copy of X.h in the overlay dir.

### 0x008FC430 CPhysicsForceModelBase::UpdateScan<const CWorldMap> -- MATCHED (region-cap PAREN count)
- Residue: retail emits ring.end()'s `mov esi,[esi] ; push 14h` after `fstp [esp+1Ch]` (0x008FC6BD); ours 5 x87 ops later.
- DAG: the block is cut into two scheduling regions (83 + 31 nodes); mov/push belong to region 2 and lead it.
- c2 region finder (c2.dll 0x107B4E2F): walks the tuple list, ends a region at a jcc/jmp (kind 0x12, inclusive) or label
  (0x1B), or after 81 tuples (`cmp esi,50h ; jg`, 0x107B4E6A). Calls (kind 0x0F) do not end regions. New tool
  tools/sched/t14.py + tools/tup.sh dumps the raw tuple runs it walks.
- Retail's region 1 ends at `fmul st(2)` (0x008FC6B7) = 75 instructions, so 6 no-code tuples precede it. PAREN tuples count:
  p1 `cos((x))` (arg parens: dropped), v1 parens around vector terms, `(radius)`, `(float)cos(..)` (fc) inert; `((float)
  cos(..))` (fc2) md 8; each trig factor parenthesised (pc: 4 PARENs, CSE merges sin(theta)) md 4; + one doubled (pd1, 5) md 2;
  + two doubled (pd2 / pd3, 6) RELOCATION_MATCH.
- Then shared engine/C3DVector.h, CPhysicsCollisionPointsObject.h, the STLport list (FABLE_RETAIL_STLP_LIST), CWorld.h,
  DELTA_NOTIONAL_ZERO for the 1e-4f; the invented GetPointIntersection stub dropped (`ring.push_back(end)`): still
  RELOCATION_MATCH. Test PASS (g++, VC7.1).

### 0x00B482A0 CEngineLightingManager::CEngineLightingManager -- MATCHED (real STLport vector ctor shape)
- Base md=2: the EmptyLightArray.LightCount store (+0x44EC) is scheduled before `lea ebp,[esi+3Ch]` (retail after).
- DAG (out/pb48.json): one region; lea#15 prio 0x9C00, the +0x44EC store #33 prio 0xA400 (the member-init store chain).
- st1: the local 3-pointer vector stand-in replaced by an STLport-shaped one (_Vector_base(const A&) with
  `_M_end_of_storage(a, 0)` through _STLP_alloc_proxy, `explicit vector(const A& a = A())`): RELOCATION_MATCH.
- h1: rebuilt on the shared headers (CEngineLightingManager.h with the new FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS,
  CArray<CThemeChannel> = shared STLport vector, CEnginePrimitiveRenderer SceneList, CEnginePrimitiveTypeInfo::
  RegisterPrimitiveManager, CMemoryAllocator::AllocateAligned, CRGBFloatColour::Initialise (Ego_r name; the prior's Set was
  invented)): RELOCATION_MATCH. Test PASS (g++, VC7.1).

### 0x005A887D CTCInventoryTrade::RefreshBuyPossibilityPC -- MATCHED (transitive same-TU CDeletion / list bodies)
- Base md=2: `deletion.Method = 3` (0x005A8A0D) is stored before the in-place iterator argument store (ours), retail last.
- DAG (out/p5a.json, region 27): the Method store #3 has an edge to the `mov [eax],ecx` iterator-slot store #9 (a store
  through esp may alias the escaped local) -> prio 0x6000, picked early. Retail has no such edge.
- d1 (implicit CDeletion ctors): VC7.1 /Os inlines the implicit default ctor (retail calls 0x0042BED4) -> md 3.
- e1 (same-TU noinline CDeletion() + list() bodies): md 2 (unchanged: copy ctor / ~list still declared-only).
- e2 (+ CDeletion copy ctor, list copy ctor, ~list bodies, all noinline): RELOCATION_MATCH.
- h1 on shared headers (CTCInventoryTrade.h, CThing GetTC/PeekTC, CTCShop::GetCurrentSellPrice (0x007743B0; the prior's
  GetCurrentBuyPrice was the propagated catalog label), CTCInventoryBase::GetBuildingThing / IsSpaceInInventoryForItem,
  CTCInventoryItem::GetInventoryType, CTCHeroStats::GetMoney, NUISystem_CDeletion.h) with new hdr opt-ins
  FABLE_RETAIL_CDELETION_BODIES, FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE,
  FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC: RELOCATION_MATCH; control h1c (no CDELETION_BODIES): md 2. Test PASS.

### 0x00EE2490 CV_TravellingHeroesScript::CTH_Thunder::Main -- MATCHED (parent reference release)
- Base md=2: the seh_Rival release: retail `mov eax,[esi+14h] ; lea ebp,[eax+48h]`, ours `mov ebp,[esi+14h] ; add ebp,48h`
  (the static ReleaseScriptedControl(handle&) helper: the handle vreg coalesced with the ParentClass load).
- Retail survey (call sites of IsValid 0x00CD23B9 preceded by mov/lea vs mov/add): 19 lea, 6 add. Landed lea-form rows
  (0x00E78630, 0x00E776F0, 0x00ECD9E0) spell `CV_..Script& parent = ParentClass; if (parent.seh_X.IsValid()) parent.seh_X.Release();`;
  0x00EE36A0 (same TU) uses an inline parent member helper.
- v1 (macro-like direct `ParentClass->seh_Rival...` twice): md 249. v2 (parent-class inline member helper): RELOCATION_MATCH.
  v3 (`parent` reference local, the landed spelling): RELOCATION_MATCH -- kept.
- Shared-header rewrite h1 (engine/CV_TravellingHeroesScript.h + CIsThingAlive / CSpawnedFunc / fable_script_distance): md 401
  (the by-value SetIsPushableByHero copy must expand inline while the filter's copy calls 0x004ABE90; the parent-reference release
  goes back to the add form in that context); h2 (inline copy ctor after the include): md 353 (the filter copy inlines too).
  Kept the local model (header debt, as the landed twin 0x00EDF3A0).
- Cleanups: IsScripting -> IsValid() const (FableWin.pdb ?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ; the
  "IsScripting" label was invented), Main private virtual (PDB EAE; the grader warned the leaf matched no symbol), the invented
  SpawnParentFunction wrapper dropped (sp1: direct `ParentClass->AddSpawnedFunction(new ..., CCharString(""))` matches), the
  base-object dtor plain inline (fi1); the CSpawnedFunc ctor needs FABLE_FORCEINLINE (fi2 plain inline: md 198).

### 0x00C6E790 MixGainRampUp / 0x00C6E820 MixGainRampDown -- MATCHED (accumulator indexed through a copy of the counter)
- Base md=2 (both): loop-end pointer steps retail `add eax,10h ; add edx,8` (accumulator first), ours source first; the
  preheader biases already match (`add eax,8 ; add edx,4`).
- DAG (out/p790_base.json): preheader -- the later IV-init `add` is pinned last by flag edges from every earlier flag setter
  (xor#5, add#8 -> add#10), so preheader order = IL order. Latch -- the three adds (#28 gain, #29 source, #30 accumulator)
  only feed `dec`, all prio 0x1400, so the lower IL index wins.
- 28 real-row forms (a..k3: operand order, index types, locals, casts, reference/pointer `out`, pointer increments,
  do/while, separate counters...) and ~60 micro forms (wk/iv/t1..t10): VC7.1 couples the two orders -- preheader X,Y always
  gives latch Y,X (derived-IV inits appended at the preheader end, updates each inserted right after `++i`). A `long& out =
  accumulator[i]` flips both (md 2 the other way). QFE 4035 compiler: identical (t5q).
- t9 y12 `int j = i; a[j] += (s[i] * g) >> 15;`: preheader acc,src AND latch acc,src -- uncoupled. t10: robust across
  `const int`, `long`, `unsigned`, hoisted `j`, a `v` temp, `a[j] = a[j] + ...`; NOT when both arrays use j (z02), only the
  source does (z03), an inline helper with an index parameter (z01) or a reference helper (z05).
- l1 (`const int sample = i; accumulator[sample] += (source[i] * gain) >> 15;`): MATCH on 0x00C6E790 and 0x00C6E820.
  src kept with a comment naming 0x00C6E7AE / 0x00C6E80D (and 0x00C6E842 / 0x00C6E89D). Tests PASS (g++, VC7.1).

### 0x00B5E0F0 CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur -- MATCHED (shared-header rewrite)
- Base (prior local model, `__forceinline` -> FABLE_FORCEINLINE) md=3: the 7th setter (DitherEnable +0x2A0C) has ours
  `test eax,esi / or esi,eax`, retail `test esi,eax / or eax,esi` (tools/tor.py: retail switches form at setter 7, ours at 8).
- g1 (fade clamp through const-ref GFLimit<long,long,long>): md 3, flip site unchanged.
- s1: rewrite on the shared headers -- CRenderStateScopedCaptureBlock / CScopedVertexShaderConstantLayout(2) /
  CScopedShadersEnable(true, true), NEngineInternal::GPShaderRenderManager Apply*/PrepareForRender(0), the inline
  CRenderStateManager setters with the PDB names (SetAlphaBlendEnable .. SetTextureFilterModeMin), DrawAsIndexedTris 0x00A0AEA0
  (FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS), a function-local static QuadIndices (0x0139A544), and a new hdr opt-in
  FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS (Ego_r members): RELOCATION_MATCH first try. The bookmark flip
  is a whole-function IR count effect; the real class layout / setter spellings put it at retail's site. Test PASS (both).

### 0x00BF01E0 CEnginePrimitiveRenderUtil::SetupOutlineSmoothAlphaStencilMaskStates -- MATCHED (shared-header rewrite)
- Base md=3 (StencilFunc block test/or flip; the prior local model also mislabelled +0x2BC4 (StencilOpPass) as StencilRef and
  +0x2BEC (StencilRef) as StencilMask, and passed (1,1,1,1) as a C4DVector where retail calls the CRGBFloatColour overload 0x00989B50).
- s1: shared headers (CRenderManager / CShaderRenderManager / CRGBFloatColour) with the PDB-named inline setters, the PDB reference
  parameter (?..@@QAEXABVCEngineMeshEffectOutline@@@Z), new hdr opt-ins FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
  (SetColourWriteEnable(bool) / SetStencilOpPass / SetStencilWriteMask inline), FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
  and the new hdr/engine/CEngineMeshEffect.h (FABLE_RETAIL_CENGINEMESHEFFECT): md 413 -- TextureAlphaRef read at its setter, edi = 0.
- s2: `unsigned long alphaRef = effect.TextureAlphaRef;` first (retail 0x00BF01F1 loads it before the first call): RELOCATION_MATCH.
  Test PASS (both).

### 0x00B6FA40 CEngineWaterRenderer::RenderSeaMidground -- PARKED (bookmark test/or window)
- Prior local model md 9 (three UV blocks in the other form). s1 shared-header rewrite (CRenderStateScopedCaptureBlock,
  AttachTextureToStage, PDB-named SetTextureUVModeU/V, SetAlpha*Enable, shared CShaderRenderManager / CEngineWaterRenderer with new
  opt-ins FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND and FABLE_RETAIL_CENGINEWATERSETTINGS_COLOUR, lighting ThemeChannels[0]):
  md 14 -- everything matches except the bookmark form (ours switches at UV setter 2, retail at 6) and the AlphaBlendEnable mask
  load order.
- v2/v3/v4 (low-end product inline, colour ctor, no `factor` local): flip unchanged. n1-n3 (texture local, manager local,
  rename): unchanged. L1-L3 (stage loops): not unrolled (worse).
- Diagnostics (dead `unsigned long dummyN = (unsigned long)SeaBumpMap + N;`): at the END k=2..16 never move the flip (k>=8 fixes
  the AlphaBlendEnable load order); at the START k=20 -> 1 site earlier, k=60 -> B B B B A A, k=90..215 -> A x6 (md 3),
  k=240 -> window at site 5; a preceding dummy function in the TU changes nothing. So the switch is a window over the IL node
  count before the first setter (period ~280 dead-load units); retail is ~4 sites later than ours.
- Resumed (wk/forms.py prints each site's form from our column): u60 (60 unused locals) inert; t60 (one `dsum +=` chain, no new
  symbols) = dl60 (B B B B A A) -> the key moves with IL TUPLES, not symbols. Calibration by deletion: -1 AttachTextureToStage ->
  A A A B B B, -2 -> A A A A B B, -capture block -> A A A B B B (code changes; read for the pattern only). Inert: one-level attach
  body (pa), plain `inline` attach (pb), GetD3DTexture() accessor (pd), copy-init capture (ci), swapped source operands in
  SetRenderState (ms: c2 canonicalises the pair), unused locals k=1/2, 60-statement dummy functions before/after (function-local).
  Mask local in SetRenderState (ml): B x6. if/else attach (pc2): code changes (2107 bytes). PARKED at 52 grades, md 14.

### 0x005B6767 CTCInventoryClothing::GetItemDescriptionArmourRating -- MATCH (was parked md 2)
- Prior local model md 4 (pointer `text`, non-const; PDB says ?..@@QBEXAAVCWideString@@J@Z with a nested ArgPasser&).
- s1 shared headers (fable_string.h, CMainGameComponent GetTextBank opt-in, NGameText_CDataBank.h, new hdr opt-in
  FABLE_RETAIL_CTCINVENTORYCLOTHING_ARMOUR: nested ArgPasser (pack 8 inside the generated pack(1) class) + the two PDB methods):
  md 85 with the bank read inline (after the symbol ctor), s2 named bank after the difference: md 4 (= prior).
- t1 bank first: md 2 (global load first, but the +0x14 load before the sub). DAG: #12 (+0x14) and #14 (sub) tie at 0x15000.
- t2 (`game` pointer local first: forwarded, md 4), t3/u3 (explicit CCharString arg: md 79/81), t4 pointer bank (md 2),
  t5 split subtraction (md 2), t6 bank before CalculateArmorValues (md 15), u1 game reference (md 4), u4 named result (md 82).

- Resumed: w1 const game + pointer bank md 4; w2 item armour local first md 2; w3 comma-initialised difference md 4; w5 split
  `-=` md 2; w6 `const ArgPasser&` alias md 2; w4 `game` local read first, difference, bank from game, then `if (game == 0)
  return;` -> RELOCATION_MATCH (DAG: IL global#11, mov#12, sub#13, +0x14#14; the null test is dropped since game was
  dereferenced). z1 `(void)game` md 4, z2 `if (!game) {}` md 4 (both removed before the anchor), z3 the test before the
  dereference: kept (290 bytes, md 85), z4 malformed (cl hang, timeout). Final src = w4 (MATCH).

## 0x00B21A50 CSplinePosParamData::DatabaseSerialise -- MATCH (RELOCATION_MATCH)
- base (prior, local models): md 12. Residue: retail `mov edx,[ecx+4]` (finish) before the CRC temporary's store, then
  `sub edx,[ecx]`; ours stored first, loaded start into eax, then finish.
- sched DAG: store#6 -> finish load#8 has a latency-0 memory edge (store -> start load also has an eax WAR edge); the
  this->Spline load has none. Retail order (finish, store, start) = the same DAG without the memory edge.
- nl1: template member resize with a visible noinline body -> md 12 (template member bodies are compiled after the caller).
- pr1 PROBE: explicit specialisation `CArray<unsigned long>::resize` under auto_inline(off) with a read-only body -> MATCH.
  So the lever is exposure: the CRC temporary's address reaches a declaration-only resize.
- s1 shared-header rewrite: fable_string.h CCharString + new FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE, new engine/CCRC.h
  (FABLE_RETAIL_CCRC_CALC_BODY: Calc's real body visible, inlined in the loop and called at the default by VC7.1's own
  frequency rule), CPersistContext NAMED_DEFAULTS two-argument Transfer, STLport vector with FABLE_RETAIL_STLP_VECTOR
  (_M_fill_insert body) + _VECTOR_C + RESIZE_OUTOFLINE -> md 1: `movzx ebx,byte [edi+ecx]` vs retail [ecx+edi].
- s2/s3 (Calc indexing spelled via cast / char) md 1; s4 GETASCHARARRAY_SELECT md 156; s5 GETASCHARARRAY_NULLRETURN -> MATCH.
- s6 without STLP_VECTOR_C -> MATCH; s7 without STLP_VECTOR (no _M_fill_insert body) -> md 12 (the body must be visible).
- final = s6. Second parameter respelled to the PDB `const NParticleEngine::CEmitterDatabaseSerialiseInfo&` (prior had `bool`).

## 0x00BE1C10 CEngineInternalPrimitive2DViewport::Update -- MATCH (RELOCATION_MATCH)
- base (prior local model): md 17. Retail interleaves the Z factor with the X/Y copy out of the C4DVector temporary; ours
  finishes Z first.
- tools/tup.sh: one 104-tuple block, cut into regions 81 + 23 (the 81-tuple cap). Ours: region 1 ends at Z's `fadd`; retail's
  must end before Z's `fld` (and, from the slot order, before the Y factor's `fstp`). k extra PAREN tuples needed before it.
- c1 `(2.0f * pi)` constant parens: folded, no PAREN. p4: the four clip-space products parenthesised -> +4, md 6 (cut after
  Z's fld). p5 (+1 doubled) md 5: cut at Z's fld boundary, but fld still beats `mov eax,[esp+18h]` in region 2.
  p6 (+2 doubled = 6) RELOCATION_MATCH: Y's fstp leads region 2 beside the copy load.
- Natural 6th/7th PAREN: c2 inline box accessors add none; ca `(float)dims.X` none; cc parens around an initialiser none;
  cb parenthesised X / Y C4DVector constructor arguments -> +2 (constructor arguments keep their PAREN) -> RELOCATION_MATCH.
  cb2 all four arguments parenthesised -> md 2.
- h1..h4 rewrite on shared headers: new hdr engine/C2DAlphaMaskInfo.h, engine/CEnginePrimitive2DViewport.h, opt-ins
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS / _2DVIEWPORT_MEMBERS (typed polymorphic classes), CDisplayManager.h
  (needs FABLE_RETAIL_CDISPLAYMANAGER + SCOPED_PTR + Win32Types/Direct3D9Types) -> RELOCATION_MATCH.

## 0x00F05A10 CQ_AmbushTradersScript::RefreshSpyAllegiences -- in progress (md 10)
- base (prior local model) md 18 (incl. a loop-head pad shift). h1: rewrite on the shared headers (CQ_AmbushTradersScript.h
  CONVOYSPY opt-in, CGameScriptInterfaceBase, CScriptThing, STLport vector, the PhaseTwo 0x00F0C140 define set) -> md 10: the pad
  shift is gone; left: the two `EntitySetThingAsEnemyOfThing` lines with a nested `Game->GetHero()` (0x00F05CF1..0x00F05D2F).
- DAG (wk/s/f05.json): IL start#39, Game#40, copy#41, outer vtable#42 (ebx), add#43, push#44, inner vtable#45 (ours edx, retail
  eax: retail's `push eax ; mov eax,[ecx]` adds a WAR edge 44->45 that lifts the add above the copy).
- tools/c2prio_op.py (c2prio + the tuple opcode per temporary): line 70's temporaries copy->edx (ptr ecx), outer vt->ebx,
  lea (the operator[] reference, also global `local` #27 -> eax) by ROTATION (ptr eax -> eax, ptr -> ecx), inner vt->edx
  (ptr ecx, ecx busy). Retail's registers fit the same order if the lea is a hint assignment (pointer stays at eax): inner
  vt eax, then the site-2 vector start in ecx (retail `mov ecx,[edi] ; mov [esp+14h],ecx`). One rotation step.
- Inert (md 10): this->Game / this->ConvoyMembersVector, static_cast<const CScriptThing&>, *(begin()+i), begin()[i],
  (unsigned int)i, parenthesised arguments, *&GetHero(), (*Game)., trailing continue. Named `const CScriptThing& member`: md 28.

## 0x00963880 CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe -- MATCH (RELOCATION_MATCH)
- base (prior local C3DVector + pad structs): md 45. h1: shared engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR, facing.Dot(toMe)):
  md 26. h2: + FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ (facing Y loaded first, 0x00963909): md 22.
- Residue: the two inline GFFastOneOverSqrt integer estimates schedule differently; retail's second `mov edx,17Ch` sits after the
  table load (0x009639C9), ours hoists it into the first Normalise. tools/tup.sh: from FindChargeEvent's test the block is 120
  tuples, region 1 = 81 tuples ending at the second estimate's `shl` (ours). Retail must cut earlier.
- PAREN count probe (toMe written as components): p0 none md 22, p1 md 18, p2 md 15, p3 md 2 (cut one tuple late: `and ecx`),
  p4a (+1 doubled) RELOCATION_MATCH, k5 (+5) md 2. So exactly four extra PAREN tuples before the cut.
- e1 `(a - b)` on class type: no PAREN; e2 direct-init: none; e3 helper with parenthesised constructor args: +3 (md 2);
  e4/k4s/k4t `GFFastOneOverSqrt((x))` on one Normalise: +1 (with p3 MATCH, either Normalise).
- Header variants (hdr C3DVector.h): FABLE_RETAIL_C3DVECTOR_SUB_PARENARGS alone md 2; + Newton-step parens n1 `(3-(r*r*v))*r*.5`,
  n2 `((3-r*r*v)*r)*.5`, n3 `(3-(r*r)*v)*r*.5` all RELOCATION_MATCH; n4 `r*(3-r*r*v)*.5` = default (same bytes, one PAREN, md 2).
  Kept n1 as FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN (the second Normalise's Newton step is after the cut).
- s1 rewrite on shared headers: CThing / CTCPhysicsBase (GetPhysicsTC()->GetFacingVector(), slot 70) / CIntelligentPointer /
  CAIStateGroupBase / CAIBrain (Creature) + new opt-ins FABLE_RETAIL_CAISTATEGROUP_VERSUSENEMYBASE (PTargetThing +0x14, the prior's
  "PEnemy") and _VERSUSSCORPIONKING (FindChargeEvent 0x00963810, the prior's "IsScorpionCharging"); CThing::GetPos 0x004C73D0
  (prior "GetPosition") -> RELOCATION_MATCH.
- Revision after 0x0096E830 (below): FABLE_RETAIL_C3DVECTOR_SUB_PARENARGS dropped (0x0096E830 needs the unparenthesised
  operator-); hb = toMe's three components written out parenthesised (target position fetched first) + NEWTON_PAREN ->
  RELOCATION_MATCH; hc (default operator-, NEWTON only) md 18. Final src = hb.

## 0x0096E830 CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition -- MATCH (RELOCATION_MATCH)
- base (prior local model) md 78; prior's semantics wrong for the reach (it read 0x012925CC as 2/3: it is 0.8333333 = the
  landed sibling 0x0096E700's ReachScale) and the angle (0x01297068 = (double)float(85/360) * TWO_PI, i.e. GFCos(85/360 turns)).
- s1 shared headers (CThing / CTCPhysicsBase GetRadius / CIntelligentPointer / CAIStateGroupBase GetCreature+GetWorld /
  CAIBrain / CCombatManager + new opt-ins _MELEECOMBATNEARBYBASE (PEnemy +0x14) / _MELEECOMBATNEARBY (private const members,
  Leaf ABE), CCOMBATMANAGER_PREFERREDPOS): md 36. The first Normalise's fstp/fxch reload already matches.
- a1/a2/a3 (creature local, fromPreferred.Dot) inert md 36. s2: Ego_r ?GetCombatManager@CAIStateGroup_MeleeCombatNearby@@ABE...
  inline (+ FableWin CWorld::GetCombatManager, hdr opt-ins) -> GetWorld before the pushes: md 27.
- Residues: second Normalise's pops/`mov ecx,17Ch` order; dot product order with toEnemy.Z kept live (552 v 544).
- tools/tup.sh: block after the combatant test = 140 tuples, region 1 = 81 ending after toEnemy's second fsub.
- x1 NEWTON alone md 20; x2 operator- forms (+ SUB_PARENARGS: +4) 544 bytes md 4; x3 (+5) md 6; x4..x7 explicit ctor parens
  md 24..26; y1..y3 (k=2,3) md 4; z2 operator- forms without SUB (k=0) md 7; z1 operator- forms + NEWTON (k=1)
  RELOCATION_MATCH. w1 explicit 3-component + Z=0 md 46; w2 toPreferred explicit (x, y, 0) MATCH; t1 toEnemy = to - from; Z=0
  MATCH (kept: one spelling for both directions); t2 fromPreferred explicit (x, y, 0) 552 bytes md 20.
## 0x00F05A10 -- parked md 10 after 50 grades
- d1..d7 (args *&, casts, operator[](i), Game[0]., i+0), e1..e3 (loop index int/long/unsigned long), f1 (local start pointer, as landed 0x00EA8C20) inert md 10; f2 (pointer member = begin()+i) md 12: site 2 and the inner vtable become retail's but the outer-this copy is kept/spilled; f3 md 18; x1 (hdr probe: operator[] = *(_M_start+n), no begin()) md 227; g1..g5, h2..h5 (front()-based), i1..i4 inert or worse; j1 (no inline_depth) 1449 bytes md 339, j2/j3 md 240/242.
- Micro TU wk/micro/m2a.cpp (shared headers, 0.4 s) reproduces our site exactly; GetHero by value (diagnostic only) gives retail's add-before-copy order, a named reference/pointer local keeps the outer this in ebx (not retail). Retail's own 0x00DFC7EE (landed 0x00DFC630, plain spelling) has our shape.
- c2 0x10775ACC: the temp gets the hint register when [[tuple+0x1c]+0x30] is set (0x10775B1F -> 0x107500DD); the line-71 lea (reload temp base) has it, the line-70 lea (allocator-local base) does not.
- Parked differ/00f05a10.cpp (h1 + pragma comments).

### 0x00808AA0 -- MATCHED (inline-COMDAT callee bodies)
- C1 (CRT exp in GetCompareValue) / C2 (dllexport) / C3 (address taken): still 218 bytes md 70 -- not IPRA through exp.
- C4..C6 (`__declspec(noinline) inline` on GetCompareValue and/or GetCompareValueForPoint), C7/C8 (plain `inline` on either), C9 (both),
  C10 (both inline, no auto_inline pragmas): RELOCATION_MATCH. An INLINE (COMDAT) body is visible for capture analysis but c2
  does not use its register usage at the caller: `last` stays in edi, the second call reloads ecx, both by-value copies
  pipeline. Plain out-of-line same-TU definitions (B-series) give c2 their register usage too (last in edx, 218 bytes).
- S1: rewritten on shared headers -- new hdr/engine/CVectorCompare_BestForTargeting.h (Ego_r layout; FableWin operator() inline
  under FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING; inline scoring bodies under ..._BODIES) + hdr/stlport/fable_containers.h
  FABLE_RETAIL_STLP_SORT_INTERNALS (iter_swap, __copy_backward/copy_backward, __unguarded_partition, __unguarded_linear_insert
  decl, __linear_insert); the row is the explicit instantiation -> RELOCATION_MATCH. 0x00807250 re-based on the same TU model
  (with and without BODIES): RELOCATION_MATCH (no local pair/swap left). Tests PASS.


## 0x008A8010 CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint -- MATCH (RELOCATION_MATCH)
- Prior: local models (C3DVector, C2DBoxI, a local STL vector copy, flat CThing/CTC/search tools) md 42; k1-k3 md 37.
- h1 shared-header rewrite: engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT), CWorld GetThingSearchTools(), CThingSearchTools
  ROUGHAREABODY (the 0x00969E70 define set), CThing GETTC_EXPANDED with GetTC<CTCSmashableWindow>(&window) (FableWin.pdb
  ??$GetTC@VCTCSmashableWindow@@@CThing@@ at 0x02ACD110 among the script-interface instantiations; Smash is public only in
  CTCSmashableWindow), NEW hdr engine/CTCSmashableBase.h, the PDB centre-radius ctor C2DBoxI(C2DCoordI(x, y), r) -> md 36:
  radius truncated first, filter in the radius slot.
- v1/v2/v3/p1-p3/f3/f4/e1-e8/t1/t2 (named centre, Initialise, member stores, ctor bodies, one-expression, corner+grow trick,
  filter / loop spellings): md 37, `x + r` through `lea` (only y + r in place).
- c2prio_op: c2 puts the commutative pair's HIGHER candidate id first (the lea base / or destination); a lea whose base dies gets
  the hint (in place), otherwise rotation. h1 had r #7, x #12, y #13 (x / y forwarded into the C2DCoordI temporary) -> both in
  place, but r was truncated first; every X,Y,r-order form gave x below r. Candidate ids follow the first definition (a named
  `long r;` declared first still got id 9 after x, y).
- g5 `long x; long y; C2DBoxI area(C2DCoordI(x, y), (long)radius);` -> md 2: X, Y, r order AND both adds in place; left: the
  filter's `mov [esp+38h],4Eh` after the vector zeroing. DAG: the box stores have memory edges to the search-tools load #37,
  the filter store none (VC7.1 sees the inline operator() only reads it). Named filter (n1-n3) md 2.
- o1 = g5 + FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE (retail 0x006A80A0 out of line) -> RELOCATION_MATCH. 33 grades.

## 0x005FD5E0 CTCInventoryMap::ConstructRegionDescriptionQuests -- PARKED (md 230, block placement)
- Prior local model md 230 (975v982). Retail (rdis): the leaves cross-jump into ONE FindChild/SetText/AddChild tail kept after the
  core UI leaf (0x005FD761..), the loop end and `ret 0Ch` follow, then the non-core subtree (0x005FD82D..0x005FD9B1) jumps back.
  Ours keeps the merged tail after the last leaf (fallback core UI) and the subtree in source order.
- b1 pc arms swapped, b3 `text = 0`, b8 `it++`, b11 bool region local, c4 fully nested chain, d2 for(;;)+break, d6 cached end,
  e2 function-scope text, e5 `== true`: all md 230. b2 container in the loop md 461; c1b/c3b `!IsCore()` nesting 986 bytes
  (core leaves after the subtree); d1 while loop 948 bytes; e1 pc-first chains 1018 bytes. 17 grades here + 14 in chunk 194.
- Name: 0x0041E5F2 is NUISystem::CManager::GetInstance (landed 0x006001E0), not CFrontEndManager::GetInstance.

## 0x005FD5E0 resumed (finishing pass): 18 more grades + ~60 reduced-model probes, still DIFFER(975v982) md 230
- Retail and ours hold the same 323 instructions (multiset compare, relocations masked); only the block order differs.
- Reduced model wk/5fd/m0.cpp (loop, IsCore / IsVig / IsOpt chain, pc leaves, shared FindChild / Set / Add tail) reproduces OUR
  layout; wk/5fd/skel.py prints the jump skeleton. Probes: inverted condition, goto (both directions, also from outside the loop),
  inline helpers (plain / forced) with early returns, switch on a computed type and on the bool, do/while(0) and for(;;) / while(true)
  break wrappers, null-pointer and temporary-in-condition tests, more / fewer arms, same-TU callee bodies, flags /Os /O1 /Ot /Og /Oa
  /GS /G5 /G6 /G7 /GB /Gy /Gr /arch:SSE, QFE 4035, and the real source under ltcg_grade.py (/GL + /LTCG link). Every one gives the
  source-order layout or the then-arm-first one; only a block ending in a real `return` was deferred past the epilogue.
- Real grades g01..g12, h20..h24 (`!= false`, declaration placement, operand order, const_iterator, copy-init, named thing, `text = 0`,
  iterator outside the loop, for(;;) break wrapper, early `continue`, explicit CCharString temporaries, CManager::GetInstance, a
  condition declaration): all 975v982 md 230 except g05 (md 241) and h21 (973, md 235). The CManager spelling (h23) is kept.
