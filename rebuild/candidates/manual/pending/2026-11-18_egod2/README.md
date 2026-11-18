# In-house Ego_d lane, round 2 (2026-11-18, owner): 14 parked near misses for one author agent

**Returned and released 2026-11-18:** egod2a matched 2 rows, landed in landing egod2 (`../../batches/2026-11-18_landing_egod2/`):
0x007243C0 and 0x00B40260. 4 rows parked closer than before, with their best drafts in `return/differ/` (UNMATCHED_ROWS.tsv points at
them); `return/hdr_overlay/` holds the header opt-ins only those drafts use (not merged). 8 rows were not started. `RESERVED.tsv` is
now `RELEASED.tsv`.
Fixes for the next round: this folder's `lane/` lacked egod_locals.py / egod_type.py, so every `egod_locals.txt` the agent got was
empty (now copied). `lane/egod_all.py` (the agent's) prints EVERY record of a name: the stock tool prints the first only, which is
the wrong overload for constructors such as 0x00A229E0 (its real record is the 4th). The agent had no Agent tool and its advisor
call timed out, so no Fable reviewer saw the parked rows.

Owner: "Have a single opus 5.5 high agent working on more near misses that ego_d is likely to help match."

`RESERVED.tsv` lists the rows (UNMATCHED_ROWS.tsv: `reserved (egod2, in-house)`): the next 14 of the pass-24 ranking
(`../2026-11-18_codex_pass24/README.md`) after the 100 given to Codex -- each a named Ego_d row whose best draft is 2-3 bytes off
retail's size (register-tie, stack-slot/frame, x87-order or unclassified residue). One author, egod2a, works them from a package made
by `lane/mkagent_egod.py` (the 2026-11-17 lane's builder, reading that lane's EGOD_NAMED / EGOD_IDENTIFY lists) with
`lane/BRIEF_TEMPLATE.md` (that lane's brief, adapted to the tools on this machine: q.py grading, read-only vland for test and
relocations).
