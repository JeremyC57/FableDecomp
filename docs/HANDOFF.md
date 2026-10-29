# Resume here (one page) — updated 2026-10-29

**Next session: read `docs/journal/2026-10/2026-10-29_HANDOFF.md`** (state, rules in force, resume steps, open items).
Fresh container: lane setup in `docs/pipeline/FRESH_SESSION_HANDOFF.md`; coordinator commands in
`tools/decomp_pipeline/linux_lane/lane_tools/coordinator/README.md`. Older one-page handoffs: `docs/journal/HANDOFF_ARCHIVE.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`), clean and pushed; no agents running.
- **51,642 of 57,267 known functions (90.2%)**, 44,687 of the original 49,568 (90.1%); 38.1% of function bytes, 34.0% of
  .text (`rebuild/COVERAGE.md`, `tools/landed_metrics.py`; never hand-edit). The functions left are the large ones.
- Retail compiler: cl 13.10.3077 (Rich header: 1,369 objects; 11 by QFE 4035; linker 3052). Lane default 3077, `--qfe` 4035.

## Done 2026-10-28/29 (details in the session handoff and `docs/journal/FINDINGS_LOG.md`)
- Chunks 561-574 (never-tried and released pass 9 rows; 568-574 were ~1-2 KB rows): quest-script rows landed 20 of 21.
- Naming pass: operator=, waves 1-8, LowerBound (records `rebuild/candidates/manual/naming/`); wide text stays
  `unsigned short` (owner, 2026-10-29). ChatGPT pass 9: 157 landed, the rest released to our agents.
- Progress reporting now from live landings (`tools/landed_metrics.py`).

## Next (owner picks the order)
1. Large-row chunks from `rebuild/candidates/manual/pending/2026-10-29_chatgpt_pass9/UNTOUCHED_RESERVED.tsv` (quest-script
   range first); at most two agents at a time.
2. Naming wave 9: `naming/NAME_FIXES_QUEUE.tsv` chunks 566-574 (145 rows) + wave 8's 267 open decisions.
3. Near misses: 0x00919AE0 (2217v2220), 0x00E29B20 (856v846), 0x00BEB6C0, 0x00F3E6B0; seven catalog rows to re-split.
4. Older open work (one-address-one-name remainder, CRetailTree remainder, recheck sweep, retail_asm candidates):
   `docs/ROADMAP.md`.
