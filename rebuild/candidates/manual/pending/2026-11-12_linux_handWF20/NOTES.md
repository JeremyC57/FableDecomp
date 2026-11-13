## 0x00956060

## 0x00956060 CAIStateGroup_MazeTutorialBattleTeleport::Update -- RELOCATION_MATCH (3 grades)
- Matched by porting onto shared headers (Guildmaster Update + JackOfBlades GetRushToPosForLunge idioms): GetCreature/GetWorld/GetTC expanded,
  GetSubDef<CMazeBattleDef> visible+noinline, remove_if+not1(CVectorFilter_FurtherFromPointThan), GFRandom, erase(remove_if,end), erase(begin()+i).
- Last residue was the inline vector::erase(pos) expanding STLport __copy; retail calls __copy 0x0074CA20 out of line from all three erase bodies
  -> new overlay opt-in FABLE_RETAIL_STLP_COPY_NOINLINE (stlport/fable_containers.h, __declspec(noinline) on the visible body).
- Overlay opt-ins needed (hdr/README.md): FABLE_RETAIL_CAISTATEGROUP_MAZETUTORIALBATTLETELEPORT, FABLE_RETAIL_CAISTATEGROUP_MAZEBASE,
  FABLE_RETAIL_CMAZEBATTLEDEF, FABLE_RETAIL_STLP_COPY_NOINLINE. No flags.
- Old draft name errors (see DOSSIER sec. 3): GetPosition->GetPos, GetPWorld->GetWorld, GetRandomSeed->GetWorldSeed, GFGetCurrentFrame->CWorld::GetFrame,
  PTarget->PTargetThing; these were draft-only spellings, not landed ones.

## 0x00F39730

## 0x00F39730 CSurfaceCompositor::StartComposition -- RELOCATION_MATCH (grade 1)
- What made it match: full port onto shared headers (CRenderStateManager setters, CSystemManager/CDisplayManager/CRenderManager, CSurface, CCountedPointer, C2DExtentsI) with the real callee identities (DetachZBuffer 0xA10E20, AttachTarget(const CSurface&), bool SetRenderTarget(CRenderTarget&), GetCurrentAWindow() const) and 4-bool SetColourWriteEnable(true,true,true,true) (the old 0xF-in-ebx residue was the hand-rolled setter). The 1-bool form was not needed.
- Headers/flags: no flags. Overlay opt-ins FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_CAPTURE_INLINE (inline Capture) and FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET (SetRenderTarget decl + inline IsRendering); plus repo opt-ins SCOPED_PTR, COLOURWRITE4, CSYSTEMMANAGER, CDISPLAYMANAGER, CRENDERMANAGERCORE, CTEXTURE, CPIXELFORMAT, C2DBOXF, CSURFACE, CSURFACE_BITMAP. CSurfaceCompositor and CRenderTarget have no shared header (modeled locally like siblings).
- Name fixes: 0x00A10E20 landed as CRenderTarget::DetachTarget -> really ?DetachZBuffer@CRenderTarget@@QAEXXZ; 0x00A0AAA0 = ?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ (old: DoSizeof label, CRenderManager2D::GetViewportBoxFloat in shared header + 6 landed 0x009DB7xx-0x009DC2C0 callers); 0x00A0AA80 = ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z (old: _global::Forward_SetViewport_via0x3a3c_00a0aa80 manifest/labels, CRenderManagerCoreView binding via 0x00B4F510) (see NAME_FIXES.tsv).
- Repair (verifier findings 1+2): NAME_FIXES.tsv extended only; source unchanged, still RELOCATION_MATCH (also strict headers), test PASS.

## 0x00523CF0

## 0x00523CF0 CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps -- RELOCATION_MATCH (first authoring session)
- What made it match: (1) ported onto the shared headers (all draft names were wrong; see DOSSIER sec. 3); (2) callee BODY VISIBILITY:
  the real same-TU bodies of SaveEntitiesBinary 0x00522160 / SaveEntitiesToScript 0x00521EB0 (landed bodies, `#pragma auto_inline(off)`)
  and of SetGamePersistentSaveData 0x00523860 (auto-inlined in the text path, called in the binary path -- exactly retail), plus
  STLport's range insert body visible-but-noinline -> neither local vector escapes (hero push_back folds to the direct 0x436AC0
  call; list walk keeps _M_finish in ebp; names dtor loads start/finish in retail order); (3) named `CCharString noScript("")`;
  (4) `things.clear()` right after construction: the only spelling found that gives retail's zero-store order start, eos, finish
  (the clear re-stores _M_finish; ~25 other spellings, QFE compiler, /Oa /GS /Ow all kept start, finish, eos).
- Headers: hdr/ overlay (README there). Flags: none. Retail TU includes the savers' landed bodies (out of line, real, not stand-ins).
- Not done here: the hero is typed through CThingPlayerCreature.h retail chain (CTHINGPHYSICAL/GAMEOBJECT/CREATUREBASE/PLAYERCREATURE)
  so GetPControlledCreature's result converts implicitly to CThing*.
- Name fix (repair pass): landed 0x0099EF90 (rebuild/src/compiled/00/99/C3DAnimationBlendState_AddCombinationBlend_0099ef90.cpp, leaf
  ?C3DAnimationBlendState_AddCombinationBlend@@YIX...) is really ?Reserve@CCharString@@QAEXJ@Z (Ego_r 0x00AFA7F0, FableWin 0x018092B5,
  labels_chunk307_corrections 0x0099EF90 CCharString::Reserve); recorded in NAME_FIXES.tsv for the coordinator to respell at landing.

## 0x00918B90


## 0x00918B90 CAIStateGroup_Gaze::Update -- RELOCATION_MATCH (wf20 author, ~75 grades)
- What made it match: ported onto shared headers (CAIStateGroup_Gaze/CAIStateGroupBase/CThing/CThingAICreature/CDefPointer/C3DVector);
  (1) GetSubDef<T> real body visible via engine/CDefClassBase_GetSubDef.h + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE -- fixed the
  merged Terminate exits and the normal-path release null test; (2) jitter spelled `jitter.Z = 0.0f; jitter *= 0.25f; pos += jitter;`
  (VC folds Z+=0 into the `mov ecx,[esp+28h] ; mov [esp+28h],ecx` self copy; any `pos = <C3DVector rvalue>` keeps a 12-byte temp,
  Initialise / fields op+=(C2DVector) lose the Z copy); also fixed the global register residue at GetPos / MoveToPos args.
  (3) workBuilding local before GetOwner(), named float angle, fps read after GFRandom, `moving` in its own block.
- Headers (overlay $W/hdr, see hdr/README.md): C3DVector.h FABLE_RETAIL_C3DVECTOR_RANDOMPOINTINSPHERE; CAIStateGroup_Gaze.h
  FABLE_RETAIL_CAISTATEGROUP_GAZE_UPDATE; CCreatureNavigationDef.h FABLE_RETAIL_CCREATURENAVIGATIONDEF (CDefClassBase base +
  FableWin StatGetTypeName). Flags: none. CTCExclusiveUse is a local model (no shared header; Ego_r User +0x0C).
- Identity fixes vs old draft (dossier-proven): SetUser (0x00916640, landed as placeholder CClass_00916640::AssignTarget_00916640 ->
  NAME_FIXES), GetWorkBuilding (not LookAtThing), GetOwner slot 14, GetFacingVector slot 70, CDefClassBase::GetSubDef on PeekBaseDef,
  AvoidDynamicObstacles, GetWorld/GetWorldSeed/GetFrame/RotateXY/GetPos, LookAt tolerance 0.0027777f, facing + pos order.
- Lesson: a Z-only integer self copy amid an XY float update = a 3D `+=` of a vector whose Z was zeroed (Z += 0 folded).
- Repair (verifier findings, both applied): (1) GetWorkBuilding inline body now cites retail `lea ecx,[eax+170h] ; call 0x00A01B50`
  at 0x00918CB7-0x00918CBD and the landed rows carrying the same line (GetFreeBench_0090bc60.cpp:46, IsTimeToGoToTavern_0065aa6f.cpp:83);
  (2) test now models C3DVector::RotateXY (landed 0x00A14040 formula, turns) on the jitter path and asserts the rotated look-at
  (10.1+0.95106, 19.8+0.30902, 5) plus the unrotated look-at on the plain path. Still RELOCATION_MATCH (also strict headers); vt PASS.

## 0x00972990

## 0x00972990 CAIStateGroup_JackOfBladesStage1::OnActivate -- RELOCATION_MATCH (wf20)
- What made it match: ported onto shared headers; first search = index local + named filter + C2DBoxI temp; light loop `count = size(); if (count > 0) for (...)`;
  shield search ONE call over temporaries `GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), 50), CThingFilter_HasDefGlobalIndex(index), &shield)`;
  and the decisive fact: GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing> (0x007FCEF0) body VISIBLE in the TU as a noinline template
  (probe 2 proved it; then the real algorithm via overlay opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY). Without it: shield loaded before the IP base ctor and boss GetTC regs differ (7 hunks).
- Headers: overlay CAIStateGroup_JackOfBladesStage1.h (_JACK_STAGE1_ONACTIVATE), CTCBoss.h (_CTCBOSS_SETPHASESTARTPOS inline), CThingSearchTools.h (_FIRSTROUGH decl, _FIRSTROUGH_BODY). No flags/pragmas.
- Names: draft's ArenaCentrePos -> CTCBoss::PhaseStartPos via SetPhaseStartPos; GetPWorld -> GetWorld; GetHitLocation -> CTCHitLocations::GetPHitLocationWithDefName (0x00815820; Ego_r.pdb 0x0080E4C0 + FableWin.pdb 0x024E6630 ?GetPHitLocationWithDefName@CTCHitLocations@@QAEPAVCHitLocation@NHitLocations@@ABVCCharString@@@Z; overlay opt-in FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME). The landed descriptive GetPHitLocationFromName is wrong: NAME_FIXES.tsv asks the coordinator to respell the 0x00815820 row and its callers. No made-up names remain in this row.

- Repair (verifier findings): callee respelled to the PDB name GetPHitLocationWithDefName (+ NAME_FIXES.tsv); dead probe overlay engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_SETPITEM) removed from hdr/ (moved to old_overlay/, not part of the hand-back). Regraded RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1; test PASS.

## 0x006C8E90

## 0x006C8E90 CThingFilter_IsEligibleWitness::operator()(const CThing&) const -- DIFFER(804v804), 6 hunks (wf20 author, first attempt)
Best source: differ/006c8e90.cpp (= v25 + MODE_SLEEP_BED naming). Ported onto shared headers: CThing.h (GETTC_EXPANDED),
CVectorMap.h, CTCPerceiveThing.h (overlay opt-in FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS, see hdr/README.md), CTCEnemy.h
(_QUERIES), CTCCreatureModeManager.h, CTCCreatureNavigation.h (_LINEOFSIGHT), COpinionDeed.h, CThingFilter_HasTC.h (_OUTOFLINE),
CThingFilter_IsNot.h, NCreatureMode.h. Local models only for classes with no header (IsEligibleWitness, IsDeedTypeEnabled,
IsPerceivingThingAndIsThingPerceived, IsAlliedWithFaction, IsEnemyOfFaction). IS_THING_ALIVE declared as a template (no body).
What fixed the old draft's ebp/edi swap + 8-byte frame: const CThing& signature + shared headers + FABLE_FORCEINLINE sub-filter
operator()s (plain inline = called out of line, 594 bytes) + IsPerceiving result written `bool result=false; if (perceived==X)
result = (hear&&heard)||los||smelt; return result;` (gives the `xor bl,bl` before GetPerceivedThing) + `if (GetType()==1) {...}
return false;` wrap (moves the type exit to the function end) + one `result` var through faction/sees/!sees (`sete al`).
Residue (only block placement; every instruction/register right):
 (a) retail places the shared esi-level `xor al,al; return` (mode-13/HasOpinions/DeedType/IsNot fails) after the IsEnemy arm
     (0x6C8F67, falling into the `!result` exit 0x6C8F69); ours keeps it in line after the mode test.
 (b) unknown-threat tail: retail tail-duplicates the `return true` epilogue (0x6C90C5) and the final `xor al,al` falls into the
     shared `return result` exit; ours has `mov al,1` falling into the shared exit and a duplicated false epilogue at the end.
Ideas tried (ATTEMPTS.log has all 80 variants): early-return vs positive nests of the filter chain (worse: esi push moves up);
|| groupings of mode/filters (moves the block to the IsNot site or the function end, never after the enemy arm); mode check as
nested if / HasTC+GetTC(id) const / in the type-wrap condition; faction as if/else, early return, `!Reverse ? A : E` (register
swap or no change); sub-filter bodies as `&&` / result var (worse); tail: result=false/return result, nav result as bool expr
(adds 8-byte frame), `return result` for successes (frame), separate `sees`/`unseen` vars (v73: FIXES (b) but then the
!unseen exit is folded to a constant in-line `xor al,al` block instead of `je` to the shared exit); pragma optimize s/t/y-off.
Best next idea: combine v73's tail (separate `sees` var, which fixes (b)) with a spelling that keeps the `!unseen` exit merged
with `return sees` (same exit block) -- e.g. a single return statement for both; then attack (a) (the shared false block's
placement after the enemy arm suggests the early-out returns share one exit created at/after the faction code).
Follow-up (v83-v85): every spelling that merges the `!UnknownThreatSearch` and `!unseen` exits into one `return <var>` exit
(`if (!UTS || !(sees = !sees)) return sees;`, nested `if (UTS) { sees = !sees; if (sees) {...} } return sees;`) brings back
residue (b); keeping them separate (v73) fixes (b) but in-lines the !unseen exit. So (b) is tied to how the `!unseen` exit
joins the shared exit; retail may build the tail as a value (e.g. a bool expression / inline whose result is returned) rather
than `return true` statements -- untested next idea.

## 0x006C8E90 -- RELOCATION_MATCH (attempt 2), source $W/006c8e90.cpp (= final2.cpp), test 006c8e90_test.cpp (vt PASS), strict-headers regrade RELOCATION_MATCH
What made it match (block placement only; no flag or header change beyond attempt 1's overlay):
 1. mode / HasOpinions / DeedTypeEnabled / IsNot as ONE && expression into `result` (c1.cpp): moves the shared esi-level false
    block to just after the IsEnemy arm.
 2. the perceiving tail (from `bool perceiving`) as a single-exit block: `bool ret; do { ... {ret = X; break;} ... } while (0);
    return ret;` (= the shape of an inlined helper): esi-level false now falls into its own `return result` exit and the final
    false falls into the ebx-level exit (dw3.cpp, 2 hunks).
 3. the unknown-threat awareness test (aware-of-target || in sight radius + nav LOS) as a SECOND nested single-exit block
    (`bool aware; do {...} while (0); ret = aware;`): puts the tail-duplicated `return true` right after the awareness test
    (nn1.cpp -> RELOCATION_MATCH). Probes: the toy files in $W/probe showed VC7.1 places a merged `return true` after its first
    predecessor only when it is a real return / inline-exit join, and that an inlined helper == do/while(0)+break here.
 Natural if/else-assignment spellings of the same logic (nat1.cpp) add an 8-byte frame; real `return` statements anywhere in
 the tail bring back the old residue (dy*/e*x variants). No helper names were invented (do/while blocks only, locals `ret`,
 `aware`). Headers: overlay engine/CTCPerceiveThing.h FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS (attempt 1, hdr/README.md).

## 0x006C8E90 -- repair (verifier findings), still RELOCATION_MATCH (also strict headers), test vt PASS
- Shape comment corrected: only IsPerceivingThingAndIsThingPerceived / IsEnemyOfFaction operator() are FableWin-only; IsAlliedWithFaction
  has the landed out-of-line row 0x006C8C50 (retail inlines it here anyway); the inline IsAllied body now cites that row (check 5).
- Name conflict handed back in NAME_FIXES.tsv: landed 0x006C8C50 spells `long m_faction` / `CTCEnemy::IsAlliedWithFaction(long)` via a
  local CTCEnemy model; PDB (struct_layouts_egor `CFaction* PFaction`, ctor `@@QAE@PBVCFaction@@@Z`) and CTCEnemy.h / mangled
  `?IsAlliedWithFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z` say `const CFaction* PFaction` / `IsAlliedWithFaction(const CFaction*) const`.
  This row uses the PDB spelling; the coordinator should respell 0x006C8C50 at landing.

## 0x0049BE90


## 0x0049BE90 CParticleEnvironmentInteraction::IsParticleCollidingAt -- RELOCATION_MATCH (grade 9 of this session, strict headers OK)
- Made it match: (1) ported onto shared headers (PWorld CWorld* over NParticleEngine::CAEnvironmentInteraction, const virtual UBE leaf,
  PeekWorldMap() x3, vector<const CThing*> + CThingFilter_HasPhysicsMesh with FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY,
  C2DBoxI(pos.ToC2DCoordI(), 2) via FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS) -> 830v808; (2) ground hitPos stored field by field ->
  804v808; (3) the two ground cases as separate ifs each with its own `hitNormal = ...GetGroundFaceNormalAt(hitPos); return true;`
  (VC tail-merges; the doubled hitNormal use moves hitPos/hitNormal into edi/ebp) -> MATCH. Mesh hit test as nested ifs (no named bool).
- Opt-ins (all in repo headers, no overlay): CCHARSTRING, CCLASS, DEF_CLASS_BASE, C3DVECTOR, CTCBASE, CTHING, CTCPHYSICSBASE,
  CMAINGAMECOMPONENT(+_GETWORLD,_WORLD), CWORLD, C2DBOXI_RADIUS_INITIALIZERS, CTHINGSEARCHTOOLS(+_ROUGHAREABODY), CTHING_STATTYPEFLAGS,
  CTCMAPWHO(+_SEARCHID), CPHYSICSMESHOBJECT, CWORLDMAP(+_GROUNDFACENORMAL). Flags: none. Local class CThingFilter_HasPhysicsMesh (no header).
- Repair (verifier findings, all applied; source unchanged, still RELOCATION_MATCH incl. strict headers): test now lists the nearer
  hit first with a farther one after it, slopes the ground in X (start 2.5 / end 5.0, asserts hitPos.Z == start's 2.5), and adds an
  ok=false thing with the smallest distance. Mutants m/m2/m3 (keep-last, endGroundZ, ok-test removed) all FAIL now.

## 0x00839100 (not landed)

## 0x00839100 CThingGameObject::GetWeaponTrailGraphicIndex -- RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1)
- Matched by: porting onto shared headers (CThing GETTC_EXPANDED, CWeaponDef, CTCCarryable/TrapBase/Berserk/Combat/ObjectAugmentations,
  CThingCreatureBase PeekPCurrentAction) with GetSubDef<CWeaponDef> visible noinline (def held in edi); whole body inside
  `if (PBaseDef->GetSubDef(&def)) {...} return -1;` (shared null-tested exit); WeaponTrails read inline at each use (no reference
  local); reverse loop `rb = rbegin(); end = rend(); for (it = rb; it != end; ++it)` (the copy gives `mov esi,eax` + aligned loop head).
- Headers (overlay $W/hdr, see hdr/README.md): CThingGameObject.h, CTCBerserk.h, CTCCombat.h (as before) + REPAIR: CWeaponDef.h
  FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS (real std::map<EObjectAugmentationType,CWeaponTrailGraphicSet>), stlport/fable_map.h
  FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE + FABLE_RETAIL_STLP_MAP_REVERSE. Existing opt-ins used: FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
  (+ FABLE_RETAIL_AUGMENTATION_PARTICLES: noinline const find, forceinline _M_find), FABLE_RETAIL_STLP_LIST_REVERSE (reverse_iterator). Flags: none.
- Repair (verifier findings 1-7): the made-up CTrailMap / CTrailMapIterator / CTrailMapReverseIterator / SNode / SNodeBase view is gone;
  the row uses the real STLport types from fable_map.h. Out-of-line real bodies of landed callees in the TU: 0x0083AEA0 (map iterator
  operator--, same bytes as landed CRetailTreeIterator_Decrement_0083aea0.cpp) and 0x0083B170 (const find<EObjectAugmentationType>,
  same bytes as landed stdRbtreeCUnprovenKey0083b170..._find_0083b170.cpp). Both compiled from this TU grade MATCH at their own
  addresses (q.py 0083aea0 leaf ??F?$_Rb_tree_iterator.. ; q.py 0083b170 leaf ??$find@W4EObjectAugmentationType..).
- NAME_FIXES.tsv (with the real landed spellings): 0x0078BED0 ?M@C_0078bed0@@QAEKXZ -> CTCBerserk::GetWeaponTrailAttack;
  0x0078BEE0 ?M@C_0078bee0@@QAEKXZ -> CTCBerserk::GetWeaponTrailKnockDown; 0x00787C18 ?fn_operator@@YI_NXZ -> CTCTrapBase::
  IsCurrentTrapToCauseKnockdown (only caller is this row, sits among CTCTrapBase members; not ICF-shared); 0x0083AEA0 and 0x0083B170
  -> the real STLport map<EObjectAugmentationType,CWeaponTrailGraphicSet> spellings (replacing CRetailTreeIterator / CUnprovenKey_0083b170).
- Test: rewritten to model the things/components: covers carrier swap, creature path (null action, each of knockdown/vital/decapitating),
  non-creature trap path on the weapon itself, wielder berserk/combat flourish, reverse augmentation walk, type-0 find, no def. vt.py PASS.
- Made-up names: none left in the row (CWeaponTrailGraphicSet is the Ego_r struct, declared in the CWeaponDef.h opt-in).

## Verifier findings not applied
- None; all seven applied.

## Repair 3
- Finding 1: CWeaponDef::FlourishSpecialWeaponTrail is now the Ego_r CWeaponTrailGraphicSet {long Attack +0; long Knockdown +4}
  (struct_layouts_egor.tsv CWeaponDef FlourishSpecialWeaponTrail) under the existing opt-in FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS
  (#ifdef/#else around the member; default path keeps the long[2] line token-identical). The row reads `.Knockdown` (was [1], +4)
  when the strike is special and `.Attack` (was [0], +0) otherwise -- same order as CTCBerserk GetWeaponTrailKnockDown/Attack and the
  map value. Graded RELOCATION_MATCH first try, also with LANDVERIFY_STRICT_HEADERS=1. Test model's Def now holds a
  {Attack, Knockdown} pair named FlourishSpecialWeaponTrail; vt.py PASS.
- Finding 2: every overlay file (stlport/fable_map.h, engine/CThingGameObject.h, CTCCombat.h, CTCBerserk.h, CWeaponDef.h) diffs
  against the current repo header with 0 removed lines (additions only); hdr/README.md updated.
- Backup of the pre-repair state: repair3_backup/.

## 0x00AD9F10 (not landed)


## 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks -- wf20 author pass 1: DIFFER(828v806), 8 hunks (from 826v806 regalloc-wide, 65 hunks)
Best source: differ/00ad9f10.cpp (= v40), `--flags /Gr`, overlay $W/hdr (see hdr/README.md).
What moved it from 65 hunks to 8:
- ported onto engine/CVertexGroupBlock.h (FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS + new _REMOVELAST opt-in), fable_containers list/vector, fable_map;
  real PDB callee names (CalcCurrentBestTheoreticalResultNewBlock / ...NewBlock2 / FindBestStartingBlock, CanInsertMoreBlocks const, nested CBlockInfo{BlockIndex,Cost,Benefit}).
- tree const find body visible but __declspec(noinline) (new FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE): hoists &CurrentBlock->GroupToIndexMap.
- new-group counter as its own local (newGroups) and InsertBlock out-counts as per-branch locals `added`: VC7.1 homes them in the dead `start` slot,
  giving retail's pointer + count-down inner loop (the draft's reuse of `start` itself blocked the strength reduction = the old "zero in ebx" residue).
- vector push_back / reserve bodies visible but out of line (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE + FABLE_RETAIL_STLP_VECTOR_RESERVE): fixed all the regalloc.
- RemoveBlock(index,count) (PDB AAEXKK, Ego_r out of line, absent in TLC) = RemainingBlocks[index]=true; CurrentBlock->RemoveLast(count); inlined -> store order fixed.
- GetBlock is BY VALUE in FableWin.pdb (?GetBlock@CVertexGroupBlockList@@QBE?AVCVertexGroupBlock@@K@Z) -> the row reads InputList.Blocks[j] (friend), not GetBlock.
Residue (exact): retail calls list::size() 0x00ADB570 three times in the empty-candidate path (Blocks.size(), OptimalBlockList.size() twice) and inlines
  only the top one; ours inlines the first two of those three (VC7.1 inline-budget). Plus eax-vs-edx for RemoveBlock's count at 0x00AD9FF6.
Probes (ATTEMPTS.log): any extra `return`/condition between the top size() and the candidate block (q4,q6,r3) flips ours to retail's pattern
  (Blocks.size/Optimal.size out of line, top inline); an extra one BEFORE the top size (r1,r5) pushes all four out; extra plain statements (q0,r4) or
  dead getters (q5) do not; a weight-heavy function earlier in the TU does not (t1: not TU-wide). Making find inlinable (v66) inlines find (870).
Ideas tried without effect on the size residue: SIZE_DISTANCE size; HasMoreBlocks out-of-class / break form / ==false; HasGroup .cpp-defined (named
  iterator / plain); CArray push_back wrapper; CBlockInfo ctor / aggregate init; Benefit = block.GetGroupCount(); GetMaxIterations() at top;
  iterations local; split || returns; continue forms; if/else first branch; early-return / &&|| / positive-condition shapes of the empty path;
  sort body visible (inline / noinline); list operator= body visible; CanInsertMoreBlocks real body visible (auto / off); /Ob1; Greedy==true; Greedy&&!empty.
Best next idea: find the real extra source-level condition the original had between the top size() and the candidates (probes say ONE extra
  test+exit there reproduces retail exactly) -- e.g. an extra guard in the first-branch / HasMoreBlocks region; or, if the owner accepts it like
  FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE ("Inline-budget"), an opt-in that keeps the three empty-path size() calls out of line.
Headers needed (overlay): stlport/fable_containers.h (LIST_ASSIGN_POPBACK_DECL), stlport/fable_map.h (TREE_CONSTFIND_NOINLINE),
  engine/CVertexGroupBlock.h (REMOVELAST). Flags: /Gr (std::sort __fastcall).
0x00ADB570 = list<CVertexGroupBlock>::size (manifest label CGuiControlTreePane::SortTreeRecursively is wrong): proven in the repair pass, see LABELS.tsv.

## attempt 2 (wf20): DIFFER(806v806), 5 hunks -- size() inlining FIXED; only RemoveBlock's count register left
Best source: differ/00ad9f10.cpp (= s1.cpp), `--flags /Gr`, same overlay. Alt: differ/00ad9f10_alt_z2.cpp (4 hunks, manual RemoveBlock).
What fixed the size() residue: an extra test+exit in AddBlocks' own IL that the optimiser folds away. VC7.1 counts it in the
  inline budget before folding. `if (space < 0) return;` (space is unsigned long, so it is always false) right after
  `unsigned long space = ...` gives retail's exact pattern: top size inline, the three empty-path size() calls out of line, 806 bytes.
  `if (0) return;` does the same. Two top guards `!empty() && ...` also flip it, but they leave a second empty() test (c2/e3).
  A `bool hasOptimal` local gives setne (e1). This is a device: the real source construct is still unknown. It needs an exit in
  the function's own body (see probe 13 for where it works). A dead exit inside the first branch or in an inlined callee does not count.
Residue (exact): at 0x00AD9FF6/0x00ADA001 ours loads RemoveBlock's count into eax where retail uses edx. At 0x00ADA205/0x00ADA208
  ours puts the base in ecx and the count in eax; retail has the base in eax and the count in ecx. Instruction order is identical.
  Manual inline (`RemainingBlocks[i] = true; CurrentBlock->RemoveLast(added);`, z2) gives retail's colours, but the count load
  comes after the byte store (retail hoists it above).
Tried without effect on the colours: RemoveBlock param order (count,index); const params; in-class / defined after AddBlocks;
  CurrentBlock local in the body; bool& ref first; explicit `n = added` copy; no FABLE_RETAIL_CARRAY; CArray<unsigned char>;
  `added` at function scope (34 hunks); `added` = start (frame 0x44); `added = 0` init; /G5 (same); /G7 (38 hunks);
  inline CalcCurrentBestTheoreticalResult wrapper; candidates.size() in the loop condition; size()==0.
Best next idea: find the IL shape where the count is read after the store but scheduled above it. Retail's colours match the
  manual (load-after-store) form, so VC7.1 must have disambiguated `added` from the byte store. Next things to try: InsertBlock
  taking `unsigned long*` (as the landed 0x00AD92E0 spells it) with `&added`; RemoveBlock(index, count) whose count is read
  through a reference to the slot. Also look for the real construct behind the dead exit; the original may have had an
  assert-style early return.
- InsertBlock pointer form tried (P1/P2): no change.

## attempt 3 (wf20): RELOCATION_MATCH (806v806) -- source 00ad9f10_match.cpp (= F1.cpp), test 00ad9f10_test.cpp (vt PASS), strict regrade RELOCATION_MATCH, addmatch done
What made it match: callee-body visibility. InsertBlock's REAL body (landed 0x00AD92E0: RemainingBlocks[index]=false; added=0; for each group of
  InputList.Blocks[index] not in CurrentBlock, CurrentBlock->AddGroup(group), ++added) is defined in the TU before AddBlocks under
  `#pragma auto_inline(off)` (out of line, as retail). With the body visible VC7.1 knows `added` does not escape, so RemoveBlock's count load is
  scheduled above the byte store with retail's registers (edx / ecx). The by-value inlined RemoveBlock(index,count) from the PDB stays.
Still needed: the folded `if (space < 0) return;` device for the list::size() inline budget (without it, with InsertBlock visible: 828v806, 3 hunks;
  `CurrentBlock==0` / `space > MaxN` guards give 822). Original construct still unknown.
Headers / flags: overlay hdr/ (fable_containers.h, fable_map.h, CVertexGroupBlock.h incl. new FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP); `// Flags: /Gr`.
Tried this pass without effect: start as first-branch out-count (frame +4); const&/& RemoveBlock count (right colours, wrong order); flag-ref/count-copy
  in RemoveBlock; CurrentBlock local; /Oa (791 bytes), /Ow, /Ox, /GX-, /Oi-.

## repair pass (verifier findings): RELOCATION_MATCH kept (also strict), source 00ad9f10_match.cpp (= R1.cpp), vt PASS, addmatch redone
1. Test rewritten: callees modelled from their landed bodies (CanInsertMoreBlocks, CountRemainingGroups, NewBlock, CalcCurrentBlockSegments/
   Paint, NewBlock2, FindBestStartingBlock); scenarios A (1 block), B (MaxN forces 2 blocks: new-block push_back/recurse/pop_back), B2 (nested
   new blocks), D (NewBlock2 segment bound evaluated), C (Greedy: sort puts the cost-1 candidate first; checks the block contents it implies).
2. 0x00ADB570 proven list<CVertexGroupBlock>::size (FableWin ?size@?$list@VCVertexGroupBlock...@@QBEIXZ; xref: only 3 callers, all in AddBlocks on
   Blocks/OptimalBlockList; SortTreeRecursively takes a list& and is 189 bytes in FableWin; label_trust 0 rows). Not landed -> LABELS.tsv, not NAME_FIXES.
3. NAME_FIXES.tsv: 0x00AD92E0 InsertBlock signature is AAEXKAAK (unsigned long&) per Ego_r + FableWin; the landed row spells `unsigned long*`.
4. Source comments now name the retail bytes the `space < 0` device reproduces: top size() inline at 0x00AD9F42-0x00AD9F57, out-of-line
   `call 0x00ADB570` at 0x00ADA15A / 0x00ADA163 / 0x00ADA172.

## 0x008C12D0 (not landed)

## 0x008C12D0 CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise -- RELOCATION_MATCH (wf20 author, ~45 grades)
- What made it match: (1) same-TU noinline CVectorMap::LowerBound (engine/CVectorMap.h) -> ebp=&PCreature as retail; (2) the real
  CDefPointer + real noinline GetSubDef body (engine/CDefClassBase_GetSubDef.h, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE) -- no
  Release null test, def not escaped; (3) THE key: the rush body is the `else` of `if (!CauseRush)` instead of an early `return` --
  the early return made VC7.1 give the def a whole-function ebx (frame 0x38, no xor-ebx zero); (4) alive test `!IsDead()` not
  `IsAlive()` (`!Dead` emits `not cl; test cl,1`). Named CCharString locals (a temporary argument differs, 790 bytes).
- Headers/flags: no flags. fable_compiler.h, engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR, _NORMALISE_LOCALS_XYZ, _ANGLE_BODIES ->
  GetAngleXY), CJackOfBladesBattleDef.h, CDefClassBase_GetSubDef.h (+DEF_CLASS_BASE, CDEFINITIONMANAGER, CRESOURCELIST, CCHARSTRING),
  CTCPhysicsBase.h (+CTCPHYSICSBASE, CTCBASE), CThing.h (GETTC_EXPANDED, FABLE_RETAIL_CTHING for inline PeekBaseDef), CThingBaseDef.h,
  CIntelligentPointer.h, overlay CTCBoss.h (FABLE_RETAIL_CTCBOSS_JACK_INTORUSH_ACCESS friend). The CCombatAction* hierarchy stays a
  local model like the landed same-TU siblings 0x008C2550 / 0x008C1600 (PDB header opaque, GenericStrikeResponseBase members opaque).
- Name fixes vs the old draft (all callee spellings, not landed rows): SetAnimationName -> CCombatActionBase::SetCombatAnimation,
  GetFacing -> CTCPhysicsBase::GetFacingVector() const, GetPosition -> CThing::GetPos, g_InvSqrtMantissaTable -> GFastOneOverSqrtTable,
  local WrapAngle(atan2*1/2pi) -> C3DVector::GetAngleXY (header body). No made-up names remain in the row (IS_THING_ALIVE is the
  lane's existing macro-style helper name used by siblings).

### Repair (verifier findings, 2026-11-13) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), test PASS
- Local CCombatActionBase / CCombatAction_GenericStrikeResponseBase / CCombatAction_StrikeResponseJackOfBladesIntoRush models removed:
  CCreatureActionBase (FABLE_RETAIL_CCREATUREACTIONBASE + _PCREATURE), CCombatActionBase (FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER), and
  two overlay opt-ins (hdr/README.md): ..._GENERICSTRIKERESPONSEBASE_TYPED, ..._STRIKERESPONSEJACKOFBLADESINTORUSH.
- CTCBoss friend opt-in replaced by FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER (FableWin.pdb ?GetGenericCounter@CTCBoss@@QBEJXZ inline);
  the row calls boss->GetGenericCounter().
- Local IS_THING_ALIVE replaced by engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE, template __fastcall, PDB spelling) with
  FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT (defined before the first CIntelligentPointer.h include); hitter via PThingThatHit.Get().
- Data comment fixed (0x01277128 = 100 deg in radians, cosine at run time, fcos at 0x008C14DF); CauseRush two-test shape tied to the
  retail `jg`/`jne` pair at 0x008C1343 / 0x008C1345 in the source comment.

### Repair 3 (verifier findings, 2026-11-13) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), test PASS
- GetTC((ETCInterfaceType)0xad, &boss) -> GetTC(TCI_BOSS, &boss) (engine/ETCInterfaceType.h, Ego_r TCI_BOSS = 173); bytes unchanged.
  No other raw enum cast in the row.
- Test: new cases h (dead hitter behind at (0,-5,0), point of hit in front at (0,5,0) -> FRONT rush, angle 0) and i (same geometry,
  hitter alive -> BACK rush). Mutation `if (w.hitterPresent)` (verify_evidence/mut2 idea, $W/mut3.cpp) now prints FAIL; real test PASS.
- Overlays re-checked against the CURRENT repo headers: CTCBoss.h, CCombatAction_GenericStrikeResponseBase.h,
  CCombatAction_StrikeResponseJackOfBladesIntoRush.h each have 0 removed lines (`diff repo overlay | grep -c '^<'` = 0, additions only).
  No CIntelligentPointer.h overlay exists: the row uses the repo header (FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT is in it).

