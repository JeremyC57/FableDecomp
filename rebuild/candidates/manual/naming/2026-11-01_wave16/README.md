# Naming pass wave 16 (2026-11-01): 145 landed rows respelled to the proven c667-c674 names

Inputs: brief `naming/briefs/BRIEF_W16.md`, QUEUE_W16_input.tsv (the 184 NAME_FIXES_QUEUE rows c667-c674), wave 15 DEFERRED.tsv.
One Opus 5.5 agent authored the overlay in `$S/naming/w16` (AGENT_README.md is its own write-up; scripts/ here); the coordinator copied
ov_final (147 files: 145 sources + 2 rewritten tests), applied the 38 RENAMES (git mv + build_candidates.ps1), BINDINGS (36 RETIRE / 61 ADD / 7 KEEP)
and LABELS (19), and rechecked every changed row against HEAD: **148/148 (9 MATCH, 139 RELOCATION_MATCH) + PASS** (`logs/coordinator_recheck.log`).
`bindings_multi_check.py` 0; the repo bindings table equals the agent's `runs/reloc_bindings.after.tsv`. Name-only: no byte changed.
Catalog: the STLport rows keep the catalog's bare spellings (the agent's undname-style `class X` forms were not applied); GetAnimName and Unload_00a0b6c0 applied.

Families, committed in this order (`runs/<fam>.rows.txt`): gdt 53 (??_G stand-ins -> PDB ??_G + ??1; 17 CVtable_* modules renamed into NScript),
qctor 8, stl 6 (real STLport 4.6 text), own 14, data 4, sp 33 (CStringParser callback P6IX0@Z), queue 22, leaf 4, w15d 2, plus the three wave 15
leftover file renames. Queue outcomes (QUEUE_W16.tsv): 134 respelled, 12 respelled-partial, 16 deferred, 22 already-correct; 0x00440730 HOLD.
Decisions: `DECISIONS_W16_RESOLVED.tsv`. Deferred (DEFERRED.tsv, 230 rows): the partial rows' remaining stand-in callers, the destructor families that
need rewriting (0099a300 / 0099a2e0 / 0099a410 / 009fc550 / 00aa75c0 / 00f35b40), the PClass singleton 013b89fc (~60 rows), the 009aa780 sibling rows,
the wave 15 remainders and the mega families -> wave 17 with the c675-c678 queue rows.
