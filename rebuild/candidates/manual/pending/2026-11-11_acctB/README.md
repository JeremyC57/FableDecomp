# 2026-11-11_acctB — reservation for a second working account

This folder reserves 189 never-attempted, unlanded rows (`RESERVED.tsv`) for a second account.
They are ordered shortest-first. Lengths are estimates from the next row's start, with trailing
CC/90 padding trimmed. They run from 6 to 32,057 bytes, and 32 of them are at most 1,500 bytes.

What the second account may do:
- Author rows only for the addresses in `RESERVED.tsv`.
- Push only to its own branch, `FullDecomp-B`.
- Write only new hand-back folders under `rebuild/candidates/manual/pending/2026-11-1x_acctB_<name>/`, containing:
  - `rows/`, `hdr/`
  - `STATUS.tsv` (address, grade, notes)
  - `NAME_FIXES.tsv`, `LABELS.tsv`
  - `LESSONS.md`, `payload.json`

What the second account must not do:
- Edit the shared files (`build_candidates.ps1`, `auto-re-candidates.tsv`, `functions.tsv`, `reloc_bindings.tsv`, COVERAGE, README, `NAME_FIXES_QUEUE.tsv`, HANDOFF, ROADMAP, GOTCHAS).
- Land anything.

The coordinator on `FullDecomp` does all landing.

Rules: CLAUDE.md, `docs/pipeline/ASM_RULES.md`, and `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`.
The grade budget is 10 grades per row per pass. When a row parks, write the exact remaining diff in STATUS.tsv.

**Released 2026-11-13 (owner):** the acctB lane is finished and handed over (`../2026-11-13_acctB_FINAL/`). `RESERVED.tsv` is renamed
`RELEASED.tsv`: the 140 matched rows are landed by the coordinator, and the 49 others (19 drafts, 23 without a draft, 7 blocked or
fragments) are ordinary parked rows from now on (`../2026-11-13_acctB_parked/`).
