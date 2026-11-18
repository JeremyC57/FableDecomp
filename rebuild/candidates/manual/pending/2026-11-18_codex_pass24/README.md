# Codex pass 24 reservation (2026-11-18, owner): 100 parked near misses that Ego_d.pdb can help

Owner: "Get a hand off zip ready with 100 near miss rows that ego_d has a good chance of helping. And include drive link to pdb in
handoff. This is for codex".

`RESERVED.tsv` lists them (`UNMATCHED_ROWS.tsv` marks them `reserved (pass24, <kind>)`); they are off-limits to every other lane until
it is renamed `RELEASED.tsv`. No other reservation is open.

## Selection (`rows.tsv`, fixed)
From the 632 named rows of the released Ego_d list (`../2026-11-17_egod_parked/EGOD_NAMED.tsv`, every one with a complete Ego_d locals
record), still parked, with a draft on record and a retail length of 24-4,000 bytes:

| kind | rows | what they are |
|---|---:|---|
| egod_parked | 35 | the Ego_d lane's own near misses: worked WITH Ego_d on 2026-11-17 and parked one named residue short (34 same size) |
| egod_open | 65 | parked before Ego_d: the draft is 0-3 bytes off retail's size (7 same size), at least two named Ego_d locals, residue class register-tie / stack-slot / x87-order / block-layout / incomplete-model or unclassified; ranked by size delta, then class, then named-local count |

Left out: the two Ego_d lane rows that matched but are held (0x009D8650 naked helper, 0x009FDE80 unaudited), one lane row parked far from
size, and rows whose residue is flags, inline-vs-call, bounds or EH (Ego_d does not speak to them). Every row's recorded retail length
equals the bounds extent scan.

Seven rows' `closest_version` pointed at a behaviour test, not a source; this commit points them at the draft in the same folder
(0x00918A40 at the newer handQ13 draft). 61 other open rows still have that defect.

## The package
`build.py` builds `fabledecomp_codex_pass24_2026-11-18.zip` in the scratch area (not committed; about 30 MB, 7,040 files). Needs `FABLE_EXE`
(extents and callers only), `EGOD_PDB` / `EGOD_TYPES` (an `llvm-pdbutil dump -types` text) to write each row's `egod_locals.txt`, and
`EGOD_PDB_LINK`, the Drive link that goes into README_CODEX.md / PROMPT_CODEX.md (`@@EGOD_PDB_LINK@@`; the link is not committed).
Writes no retail bytes and no PDB. Layout as pass 23 (`../2026-11-15_codex_pass23/README.md`) plus, per row, `egod_locals.txt` (named
params / locals / scopes / statics with resolved types) and `egod_tu.txt` (the row's object file in Ego_d order with landed retail
addresses), and `egod/` (findings, the lane's brief, lessons, ledger, every batch's notes). README_CODEX.md "Ego_d.pdb" turns the
lane's lessons into the attempt order.
