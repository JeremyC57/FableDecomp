# NF266 notes (2026-11-22, round 26b)

Base: the repo tree with nf264 landed. NF265 runs beside me: none of its rows touched (every row I compiled was `released` or had no lock
directory before I claimed it: work/locks.log); none is in codex_pass28 RESERVED.tsv. Tools: NF264's set re-pathed to agentNF266 (tools/),
plus tools/prot.py (the entry-1 zero-line-delta edit). Read-first list: the brief chain NF266 -> NF264 -> ... -> NF215 (NF231..NF216 in
full on the advisor's prompt, after a first truncated preview), NF214 and its list (COORDINATION.md, NF92 BRIEF / LESSONS / NOTES, NF44
BRIEF, NF88-90 LESSONS, GOTCHAS.md, EXTRA_RULES.md and ASM_RULES.md in full, in byte chunks, nothing cut), the nf264 / nf261 landing
READMEs and hand-backs (NOTES, LESSONS, evidence).

## Entry 1 (q12760): the four UBE AllowsCombinationAnims binders -- done (4 rows, RELOCATION_MATCH, first form)
- Claim: 0x00693B30 / 0x00693DC0 (CCreatureActionBase ctors, ??_7CCreatureActionBase@@6B@), 0x0069F0F0 (UnsheatheItemFromInventory ctor) and
  0x006A0D50 (UseObject ctor) bind slot 66 as the public ?AllowsCombinationAnims@CCreatureActionBase@@UBE_NXZ; both PDBs spell it MBE.
- Retail targets first (vtmap, evidence/vtmap_*_head.txt): slot 66 of 0x01259D24 / 0x0125CCBC / 0x0125CEFC is 0x00692890 in all four tables
  (no CCombatActionBase / derived override), so the base MBE spelling is the right one, not an override.
- Scope (decision 1): the four rows model CCreatureActionBase locally (HEAD, renaming-macro includes) with every virtual public. Ego_r and
  FableWin name sets spell the whole run of slots 56-69 protected (FrameUpdate MAE, IsUsingAnimationMovement / IsUsingForcedMovement /
  IsMovementUsingPhysics MBE, UpdateActionMoveToPos / InitialiseActionMovement MAE, GetFrameUpdateTime / GetActionSpeedParameter /
  GetActionStartRunningTimeSeconds MBE, OverrideFaceWithDefault / AllowsCombinationAnims / CanAnimationTransitionOut MBE, OnAnimationLoaded
  MAE, WaitForMeshToLoad MBE), and engine/CCreatureActionBase.h declares exactly that run protected. So one in-place respell moves the
  whole run: `protected:` on the slot-56 line, `public:` on the first line after slot 69 (zero line delta: 0x00693B30 / 0x00693DC0 carry
  FABLE_STATIC_ASSERTs of their own). Slot 66 alone would be a two-label edit if the coordinator prefers the queue line's narrow scope.
- Census: nf264's evidence/census_slot66.tsv (clang scan of 4,001 rows) lists exactly these four rows on the UBE symbol; callers.py finds no
  E8/E9 caller of 0x00692890. So 0x00692890 goes 2 -> 1 spelling. For the other thirteen slots I have no whole-tree census; what I can
  claim is "not grown": header-based emitter 0x006DF800 (nf261 evidence/vtmap_006df800_rows.txt) already binds the protected spelling at
  every one of these retail addresses (and 0x00694020 / 0x00692BC0 / 0x00692C30 / 0x00692880 / 0x006928B0 have landed MBE / MAE self
  leaves), so no address gains a spelling. In 0x006A0D50 slots 56 / 58 are UseObject's own overrides: not touched.
- Objects (evidence/symdiff_<row>.txt, VC7.1 HEAD v new): defined symbols identical; all function COMDATs byte- and relocation-identical
  (14 / 15 / 12 / 5); the undefined set differs by exactly the U*E -> M*E pairs, 14 / 14 / 14 / 12 (0x006A0D50 has two own overrides in the run).
  vtmap (evidence/vtmap_<row>_{head,new}.txt): only the slot 56-69 lines change; 0x00692BC0 / 0x00692C30 / 0x00692880 / 0x00692890 /
  0x006928B0 / 0x00694020 go DIFF -> SAME, the NOLEAF ones stay NOLEAF on the same retail addresses. No new pick-any copy (pk: nothing to check).
- Link (evidence/link_e1.txt; LINK unset, /DLL /NOENTRY /FORCE /NODEFAULTLIB): HEAD set (the 4 HEAD rows + header controls 0x006DF800 and the
  landed definers 0x00692BC0 / 0x00692C30 / 0x00692880 / 0x00692890 / 0x006928B0): 0 duplicates, 54 references to U*E base-slot symbols
  unresolved (14 + 14 + 14 + 12; nothing defines them; the tool's raw count of 59 includes 5 OnFrameUpdate lines, a public slot-25 symbol
  that matched the watch substring, in both sets). New set: 0 duplicates, 0 U*E slot 56-69 references left, GetFrameUpdateTime / GetActionSpeedParameter / OverrideFaceWithDefault / AllowsCombinationAnims /
  CanAnimationTransitionOut each resolve to their one definer; the rest are the protected symbols with no landed definer in the set, as for
  0x006DF800. Positive control (a renamed copy of the 0x00692890 object): LNK4006 on the MBE symbol. 0x00694020 (FrameUpdate) was meant as a
  control but its HEAD source fails the clang precheck (fable_map.h:703 `erase(first++)` on an iterator with no postfix ++, which VC7.1 accepts
  with C4620; GOTCHAS hand4b): not compiled, lock released.
- Tests: the four HEAD tests are standalone host models (they define their own CCreatureActionBase bodies, not linking the row): unchanged,
  prechecked, PASS in the recheck.
- Recheck (evidence/vland_e1.log): 4 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT (the WARN SHARED_HEADER_AVAILABLE lines are HEAD's).
- No header change, so no equiv_check / pin_asserts for this entry. TABLE_CHANGES: none.

## Entry 2 (q12743): CastAssassinRush -- partial: 0x008CD470 done (first form); 0x0072CAA0 left
Scoped from nf261 NOTES entry 4; the three facts settled before any header text:
- Slot 12: retail vftable 0x01284254 (81 slots) holds 0x008CD470 at slot 12, so `virtual void Initialise();` overrides, appends nothing.
  The HEAD object already defined ?Initialise@CCreatureAction_CastAssassinRush@@UAEXXZ (its local base declared Initialise virtual), and
  the port defines the same leaf: no self rename, nothing newly resolves. 0x008CD470 emits no ??_7CCreatureAction_CastAssassinRush. Slot 12 elsewhere
  (evidence/census_slot12_clang.txt, clang MS-ABI -O0 of the HEAD sources, NOT confirmed with VC7.1 objects): the ctor rows 0x008CE040 /
  0x008CE100 / 0x008CE1B0 / 0x008CEF50 emit 1-slot stub tables (local models); 0x0096AE50 / 0x009735D0 emit 70-slot tables binding
  ?Initialise@CCreatureActionBase@@UAEXXZ at slot 12 and 0x008C2550 one binding ?Initialise@CCombatActionBase@@UAEXXZ, where retail has
  0x008CD470 (NAME_FIXES, clang-only). None of them defines the new macro, so none changes here.
- CTCAssassinRush: no header anywhere under rebuild/include (0x008CEAE0's comment names a flat engine/CTCAssassinRush.h that does not
  exist). Its HEAD local model stays in place; only FinalAction (+0x90, Ego_r) and SetFinalAction's parameter are respelled from the
  local CCountedAction stand-in onto the shared CCountedPointer<CCreatureActionBase> (both PDBs: ?SetFinalAction@CTCAssassinRush@@QAEXV?$CCountedPointer@VCCreatureActionBase@@@@@Z,
  Ego_r 0x00906010 / FableWin 0x029F9680, which is 0x008CEAE0's landed leaf).
- Include closure: the define set follows landed twins on the same headers (0x0096AE50 for the CastAssassinRush retail branch, 0x008C64F0
  for CCombatActionBase MELEE + INITIALISE with CCreatureActionBase FULL_MEMBERS); CThing::RemoveTC through the declaration-only
  FABLE_RETAIL_CTHING_REGIONLOAD_TCS block; CRightHandedSet CTOR_INLINE (Ego_r ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z, the
  0x0079ACC0 in-place build); CTCPhysicsBase slot 74 SetFacingAnglesFromRHSet (`call [eax+128h]`, as HEAD). The overlaid header is
  included last by the row itself (nothing in rebuild/include includes it).
- Header: hdr/engine/CCreatureAction_CastAssassinRush.h, one add-only declaration (hdr/README.md); pin_asserts pinned 0; whole-tree
  equiv_check, repo rebuild/include v work/tree_new (the repo tree + the overlay file), every landed source and test: 57,125 rows checked,
  0 differ, DONE 0 (evidence/equiv.log). Includer recheck on the merged tree (evidence/vland_includers.log): 0x0072D990, 0x0096AE50,
  0x009735D0, 0x008C2550, 0x008CEF50, 0x0072CAA0 all RELOCATION_MATCH + PASS (sources and HEAD tests prechecked first). The 3-argument ctor declaration that 0x0072CAA0 needs is NOT added (that row was not started).
- 0x008CD470 (rows/008cd470.cpp): RELOCATION_MATCH on the first form. Its Initialise COMDAT is byte-identical to the HEAD object's (298 B,
  same hash); the only relocation change is the SetFinalAction stand-in -> the PDB spelling (evidence/symdiff_008cd470.txt; RENAMES s2).
  The object drops the local-model COMDATs (CCountedAction copy / dtor, CPhysicsMethods wrapper, CopyDirection, GetPhysics) and gains
  header inlines: pk (evidence/pk_008cd470.tsv) ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z EQ owner 0x00487050, ??1?$CCountedPointer@VCCreatureActionBase@@@@QAE@XZ
  EQ owner 0x0048C5C0, the other 8 NO-OWNER (C3DVector ctors, CIntelligentPointer Get / ->, GetPhysicsTC, GetType, the CCountedPointer copy ctor).
- Census: callers.py 0x008CEAE0 -> one E8 site, in 0x008CD470; no other source spells SetFinalAction for this class (grep); reloc_bindings
  has only the stand-in line (witness 0x008CD470, TABLE_CHANGES). So 0x008CEAE0 goes 2 -> 1 spelling.
- Link (evidence/link_e2.txt): HEAD 0x008CD470 + HEAD 0x008CEAE0 leaves the stand-in SetFinalAction unresolved; new 0x008CD470 + HEAD
  0x008CEAE0 resolves it to its one definer, 0 duplicates; positive control (a renamed copy of the 0x008CEAE0 object): LNK4006.
- Recheck (evidence/vland_e2.log): RELOCATION_MATCH + PASS. The HEAD test is a standalone host model (own copy of the body and models):
  unchanged; rewriting it on the shared headers is test debt (queued, NAME_FIXES).
- Left: 0x0072CAA0 (local CThing / CPlayer / CThingCreatureBase / CTCTargeter / CTCSpecialAbilities models + the CastAssassinRush model with
  its conversion operator; needs the TARGETCTOR + a new 3-argument ctor declaration and a second equiv_check), and 0x008CE040 / 0x008CE100
  (self-v-binder splits, queued) -- not started on the advisor's stop rule (context).

## Entries 3-4 (q12744, q12659): open (not started)
Handed back at the q12743 boundary (advisor stop rule: context). q12744 keeps nf261's caution (NOTES entry 5): with TREE_DTOR_DECL_ONLY in
all four rows nothing may define ??1?$_Rb_tree@KU?$pair@$$CBKJ@std@@...; today 0x00AD9770 / 0x00AD9980's pick-any copies resolve
0x00ADCE10 / 0x00ADC310's reference; link the four objects before and after, unresolved lines included. q12659 needs the full 469-includer
proof. No lock of those rows was taken.

## Whole-pass checks
- Funclets / folds: no `../src/compiled/` include names any of the five changed files (grep of rebuild/src, rebuild/tests, rebuild/include).
- Descriptive names added: none. Flags lines added: none (no changed row has CompilerFlags; none needs one).
- Locks (work/locks.log): the five hand-back rows stay NF266 (rows.txt); every control (0x006DF800, 0x00692BC0, 0x00692C30, 0x00692880,
  0x00692890, 0x006928B0, 0x00694020, 0x008CEAE0) is back to `released` (0x00693DC0 / 0x0069F0F0 / 0x006A0D50 / 0x00692BC0 / 0x00692C30 /
  0x00694020 / 0x008CEAE0 had no lock directory: created with plain mkdir); the six includer-recheck rows 0x0072D990 / 0x0096AE50 /
  0x009735D0 / 0x008C2550 / 0x008CEF50 / 0x0072CAA0 were claimed for the recheck and released after it (0x008CEF50 / 0x0072CAA0 unchanged sources).
- recheck_final.log: one vland --recheck-landed run over the five changed rows (overlay hdr/).
- Nothing written in the repo (root_before.txt v work/root_after.txt); no git; no sleep.

## Rule slips
- Three Bash calls exited 1: a `wc` that hit the nf264 batch's logs/ directory, a `sed` whose address came out empty, and a poll whose last command was a `kill -0` on a finished PID.
- NF231..NF216 were first read through a truncated preview (output over the limit); re-read in full in two calls when the advisor flagged it.
- The entry-1 recheck (`vl.sh e1`, whose log was the first recheck_final.log) was launched without an outer `timeout 900` on the launch
  line (vl.sh carries `timeout 900` inside on the vland line); every later launch has it on the launch line.
- 0x008CD470's HEAD test was prechecked only after its e2 recheck (clean then; the fin recheck PASSes too).
- One `rm -rf $A/work/tree_new` on a variable path (the directory did not exist yet: a no-op) before building the merged tree; should have
  been tools/clean.py.

## Decisions for the coordinator
1. q12760 scope: the in-place respell moves the whole PDB-protected run, slots 56-69, in the four local models. Censused: 0x00692890
   2 -> 1 (nf264 slot-66 census + callers.py). The other thirteen addresses (the five with landed protected self leaves included) have no
   census: "not grown" (witness 0x006DF800 binds the protected spelling at each). Accept, or trim
   back to slot 66 only (a two-label edit)?
2. q12743 lands partial: the add-only CCREATUREACTION_CASTASSASSINRUSH_INITIALISE declaration + 0x008CD470 (and the two TABLE_CHANGES
   lines); 0x0072CAA0 (with the 3-argument ctor declaration and a second equiv_check) and 0x008CE040 / 0x008CE100 to the next agent?
3. NAME_FIXES (queue): slot 1 Validate and slot 3 PerformAction self-v-table splits, the 0x006929C0 / 0x00694D10 stand-in self leaves,
   the CIntelligentPointer<CThing> ??_E v ??_G slot, the four entry-1 rows' port onto engine/CCreatureActionBase.h, the 0x008CD470 test.
4. q12744 / q12659 to the next agent.
