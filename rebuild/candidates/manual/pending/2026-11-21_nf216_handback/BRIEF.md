# Name-fix agent NF216 (2026-11-21, round 10): the CBasicString<char>::operator==(const char*) caller copies (family pass, rows 1..14)

Owner (2026-11-21): "you can continue the naming queue agent too". You are the ONLY naming agent; authoring agent AUTH3 runs at the same
time (locks `AUTH3`; its overlay `$S/w22/auth3/` read-only; earlier authoring overlays are per row under `$S/w22/auth1/<addr>/hdr/`,
`$S/w22/auth2/<addr>/hdr/`). Account A = branch FullDecomp.

## Your brief is NF215's, which is NF214's with overrides
Read `$S/w22/agentNF215/BRIEF.md` (its "Lane" section replaces NF214's and applies to you unchanged) and then
`$S/w22/agentNF216/NF214_BRIEF.md` in full with everything it says to read first. All of it applies, with every `agentNF214` /
`agentNF215` path -> `$S/w22/agentNF216` and `NF214` / `NF215` -> `NF216`. Where they conflict: this file > NF215 BRIEF > NF214 BRIEF.
Also read NF215's hand-back (`$S/w22/agentNF215/`: NOTES.md, LESSONS.md, STATUS.tsv, work/definers.tsv, work/census.tsv): it is the
census your slice comes from. Lane caveat (FINDINGS 2026-11-21): a catalog entry with a bare CompilerFlags value (e.g. '/d2inlS12') makes
recheck.py add /Oy- and grade DIFFER: grade such rows with q.py and the row's own `// Flags:` line.

## State of the repo
nf215 (NF215's six rows and its fable_string.h opt-in `FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO`) is being committed by the
coordinator now; the working tree already has it. Base every overlay copy on the CURRENT working-tree header. 0x004115A0 now has the PDB
self leaf `??8?$CBasicString@D@@QBE_NPBD@Z`; 0x00859040, 0x004AFE20, 0x006238A0 bind it.

## NF216 specifics (ENTRIES.tsv: q12378 = the family leftovers line, q12374 = the 0x004115A0 name-fix line)
The unit is a ROW. Rows (locked NF216, rows.txt): the caller rows of 0x004115A0 whose object defines an `operator==(const char*)` copy
that differs from the owner's 88-byte body (NF215 work/definers.tsv `caller=1`, minus the three it ported):
0x004A0D90 0x004FDCB0 0x004FE630 0x005A04F7 0x005A4CDD 0x005A5B31 0x0065A075 0x007D2CC0 0x00870E00 0x00870FC0 0x00871290 0x009ACEE0 0x009AF1E0 0x00CBFB7D.
- Proven route (NF215 on 0x006238A0 / 0x004AFE20): port the row onto fable_string.h with `FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO`
  + `FABLE_RETAIL_CBASICSTRING_COMPARE_BODY`; the pick-any copy becomes the owner's 88 bytes (equal in bytes AND relocation targets: check
  each one). Prove it by hand on 2-3 rows, script it, grade the rest in one payload.
- A row whose leaf breaks under the route: at most 3 forms, then `blocked` with the diff. Rows that already define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE
  (0x004A0D90, 0x007D2CC0, 0x009ACEE0, 0x00CBFB7D) swap that macro for the COMPARE_ZERO one.
- 0x004FE630 is the namespaced stand-in `??8?$CBasicString@D@X_004fe630@@QBE_NPBD@Z` (NF215 NAME_FIXES): its port also removes that split.
- No new split: every row you touch ends binding `??8?$CBasicString@D@@QBE_NPBD@Z` for 0x004115A0 (and the PDB `?Compare@?$CBasicString@D@@KIJPBD0@Z`
  for 0x00411570 if it binds Compare).
- Out of scope: 0x006D41F0 / 0x006D4180 (blocked, header claim), the 40 TEXT_EQ_INLINE definers, the dead-copy definers (caller=0).
Report briefly when the slice is done or your context gets heavy (hand-back files current first).
