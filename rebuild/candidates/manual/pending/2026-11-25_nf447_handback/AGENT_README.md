# NF447 b1 (land_scripted run dir): CSalRenderPanDelay / SSE2 / MMX, 23 rows, one unit (00c55270)

Rows (rows.txt): 00c55070 00c550d0 00c55270 01216180 0121618b 01216196 012161a1 012161ac 012161b7 012161c2 012161cd 00c6ca20 00c6cac0
00c6cad0 00c6cc60 00c6d120 00c6d150 00c6d560 00c6d630 00c6f300 00c6f3e0 00c6cef0 00c6cf40. RESULTS.tsv: 23 ACCEPT.
Option S members (UNITS kind S): 00c55070 / 00c550d0 (owner 00c55270), 00c6cf40 (owner 00c6cef0). Funclets / 00c55270 carry `; target 00c6ca20`.

Files: rows/, tests/ (all linked against the row object), hdr/engine/ (3 NEW headers + CSalOutputKS.h opt-in; base copy
orig/hdr_CSalOutputKS.h), HEADERS.tsv, RENAMES.tsv (14 renamed rows; file names unchanged), CATALOG_CHANGES.tsv, TABLE_CHANGES.tsv
(6 replace + 1 delete, witnesses are run rows), SELF_SYMBOLS.tsv, UNITS.tsv, BEST_GUESS.tsv (empty: every name has two sources),
NAME_FIXES.tsv (2 IAT lines, other rows' spellings), head/ (HEAD payload + oracle, 23 rows), orig/ (HEAD rows + tests), evidence/.

Gates, HEAD v new (evidence/):
- strict verify_and_land --recheck-landed (landing simulation, LANDVERIFY_STRICT_HEADERS=1): new 20/20 PASS + option S strict leaf 3/3 PASS
  (vland_new.log); HEAD 22/23 (00c6d120 HEADER_POLICY) (vland_head.log).
- tests 23/23 PASS (tests_23.txt); mutants 20/20 change bytes and FAIL their test (every renamed self, incl. owner-site mutants for the
  thin rows 0x00C55070 / 0x00C550D0, which drop the ctor COMDAT), + 1 equivalent mutant marked as such (mutants.txt).
- pickany_vs_retail + pk_all: 0 NEQ HEAD and new on every row; reachflip 15 pairs, 0 flips (gateall_23.txt, pk/). New pick-any / strong
  copies EQ at their homes (pk_all --addrs): ??_G 00c554c0 / 00c55550 / 00c6d350, ??1 00c554e0 / 00c55570, ??0 00c55070 / 00c550d0,
  UpdateFilters 00c6cf40.
- vt432 (VT_SELFMAP): HEAD 0 agree / 20 WRONG / 2 unknown -> new 32 agree / 8 WRONG (all slot 0: the pre-existing ??_G stand-ins).
- /FORCE link: NF445 16-object set 36 -> 36 unresolved (spellings only); slots set 54 -> 45 unresolved, duplicates 3 -> 2, no UND+ / DUP+.
- apply_table --dry: replace 6, delete 1, misses 0, multi-target 1 -> 1. reserved_check 0 (23 rows). descriptive_check --extra 0 FAIL, but
  vacuous: BEST_GUESS.tsv is header-only, and descriptive_names.tsv has no line for any b1 address and no GameCODA SDK names registered.
- includers: the only landed includers of the two owner files are the 8 funclets, all in the run (no outside regrade).
- name_gate: FAIL lines 124 -> 69, LOCAL_CLASS 78 -> 17, standin 25 -> 7, SHARED_HEADER 2 -> 0 (name_gate_diff.txt). New FAIL lines are
  only: self agree_unverified (SDK names), split_unknown at batch-owned addresses that TABLE_CHANGES resolves, RELOC_NAME catalog labels
  RENAMES fixes, and LOCAL_CLASS CAudioMaster in 0x00C6CEF0 (moved there from the HEAD 0x00C6CF40 text of the same option S slice). CAudioMaster
  is a local view of a class that DOES have a header: engine/CSalPlatform.h (SDK member m_nSWOutputChannels; `extern CSalPlatform* g_pSal`,
  already bound at 0x0143BC2C in reloc_bindings, witness 0x00C5ACB0). All five users (0x00C6CC60 / 0x00C6D150 / 0x00C6D630 / 0x00C6F3E0
  and the 0x00C6CEF0 slice) carried it in HEAD; not re-ported here (follow-up: whether the header puts m_nSWOutputChannels at +0x1BC is
  unverified). The remaining LOCAL_CLASS lines (CVtable_012bed90 / 8a78 / 8ab4 / 8a8c / 8ac8 / edb8 / edcc / ede0 in 0x00C55270,
  SDelayTap, SDirectionFilter, CAudioMaster) are HEAD debt (held families; SDelayTap / SDirectionFilter have no header). Funclet rows report "row does
  not match" in HEAD and new alike (name_gate grades the funclet against the parent; a tool limitation).
