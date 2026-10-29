# ChatGPT pass 9 (external session, landed 2026-10-29): 157 of 2,157 reserved rows

Pass 9 (2,157 rows: 781 fresh under 1,024 bytes, 1,376 parked retries under 512) was reserved for an external ChatGPT
session and handed over without game files, retail bytes or links. It verified 157 rows (39,391 bytes); 374 are parked,
about 100 held or blocked, 1,523 never attempted (HANDOFF_FROM_CHATGPT.md, SUMMARY.json, STATUS.tsv).

Coordinator checks before landing:
- The delivery's SHA256SUMS (30,155 files) and its `verify_package.py` (read first, standard-library only, run with
  `python3 -I`) pass; no game files, binaries, oracle bytes or links.
- Module was the placeholder `Pass9` for every row: module and name are taken from each row's exact mangled leaf, with
  full template arguments from the package's MSVC undecorator output (55 template rows).
- Our own oracle cut from our Fable.exe (SHA-1 39fd416b...), and an independent dry `vland.py` regrade: 157/157 PASS
  (17 MATCH, 140 RELOCATION_MATCH); no `__asm` / `_emit` / `naked` / `volatile`.
- RELOC_CONFLICT on three rows; in each the pass 9 side is right and an older landed row is misnamed (handed to the
  naming pass): 0064a2db `CIntelligentPointer<const CThing>` ctor is 0x0056C860, not 0x004A96F0 (landed 0072cba0 binds
  it there); 009fd9b0 `CBankFileAsyncData::GetData` is 0x009D5530 and PeekData 0x009D5580 (Ego_r / FableWin declare Get
  then Peek, adjacent identical 77-byte bodies; our catalog labels were swapped, landed 00be04c0); 00cdd0a0
  `vector<bool>::resize` is 0x00467686, not `_M_fill_assign` 0x006860F0 (landed 006821e0).
- 0x0099F2E0 and 0x0099B150 replace bootstrap asm_bake oracles (moved to rebuild/integration/bootstrap_asm).

Landed with `vland.py --land` (verify.log). Labels: ghidra_out/labels_chatgpt_pass9_2026-10-29.tsv. The reservation is
released to our agents (owner, 2026-10-29); the unresolved rows' status is in pending/2026-10-29_chatgpt_pass9/
(STATUS.tsv, UNTOUCHED_RESERVED.tsv). ChatGPT's 465 NAME_FIXES (NAME_FIXES.tsv, 25 conflicts in NAME_FIX_CONFLICTS.tsv)
go to the naming pass; its ~900 MB of attempts, selections and receipts stay out of the repo.
