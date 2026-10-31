# Hand chunk 649 (CTCInventory .. global): 2 of 10 byte-exact

All ten rows were parked same-length misses from the 2026-10-30/31 retry sweep. Each best attempt was reproduced first
(grade tag `a649_r0`), then changed one lever per probe. Two rows match; the eight parked rows keep their best source in
differ/ with this wave's levers on line 2.

## What matched (2)
- **CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending 0x0093DAE0** (RELOCATION_MATCH, 48 bytes). It returns
  true when the creature is performing any action. Otherwise it asks the combat manager whether the creature is in a
  non-interruptable sequence.
  - Lever 1: a NAMED `CCombatManager& manager = GetCombatManager();` taken from the PDB inline
    `CAIStateGroup_MeleeCombatNearby2::GetCombatManager()`. That inline is `GetWorld().GetCombatManager()`, and
    `CWorld::GetCombatManager()` is itself a PDB inline over the scoped_ptr at +0x4c.
  - Lever 2: both creature reads go through the Ego_d.pdb inlines `CAIStateGroupBase::GetCreature()` ->
    `CAIBrain::GetCreature()` (each returns `CThingAICreature&`).
  - Each layer counts. A raw `*PBrain->Creature` loads the argument through edx. Dereferencing the scoped_ptr directly
    gives `add eax,4ch ; mov eax,[eax]`. The unnamed chain keeps the creature in a callee-saved register (50 bytes).
  - Every callee now has its exact PDB spelling (checked with objdump -t).
- **GFACoordToVCoord 0x009E21D0** (RELOCATION_MATCH, 106 bytes, global __fastcall). It converts an actual coordinate to
  virtual coordinates through its two same-TU helpers, GFAXToVX and GFAYToVY, both inlined.
  - Lever: write the Y helper as `(float)((float)VIRTUAL_SCREEN_HEIGHT * y) / dims.Y`. The explicitly truncated product
    is evaluated before the dimensions call and spilled into y's argument slot between `push ecx` and `mov ecx,eax`,
    as retail does.
  - Forms that put the spill after `mov ecx,eax`: `y = (float)H * y;`, `y *= (float)H`, a named float, a named display
    pointer, `operator->`. `(float)(H * y)` loads y first. A named dims local stores before the push.
  - Bonus: the same body matches the parked out-of-line **GFAYToVY 0x009E1DD0** (bonus/009e1dd0.cpp + test, g++ PASS).
    It is not a chunk-649 row, so it is not in payload.json.

## Parked for Fable 5.1 (8)
- 00587bff CTCInventory::OpenPCSkillsMenu DIFFER(1150v1150): frame 0x20 v 0x1c. Retail shares the find-loop counter slot [ebp-8] with the CreateComponent("PC_SKILLS_LIST") CCharString temp; ours gives that temp [ebp-0xc], and every later slot shifts by 4 | tried: loop in its own brace scope, three declaration orders for fe/list/menu, frame/description/viewport (and the whole entry block) in a nested scope -- all inert | next: the GetChild temps after the block DO share i's slot, so the CreateComponent temp's lifetime is what differs: try CreateComponent through a named CCharString / a different CreateComponent overload, or the PDB `CreateMenuDescPC`-style helper.
- 00563480 CTCHeroOnlineScoreboard::DownloadScoreboardForSelectedClan DIFFER(1178v1178): this pending copy reproduces with frame-slot drift from the first store ([esp+0x40] v [esp+0x44]) and 43 masked hunks, not the single std::string register residue its header describes | tried: nothing new (reproduction only) | next: find the newer source that the 1178v1178 one-residue header refers to before any further work.
- 005f45b5 CTCInventoryAbilities::ConstructPCExpressionCategoryList DIFFER(1233v1233): global register map. Retail keeps -1 in ebx and owner then `items` in esi, with `items` reloaded from [ebp+10h] in each block; ours keeps `items` in ebx for the whole body | tried: nothing new (reproduction only) | next: per-block reads of items through an inline accessor or a named reference per block.
- 0086bfb0 CRandomAppearanceMorph::TransferBinaryIn DIFFER(1307v1307): `jmp 0x86c109` v `jmp 0x86c0f8`. Retail's resize erase arm skips the default CBodyPartMesh temp's free check | tried: the STLport begin()/end()/insert(pos,n,x) layer over _M_fill_insert, a single resize(n, const T& x = T()) with that layer (both inert), a named in-function temp (1179 bytes) | next: lane gap: flow-sensitive known-null of an address-taken temp (retail threads the jump, our build does not).
- 00bf01e0 CEnginePrimitiveRenderUtil::SetupOutlineSmoothAlphaStencilMaskStates DIFFER(1374v1374): the StencilFunc block has `test eax,esi / or esi,eax` where retail has `test esi,eax / or eax,esi` | tried: core/state mask locals, explicit-assign or, ChangeState split, `!(..)` test, value-first SetState, an extra inline wrapper layer, PDB const& effect and const C4DVector& SetUserVertexShaderConstant, inline effect accessors, table-lookup values, the PDB CRenderStateManager::SetRenderState member model (fixes nothing alone; with dead loads it fixes the test/or but moves `mov ecx,eax` after `mov edi,esi`) | FINDING: three DEAD `effect->` byte loads anywhere in the body (even after the last call) give RELOCATION_MATCH (lever_dead_loads/00bf01e0.cpp). One or two do not. Dead global loads or pointer copies do not count | next: owner decision on dead-load levers, else find a natural source with about 3 more effect loads (the PDB inline `SetupOutlineStencilMask(ulong&, ulong&, const CEngineMeshEffectOutline&, ulong, ERenderPassType)` is never out of line in retail and is the likeliest real source of them).
- 00b5e0f0 CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur DIFFER(1547v1547): the DitherEnable (+0x2A0C) block has the same test/or swap | tried: dead-load counts 1/2/3/5/8 | FINDING: five or more dead `this` member loads give RELOCATION_MATCH (lever_dead_loads/00b5e0f0.cpp) | next: as 00bf01e0.
- 00d39960 NScript::CQ_FireHeartScript::StartRound DIFFER(1774v1774): the post-loop `if (IsActiveThreadTerminating()) return;` cleanup copy (DeregisterTimer x2 + epilogue) sits inline behind `je` in ours, and after the shared early-exit epilogue behind `jne` in retail. The two cleanup copies are swapped in layout, and the loop returns follow them | tried: explicit final `return;` (identical bytes), timers + loop + tail in a nested block (frame shifts by 8) | next: lane gap: block layout of a return-with-dtors after a loop. Retest only with a new structural idea, e.g. the loop body in an inline helper.
- 00b6fa40 CEngineWaterRenderer::RenderSeaMidground DIFFER(2019v2019): three sampler blocks (+0x2EF8, +0x2F98, +0x2F0C) have `test esi,eax / or eax,esi` where retail has `test eax,esi / or esi,eax` | tried: plain SetState everywhere (4 blocks off), dead-load counts 1-8 on both forms (never better) | next: the same temp-count tie, but this row needs FEWER temps than the plain form. Look for the real (smaller) setter shape; the per-block SetState1 mix in the best source is not a natural form.

## Skipped
None.

## Names (proven, two sources each)
- 0x0093DAE0 callees: `?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ` 0x006622A0 (Ego_r + catalog),
  `?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ` 0x008FCFC0 (Ego_r + catalog + landed 0x0093DFC0),
  `?IsCreaturePerformingNonInterruptableSequence@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z` 0x006E97A0 (Ego_r + catalog
  + `ret 4` body). Inlines `?GetCombatManager@CAIStateGroup_MeleeCombatNearby2@@ABEAAVCCombatManager@@XZ`,
  `?GetCombatManager@CWorld@@QAEAAVCCombatManager@@XZ`, `?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ`,
  `?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ` (Ego_d.pdb).
- 0x009E21D0: `?GFGetSystemManager@@YIPAVCSystemManager@@XZ`, `?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ`,
  `?GetDisplayManager@CSystemManager@@QAEPAVCDisplayManager@@XZ` (inline), `?GFAXToVX@@YIMM@Z` / `?GFAYToVY@@YIMM@Z`,
  `?VIRTUAL_SCREEN_WIDTH@@3NA` / `?VIRTUAL_SCREEN_HEIGHT@@3NA` (Ego_r data symbols, queued by codex50).
- No descriptive names in the two matched sources.

## Label corrections
- 0x006E97A0: manifest "CombatSequence_CheckActionPending" (_global) is CCombatManager::IsCreaturePerformingNonInterruptableSequence.
- 0x008FCFC0: about 45 reloc_bindings spellings (GetPWorld / GetPFrameWorld on many classes, Sub_8FCFC0, Val_938ed0_A, ResetHelper);
  the one name is CAIStateGroupBase::GetWorld (NAME_FIXES.tsv).
- Render-state members (header debt, not symbols). The 0x14-byte "CTrackedRenderState" of the landed render-state rows is
  Ego_r `CRenderStateManager::CStateInfo`: CurrentState +0, DesiredState +4, BookmarkMask +8, DirtyListFlag +0x10.
  The core is `CRenderStateManager`: StateStack +0x8 (CStateStackElement {State, Value}), StateUpdateList +0x2008,
  StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, DeviceState (CDeviceState) +0x2818.
  The setter inlines are the private `SetRenderState(ulong, CStateInfo&)` + `AddStateToUpdateList(CStateInfo&)`.
  0x01436E18 points at a CRenderManagerCore, which derives from CRenderStateManager.
  The landed CEnginePrimitiveRenderUtil::SetupStencilMask 0x00BF1030 names +0x2BC4 "m_stencilRef" and +0x2BEC "m_stencilPass".
  By the Ego_r CDeviceState order those are **StencilOpPass** (+0x2BC4) and **StencilRef** (+0x2BEC). The parked differ
  sources of 0x00BF01E0 carry the same swap.

## Header debt
- 0x0093DAE0 models CThingCreatureBase, CAIBrain, CWorld, CAIStateGroupBase, CAIStateGroup_MeleeCombatNearbyBase and
  CAIStateGroup_MeleeCombatNearby2 locally. The generated headers flatten the vptr into `void* __vftable`, so they are
  included under `#define X XLayout` aliases (vland: no SHARED_HEADER warnings).
- 0x009E21D0: CSystemManager has no generated header. It is modelled locally with the Ego_r member PDisplayManager
  (+0x60) and uses the shared engine/CCountedPointer.h.
