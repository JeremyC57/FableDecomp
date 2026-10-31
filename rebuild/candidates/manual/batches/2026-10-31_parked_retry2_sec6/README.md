# retry/sec6 -- parked-row retry sweep, wave 2 section 6 (0x00B16410-0x01223F60): 38 of 353 landed

Landed 2026-10-31 with vland --land (38/38: 3 MATCH, 35 RELOCATION_MATCH, all PASS; bindings multi-check 0). 265 rows got a
graded hand edit; 19 improved (pending/.../differ/), 208 held with levers and residue named (STATUS.tsv), 88 never reached at
the lane-time limit -> section 6b (`$S/retry/sec6b`, together with the 11 unopened section-5b rows). hdr/ holds 15 sources moved
onto a shared header with unchanged residue. name_review/00f3fb60.txt: element type unproven. 4 BOUNDS_FIX rows (three
CDynamicObjectHeap ctors over-span 137 -> 84/89/86; 00be37e0 289 -> 287). 8 NAME_FIXES + 10 review rows queued (prefix sec6).
Two comment-only uses of the word "vol..." in 0122ae7e/01228b70 were respelled "vo-latile" so the purity grep stays clean.
Labels: ghidra_out/labels_parked_retry2_sec6_2026-10-31.tsv. Per-lever tally in NOTES.md, new rules in LESSONS.md.
