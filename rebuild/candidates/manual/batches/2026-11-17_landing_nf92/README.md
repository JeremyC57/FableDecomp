# 2026-11-17 landing nf92: name-fix round 7, NF92 (34 rows: the nf90 follow-ups, q11206..q11224); the last naming landing for now

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "just dont launch anymore
naming agents. you can queue more just save the queue for later" (NF92 was already running). No naming agent runs after this landing;
its follow-ups are queued for later.

NF92 took the 19 entries queued at the nf90 landing. Brief, list and records: `naming/2026-11-17_agentNF92/`.

## Rows (STATUS.tsv, 53): 34 ported, 18 not-a-binder, 1 blocked (0x005BF654), plus 0x0059C6B0 deferred (held by NF91)
- **q11218, `CThing::TCsToBeRemoved` (0x013B8A24):** the private static `?TCsToBeRemoved@CThing@@0V?$list@...@std@@A`. The catalog row 0x0121A670 that
  claimed its initializer actually initialises `NThingManager::UIDsToReplaceWithUID` (0x013B8A8C); the real initializer is 0x0121A440 with its atexit
  destructor at 0x01228CE0. All four renamed. RemoveTC 0x004C9840, WorldUpdate 0x004C8050 (needed the original IS_THING_ALIVE helper and the
  list remove body visible in the TU) and ~CThing 0x004C8310 (an unsigned size comparison) are on the shared headers.
- **q11224, CModeDragonBase::OnDeactivate (0x008E72B0):** retail expands it in 0x008E74F0 (a one-TU probe reproduces retail). The body is inline
  once in engine/CModeDragonBase.h; 0x008E72B0 emits it through the nf86 / nf88 scaffolding (MATCH) and 0x008E74F0 makes the qualified call
  (RELOCATION_MATCH). Link proof with `LINK=` cleared (`link/`): a strong owner plus the new 0x008E74F0 gives LNK2005 / LNK1169; the two new objects
  link. Caveat (NOTES): FableWin.pdb places the original body with the .cpp functions, so retail probably had a .cpp function inlined in its own
  file; that gap is what the `// Fidelity: scaffolding, not retail` marker records. The OnActivate pair 0x008E7290 / 0x008E7390 has the same
  shape (queued).
- **q11223:** the `map<long, CDefString>` tree `operator=` 0x004F9207, `_M_copy` 0x004F927A and `_M_create_node` 0x004F930A.
- **NInventory CItem / CCategory (q11208..q11216):** IsSlotFull, UpdateItemStats, SetSelectedItemIndex, the AddItem / AddInventoryItem overloads,
  IsSlotEmpty and a `find_if`, on NF88 / NF90 / NF91's landed opt-ins.
- **Blocked:** 0x005BF654 (DIFFER 297 v 297, a register permutation unchanged over 7 grades; queued).

## Entries (APPLIED.tsv, 19): 18 done (q11213 done-already), 1 blocked (q11215)

## Rows and checks
- **Section 1: 30 self renames (52 file moves).** Modules match their siblings' catalog spellings (new instances: the EDamageAttribute /
  CHeroStatIncreaseData tree iterator, the `unsigned __int64` tree, and NThingManager's UIDsToReplaceWithUID initializer pair).
- **Headers (14 opt-ins in 8 overlays, rebased on NF91's landing):** CCategory.h, CModeDragonBase.h (`_ONDEACTIVATE_INLINE`), CTCInventoryTrade.h, CTCFish.h
  (1 assert pinned), NInventory_CTCInventoryBase.h, CThing.h (`_CTHING_DTOR`), CTCInventoryItem.h (`_INVENTORYCATEGORY_MEMBER`, used only by the blocked
  0x005BF654 draft), stlport/fable_containers.h (`_STLP_LIST_REMOVE_NOINLINE`, `_STLP_BITSET_RESET`, `_STLP_BITSET_RESET_DECL`). Token-identical without the
  opt-ins (`tokid.log`); `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 34 rows graded from the repo after landing: 29 RELOCATION_MATCH and 5 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 40 binding lines and added 23; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued for later (`land1117fu`, 17)
NF92's NAME_FIXES.tsv (15; new: `list<CTCBase*>::remove` 0x004CC150 from Ego_r and FableWin, whose own row is still a local model), plus the
OnActivate one-definition pair and the 0x005BF654 port. `CThing::NoThings` (Ego_d only) is noted as a candidate name for 0x013B8A20.

## Rule slips
- Two Bash calls exited non-zero; 0x0059C710 was left in progress while later rows were worked; q11218's 0x004C8050 / 0x004C8310 were done after
  q11224, against the queue order.
- NF92 overwrote its fable_containers.h overlay with a fresh repo copy, dropping `STLP_BITSET_RESET_DECL`; it restored the block word for word and
  regraded 0x004C9840 (in the final recheck).
