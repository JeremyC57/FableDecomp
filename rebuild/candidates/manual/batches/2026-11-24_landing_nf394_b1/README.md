# Landing nf394 b1: the last three made-up `SortTreeRecursively(Tree*)` rows are list::size (T3 nf391 q10, ruling R1)

The three rows below defined the made-up NoDuplicates `?SortTreeRecursively@@YIHPAUTree@@@Z`, which made strong duplicates at link. They now carry their `std::list<T>::size` spellings in the nf391 b4 form. That name has no definer and no `self_symbols.tsv` line left.

| Row | Name | Retail callers | FableWin match | Caller relocations anchored |
|---|---|---|---|---|
| 0x004B87D0 | `list<CQuestManager::CBoast>::size` | CTCBoastUI::Activate (call at 0x0056B157) | each caller calls exactly one `list<U>::size` with this U | 39 / 55 |
| 0x006CD240 | `list<CNavWayPoint>::size` | six sites in CNavigatorAStar | same | 6 / 10 |
| 0x00DA0ED0 | `list<CScriptThing>::size` | GetRandomSpawnPoint 0x00DA0E40 | same | 2 / 3 |

`label_trust.py` trusts none of the callers, so each caller is anchored by resolving its relocations against FableWin callees.
- **0x00DA0ED0** has a byte-level second anchor: 0x00DA0E40 passes the node payload (`add esi,8`) to `CScriptThing::operator=` 0x008AB980. Its only caller sits in RunSpawnPointControl, as in FableWin.
- **Ruling R1 / R10:** the tier-1 copies are EQ by COMDAT to landed shared-header definers (coordinator `comdat_cmp` of 004b87d0 against 00813d00: all EQ). Copies whose only landed definers are local models (00a1e030) were re-binned to tier 2 instead: non-unique function-start matches, so no address is recorded. 00a1e5c0, under Account B's claim, was compiled read-only: its `size<CNavWayPoint>` copy is Any and EQ.
- **Coordinator checks:**
  - each source equals the graded source (apart from a trailing newline);
  - landed 3 of 3, MATCH;
  - self = `// Leaf:` = `self_symbols.tsv`;
  - only these rows declared the overload;
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.

  No table change. 00da0e40 still binds the stand-in `?size@CRetailList_CScriptThing@@QBEIXZ` (queued).
- **Process note:** NF394 ran six git commands against its brief. Only a fetch wrote anything, and only to `.git`; `git status` was clean and the reflog showed only coordinator commits.
