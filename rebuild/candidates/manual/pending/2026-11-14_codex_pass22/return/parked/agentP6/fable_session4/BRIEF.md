# agentP6G -- finish 0x00F3E120 CTriangleStripifier2::FindBestStartVertex (owner, 2026-11-15: "finish matching 0x00F3E120")

Owner also said: "try hand authoring as well as your usual tools. do not rely only on automatic tools". FableWin.exe (the debug build)
is NOT available. Do not touch any reserved row (Codex pass 23: rebuild/candidates/manual/pending/2026-11-15_codex_pass23/RESERVED.tsv;
acctC). Repo /home/user/FableDecomp is read-only for you: no git, no edits.

Scratch `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`; your dir `$S/w22/agentP6G`.
Previous sessions (read first): `$S/w22/agentP6F/` LESSONS.md (the 0x00F3E120 lines), ATTEMPTS.log ("session 3"), work/hand/h*_00f3e120.cpp,
tools/README.md (grader g.sh, cnt.sh = constant ids/counts, f3ids.sh = root candidate ids + split pieces, hk.sh + sched/*.py c2 hooks:
tfree2.py free list, tu.py constant walk, t13.py + showdag.py scheduler DAG with priorities). Grade: `bash $S/w22/agentP6F/tools/g.sh
00f3e120 <abs src> "" -v` (about 1 s). Final check: one-row vland dry run (see $S/w22/agentP6F/BRIEF.md; oracle $S/batch/p6_oracle.tsv).

## Where it stands (coordinator, measured)
Two 2..4-line bases, each wrong in a different place:
- `base_pdb_order.cpp` (locals vertex_count, best_score, best_start_vert declared+initialised in PDB order): prologue matches; the
  reload pairs at 0x00F3E291 and 0x00F3E3C5 come out ebx-then-edi (retail edi=[esp+0x7c] then ebx=[esp+0x24]). 4 lines.
- `base_u1.cpp` (same declaration order, `long vertex_count;` uninitialised, assigned AFTER best_score/best_start_vert): both reloads
  match; only the prologue store order differs: ours [0x10]=0, [0x28]=-1, [0x18]=ecx; retail [0x18]=ecx, [0x10]=0, [0x28]=-1. 3 lines.
Mechanism (all measured with the hooks):
1. c2 creates constant syms in forward tuple order (ids). A constant with one use outside loops gets use count 0 and its record is
   freed at round 0 in hash order (bucket value&0x3F ascending, newest first in a bucket); root candidates this, vertex_link_map,
   vertex_to_poly_map, poly_buffer... pop the free list LIFO. Retail needs id(this) < id(vertex_link_map) after round 0, i.e. -1
   created before the divide's 0x2aaaaaab (u1), or a third freed constant X created after -1 with bucket 44..63, or -1 / 0x2aaaaaab
   kept (2 uses), or later: exactly one of constants 2 / 4 dropped at 0x107891E9 (that drop frees in ascending id order).
2. Prologue scheduler (one region): prio = ((height + outdeg) << 10), ready list prio desc then IL index asc. The three stores all have
   prio 0xc00 and succ only the jcc, so they come out in IL order. u1's IL order is s, b, [divide], c.
So a match needs either the u1 IL with the vertex_count store outranking (or preceding in IL) the other two, or the PDB-order IL with
-1 numbered first / an extra freed constant / one of 2,4 kept -- all with no extra bytes.
Ruled out (no change): __assume in 5 places; FABLE_RETAIL_ASSERT dead arms with FABLE_RETAIL_ASSERT_DEAD_ARM (pruned before the
walk); `~0L`, `(long)0xffffffffUL`, `& 0xffffffff`; `<= n-1` bounds (c1 folds or code changes); explicit byte division / pointer
difference for the size; const vertex_count; temporaries for the size; source pointer induction in the id loop (`corner`), `pl += 1`,
`begin()+pl`; all 6 declaration orders and declare/assign splits (store order always = assignment order). Byte-changing: `.empty()`
guards, `best_start_vert = start_vert; break;` in phase 3 (frame slots move), `return -1;` (HAND-15: everything to 0x00F3E695 equal,
extra `or eax,-1` epilogue).
Clue: the PDB line table puts the function at source line 1303, phase-1 CArray declarations at 1358-1360, BuildRings 1362, score 1371,
an explicit `continue` at 1386 and the fallback's declarations before BuildRings at 1404 -- the original is about twice as long as
ours (comments, or statements that compile away), so its IL may differ in ways a literal transcription misses.

## Your job
Hand-author. Read the retail disassembly (`g.sh ... -v` prints it) and write whole-function alternatives the way a Lionhead
programmer could have written them (helper locals that compile away, different loop/flag shapes in the fan walk, separate statements,
the size read as a CArray member, early `continue`s, etc.), checking each with g.sh and, when the reload or store order moves, with
cnt.sh / f3ids.sh / the DAG dump to see which link of the chain moved. Look for any byte-neutral construct that (a) gives the
vertex_count store a higher scheduler priority or lower IL index than the two constant stores in u1, or (b) changes the constant
numbering / free order in the PDB-order base. Keep all output in your dir: ATTEMPTS.log (every grade), LESSONS.md (GOTCHAS one-liners),
and on a match final/00f3e120.cpp + final/00f3e120_test.cpp (test: copy $S/w22/agentP6F/start/00f3e120_test.cpp, mirroring the body)
that pass the vland dry run. SendMessage the coordinator (main) only on a match or a blocker. Final message: short report.
