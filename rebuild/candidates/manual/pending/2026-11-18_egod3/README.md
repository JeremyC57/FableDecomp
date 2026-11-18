# In-house Ego_d lane, round 3 (2026-11-18, owner): 12 rows for one author agent, hand authoring first

**Returned and released 2026-11-18:** egod3a matched 3 rows, landed in landing egod3 (`../../batches/2026-11-18_landing_egod3/`):
0x00A6C640, 0x00A229E0, 0x00803280. The other 9 are parked with drafts in `return/differ/`; 0x00BE2CD0 and 0x0059FEE3 have
escalation notes for a Fable review in `return/escalate/` (each had only 3-4 grades this round). The other 7 are still old local-model
drafts that need porting onto shared headers (LESSONS.md: copy a landed same-TU sibling's define / include block first).

Owner: "start another round. dont forget that hand authoring is important".

`RESERVED.tsv` (UNMATCHED_ROWS.tsv: `reserved (egod3, in-house)`): round 2's 12 unlanded rows -- the 4 it parked closer
(0x00A6C640 and 0x00A229E0 same size, one site each; 0x00BE2CD0, 0x0059FEE3) with its drafts and header overlay, and the 8 it did not
start. Author egod3a works from `lane/mkagent_egod.py` (now: today's best draft from UNMATCHED_ROWS first; `egod_all.py` so every
Ego_d record of an overloaded name is listed) and `lane/BRIEF_TEMPLATE.md` ("Hand authoring first" section; up to about 100 grades a
row in pass 1; a stuck row goes to a Fable review, run by the coordinator when the author cannot).
