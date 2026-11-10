# Codex pass 16 (2026-11-10): 40 parked near misses

Handed to ChatGPT Codex as `fabledecomp_codex_2026-11-10.zip`, built by the coordinator's package builder: the pass-15 builder adapted
to parked rows only. The zip holds no retail bytes; Codex cuts them from its own Fable.exe with `tools/cut_row.py`. The rows are
in `RESERVED.tsv`: the 40 closest of the 358 small parked drafts, regraded on 2026-11-10 against today's headers, at 0-7 differing
instruction lines after masking. Excluded: rows that have landed since, and rows held by in-house agents. Those are the two quest-bar
DrawAt rows, HenchmanCommentary, the three CTCInventory screens, the set / map / list instantiations of Codex fix [5]-[10], and 0x0099BDB0.
In-house agents leave these 40 rows alone until the return lands (`group_remaining.py --exclude .../RESERVED.tsv`).
The README and prompt given to Codex are `README_CODEX.md` and `PROMPT_CODEX.md` in this folder.
