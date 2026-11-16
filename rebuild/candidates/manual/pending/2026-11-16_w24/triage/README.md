# w24 triage (2026-11-16): the closest free parked rows

Every free parked row in UNMATCHED_ROWS.tsv (1,162 after excluding all RESERVED.tsv rows, Codex pass 23 funclet parents, rows whose
extent overlaps a reservation (`excluded_overlaps.txt`) and the owner-parked 0x00F3E120) was regraded under the current headers
(`one.sh`, q.py; extents from BOUNDS_FIX.tsv, else the retail length in the row's grade, else length_estimate -- estimates are noted
in the `len_src` column). SRC_FAIL / NO_RESULT rows were regraded with their lane's `hdr/` (`one3.sh`, `triage_lane_overlays.tsv`).
`classify.py` gives status and class; `rescore.py` re-ranks by an instruction alignment (difflib), so one inserted instruction
counts once (`ranking_aligned.tsv`); `showd.py` prints a row's aligned diff. The oracle bytes stay in scratch (never committed).
Results: landings gh (5), gi (5), gj (1), gk (6). Caveat: built `push 0` / `mov reg,0` against a retail address is a relocation
the masking does not catch, so a few rows rank lower than they are. 0x00BA50A0 belongs to acctC's open TU (status set to reserved).
