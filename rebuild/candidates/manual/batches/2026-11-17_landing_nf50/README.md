# 2026-11-17 landing nf50: name-fix round 7, NF50 (98 rows: STLport red-black tree internals)

Owner: "Continue the naming fix queue with two agents until complete".

NF50 took 9 entries, 99 binder rows from NF41's probe and NF44's binder lists. The main entry is nf44 39,
`?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z` (0x0042971D, 79 rows). The rest are `_Rb_tree`
insert_unique / _M_find / _M_erase / find / clear entries and one `_List_base::clear`.

Container restarts interrupted the agent twice. Its last run finished every row, wrote the hand-back and ran the final recheck
before the next restart. Only its final report was lost, so the coordinator landed it from its files. Brief, worklist and
records: `naming/2026-11-17_agentNF50/`.

## Rows (STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 98 (96 worklist rows + 2 extra) |
| not-a-binder | 3 (0x004804DE, 0x004830A1, 0x00563920) |
| blocked | 0 |

The two extra rows:
- 0x00769980, the `_M_create_node` callee of 0x0076A110, ported in step with it.
- 0x00566FE0, the e185 `find` callee. Its own leaf was the placeholder `CUnprovenKey_00566fe0`; its type is now Ego_r's
  CTCHeroOnlineScoreboard::DownloadedClanScoreboard.

## Entries
All 9 entries are done: e39 (79 rows), e42, e53, e55, e85, e175, e117, e217 and e185.

## How e39 was ported
- **Step 1 (69 rows):** the call binds `std::_Rb_global<bool>::_Rebalance` from stlport/fable_map.h. Each stand-in node type derives
  from `std::_Rb_tree_node_base`.
- **Step 2 (10 rows):** explicit instantiations of the header's `_M_insert` (`FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY`). The
  instance types come from Ego_r.pdb members and the landed callers. These 10 rows are renamed to the STLport `_M_insert`.
- **Self names still stand-ins:** 78 rows had their binding fixed but keep stand-in self names, such as
  `CRetailTree_*_buyNNNN::Insert` and `list_T_*`. Their callers must be ported first. They are queued as one family entry,
  `land1117fu`, with the list in NF50's NOTES.md.

## Rows and checks
- **Self renames (section 1, 20 rows; 34 file moves for sources and tests):**
  - the 10 `_M_insert` instances;
  - `_Rb_tree::_M_erase` / `clear` 0x00630D90 / 0x00630FE0 (map<unsigned long, CCharString>, Ego_r CUIDef::ActionMap);
  - `~_Rb_tree` 0x00485E45 (map<CWideString, bool>);
  - `_List_base<CNavRegion>::clear` 0x00A84110;
  - `find` 0x00566FE0;
  - the CTCInventoryTrade Refresh* leaves.
- **Coordinator correction:** NF50 listed 0x0076A110 / 0x00769980 under the module "CSoundSetTree (stand-in)". These are leaf-only
  changes (pair class key `V?$pair` → `U?$pair`), so the coordinator kept their STLport catalog names
  (`std::_Rb_tree<EObjectAugmentationType, ...>::_M_insert` / `_M_create_node`). Their leaves still carry the pre-existing stand-in
  class; this is queued.
- **Recheck (`logs/recheck_part0..2.log`):** all 98 graded from the repo after landing. 1 MATCH (0x00566FE0) and 97
  RELOCATION_MATCH, every test PASS. NF50's own recheck (`naming/2026-11-17_agentNF50/recheck_final.log`) agrees.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 129 binding lines and added 68;
  - bindings_multi_check is at its baseline (`NullCoord`).
- **Local models:** 0x004759AD and 0x00A61270 keep their pre-existing local CCharString / CWideString models. fable_string.h lacks
  `CWideString::operator<`, and it is a root `#pragma once` header, which is not overlaid.

## Headers (opt-ins only; default path unchanged)
- **stlport/fable_map.h:** `FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE`.
- **engine/CVertexGroupBlock.h:** one opt-in, documented in `naming/2026-11-17_agentNF50/hdr/README.md`.
- **Overlay safety:** NF50 regraded two unrelated includers of both headers, 0x00AD9770 and 0x00AD9F10: both RELOCATION_MATCH,
  PASS.

## Rule slips
- Three Bash calls exited non-zero (greps with no match, a diff).
- One assertion-chained grade ran on a missing file; that grade was wasted.
