# C2T1: lane tools agent (2026-11-22): rebuild the c2 CSE-temp-id comparator readout

Advisor ruling 2026-11-22 (docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md, last entry): a bounded tools task, not authoring.
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs ONLY under `$S/w22/c2t1/`; the coordinator
commits the finished tools under `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/`. Never write into /home/user/FableDecomp; never run git,
not even read-only. Wait on jobs by PID (kill -0), never pgrep -f. No rm -rf on variable paths. Disk is tight (~3.5 GB free): keep logs small.

## What existed (acctC lane, never committed)
`rebuild/candidates/manual/pending/2026-11-15_acctC_r6_wfE6/NOTES.md` lines ~160-185 and LESSONS.md (wfE6-4..8, the "(tools)" line):
wfB3 `tools/lkb2.py` on c2's comparator log (copied as tools/lk + lka.sh), the rule B = [2X mod 2048 >= 2Y mod 2048] with X = first-pass
CSE temp id and Y = late temp id, `fsolve.py <lk.json> <first> <last> <flips>` solving uniform shifts F / T, and the quick probe (a plain gdb
breakpoint on the comparator with ignore_count K, then M logged hits). Also wfB4 `k/cmp.py`. Read those first.

## What the lane has
`tools/decomp_pipeline/linux_lane/lane_tools/c2probe/` (c2dbg.py, c2prio.py, cfg.py, explore1/2.py): the c2 hook kit AUTH3 / AUTH4 used
under wine. Earlier hook uses: `pending/2026-11-21_auth3/`, `_auth4/`, `_auth7/`, `_auth16/`. Find the comparator in c2.dll from these.

## Deliverable
1. `lk` (log the comparator calls for one compile: site, X, Y, B), `lka` (align our log with a reference row's sites), `fsolve` (the
   uniform-shift solver), each with a docstring and a README section.
2. **Acceptance test (the advisor's):** on 0x00B4D370, AUTH20's p8 and p9 (`$S/w22/auth20/00b4d370/x/p8.cpp`, `x/p9.cpp`, flags
   `/d2inlT314`, overlay `$S/w22/auth20/00b4d370/ov/`; copy them, do not edit) differ in one x87 operand-order site. The tool must show that
   site's bit flipping between p8 and p9 and no other bit changing. If it cannot, say so plainly: then the x87-sites-follow-the-id-rule claim
   is wrong for this row.
3. Report the readout for p9's remaining x87 sites (AUTH20 ESCALATE.md lists them) as data only. You do not author candidates.
Hand back: tools/, README.md, ACCEPTANCE.md (the p8/p9 result), LOG.md.
