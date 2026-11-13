## 0x00839100

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

## Repair 3 (verifier findings): RELOCATION_MATCH kept (normal and LANDVERIFY_STRICT_HEADERS=1), source 00ad9f10_match.cpp (= R3.cpp), vt PASS, addmatch redone (/Gr)
1. Overlay pruned to the current repo headers + only the opt-ins the source defines: removed FABLE_RETAIL_CVERTEXGROUPBLOCK_HASGROUP_DECL,
   FABLE_RETAIL_STLP_SORT2_BODY (+_NOINLINE), FABLE_RETAIL_STLP_LIST_ASSIGN_BODY, FABLE_RETAIL_STLP_LIST_POPBACK_NOINLINE (ASSIGN_POPBACK_DECL now
   declares pop_back plainly). Kept: LIST_ASSIGN_POPBACK_DECL, TREE_CONSTFIND_NOINLINE, CVERTEXGROUPBLOCK_REMOVELAST / _ADDGROUP. 0 removed repo lines.
2. 0x00ADB570 moved from LABELS.tsv to NAME_FIXES.tsv: it IS landed (CGuiControlTreePane_SortTreeRecursively_00adb570.cpp, `int __fastcall
   SortTreeRecursively(Tree*)`), wrong; correct symbol ?size@?$list@VCVertexGroupBlock...@@QBEIXZ (FableWin.pdb + xref: only AddBlocks' 3 call sites;
   SortTreeRecursively is 189 bytes in FableWin, takes a list&). LABELS.tsv now has no rows.
3. NAME_FIXES.tsv: 0x00A96DA0 = ??1CVertexGroupBlock@@QAE@XZ (Ego_r 0x00BD5B30, FableWin 0x032FF390; called at 0x00ADA1A3 after push_back 0x00ADA19A);
   landed row still spells CEntryListBuffer_00a96da0::~CEntryListBuffer_00a96da0.
4. Source comment on HasMoreBlocks: Ego_r keeps ?HasMoreBlocks@CVertexGroupBlockOptimiser@@ABE_NXZ out of line (0x00C5BE00) but TLC has no copy; AddBlocks
   expands it at 0x00AD9F66-0x00AD9F99 (GetBlockCount imul 0x00AD9F66-0x00AD9F7D, remaining-byte loop 0x00AD9F90-0x00AD9F99).

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

