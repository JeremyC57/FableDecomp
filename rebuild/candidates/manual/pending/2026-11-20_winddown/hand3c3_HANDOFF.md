# hand3c3 (chunk 3, Account B) -- state at the owner wind-down, 2026-11-20

The 20 rows of this chunk: 14 matched, 6 open. Everything is under this workspace; nothing was written to the repo.
- LANDED (coordinator): b1 0x009ACEE0, b2 0x00A4C1F0 / 0x00A52760 / 0x00A53860, b3 0x00B075D0, b4 0x0078A690, b5 0x0077A430,
  b6 0x0079D360, b7 0x0057C4F1, b8 0x00A781A0.
- READY, awaiting audit / landing: batches/b9 0x0041AFA0, b10 (rev 2) 0x0077D5B0, b11 0x00B16410, b12 0x0078D250. Each batch has
  README, src, test, mutants, hdr + hdr.diff, payload.json, oracle.tsv, vland.log, NAME_FIXES.tsv. Their overlay headers are live in
  hdr/engine (list: hdr/README.md, last section).
- PARKED: 0x006E5A00 -- parked/006e5a00/README.md (draft does not compile yet; never graded).
- Not started in this chunk: 0x00A475C0, 0x00B079B0, 0x0087CFE0, 0x008D5AA0, 0x00988E80 (NOTES.md "WIND-DOWN").
Resume: `source $W/tmp/pre.sh; cd $W; export W` before every command (tools in $W/tools; see NOTES.md).

---- (older hand-off text of the workspace this one was seeded from follows) ----

# hand3c workspace hand-off (2026-11-19, owner wrap-up order)

Workspace: `W=/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3`. It holds the reserved
"not plain C++" rows: hand-authored, byte-matched under VC7.1, behaviour-tested in the strict vland.
The detail lives in these files:
- `STATUS.tsv`: one line per row with grade, bytes, test, grades used and notes.
- `NOTES.md`: per-row write-ups. Its last section, "Wrap-up state", lists every touched row that did not land.
- `ATTEMPTS.log`: every grade.
- `LESSONS.md`: the levers that worked.
- `NAME_FIXES.tsv` / `LABELS.tsv`: proven names.
- `hdr/README.md`: every overlay opt-in.

## Done
- 51 rows are MATCH / RELOCATION_MATCH / DONE in STATUS.tsv.
- Batches b1..b22 were handed back and are landed in the repo; `git log` shows "land: ... from hand3c" up to AddToBoundingSphere.
- **b23 READY, not yet landed:** 0x00C40590 CCodaListener::Commit, RELOCATION_MATCH, vland PASS. It is in `batches/b23/` with its src, test, CCodaVector.h overlay, diff, payload.json and oracle.tsv.
- Pass pattern: `<addr>_TEST PASS`.

## Remaining reserved rows
**Open with residue.** Best source, residue and next idea for each are in NOTES "Wrap-up state":

| Row | Function | State | Best source | Residue |
|---|---|---|---|---|
| 00986210 | CallingChildrenIsRunnable | DIFFER 461v461, 14 grades | `src/` | 3-instruction schedule at 0x00986374 |
| 00d8a240 | WatchForBooty | DIFFER 610, 29 grades | `src/` | ~CScriptThing expansion site map |
| 00a4d770 | CalcLocalTransformationsRelative | DIFFER 420v420, 42 grades | `src/` | lea operand order at 0x00A4D8F1 |
| 00989230 | — | escalated, ~88 grades | `differ/` | see `keep/ESCALATE_00989230.md`; same class as 00988e80 / 00ae1320 / 00ae29d0, which are NOT to be graded |
| 0063f4b0 | — | DIFFER 769, 30 grades | `differ/` | — |
| 00ac7eb0 | — | DIFFER 215, 66 grades | `differ/` | — |
| 008a8260 | — | DIFFER 456v466 | `differ/` | — |

**Untouched beyond a baseline grade.** Drafts are in `ref/<addr>/draft.cpp` or `prior_*.cpp`, with prior notes in `ref/<addr>/prior_notes.txt`. Byte sizes are in parentheses:
- 0041afa0(1455) 0057c4f1(1192) 00591b7f(735) 00646762(740) 0064dd9a(580) 006e5a00(1772) 00766660(696)
- 0077a430(1115) 0077d5b0(1491) 0078a690(1097) 0078d250(1729) 0079d360(1115) 0087cfe0(1447) 008d5aa0(1126)
- 00988e80(917, do not grade) 009acee0(1017) 009bf7e0(846) 009c3c20(909) 00a475c0(1857) 00a48ba0(875) 00a48f10(811)
- 00a4c1f0(1006) 00a52760(1004) 00a53860(1005) 00a567a0(766) 00a781a0(1244) 00a793d0(678) 00a79680(678)
- 00aabf60(620) 00aad340(584) 00adf960(766) 00ae1320(681, do not grade) 00ae29d0(726, do not grade) 00b075d0(977)
- 00b079b0(2032) 00b16410(1671) 00b51ad0(969) 00b53f60(411, needs 0x00B51AD0 reconstructed first in the same TU)
- 00b99300(871) 00ba1140(2344) 00bade40(793) 00bd71b0(2116) 00c41fb0(1570, GameCODA range) 00d6c830(607)

**Coordinator end-of-queue item, not started:** port 0x009C31E0 Pause with 0x009C3590 / 0x009C3660 onto `engine/CFileInstaller.h` (FableWin
0x030495F0 / 0x030499D0), and remove the `?DirtyDisc@CFileInstaller@@2_NA` binding (the PDB has `@@1_NA`).

## Overlay headers in hdr/ not landed yet
These are copies of the current repo header plus new `#ifdef FABLE_RETAIL_*` opt-ins. The default path is unchanged and the asserts are pinned.

| Header | Opt-in | Row |
|---|---|---|
| `engine/CCodaVector.h` | CCODAVECTOR_ORTHOGONALRIGHT | b23 0x00C40590, READY |
| `engine/CAIStateGroup_OpinionReactionRoot.h` | ..._CALLINGCHILDREN | OPEN 00986210 |
| `engine/CQ_HerosOldHouseScript.h` | ..._WATCHFORBOOTY | DIFFER 00d8a240 |
| `engine/C3DAnimation2.h` | CALCLOCALRELATIVE | OPEN 00a4d770 |
| `engine/C3DMesh2.h` | GETBONEKEYFRAME | OPEN 00a4d770 |

Overlays already landed are backed up in `hdr_landed_backup/`. Rebase on the repo copy before reuse.

## Tools (tools/)
Bash prefix:
```
export S=/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad W=$S/w25/hand3c3; cd /home/user/FableDecomp; source $S/wine/env.sh; export FABLE_HDR_OVERLAY=$W/hdr
```
- `g.py <addr> <src> "<idea>" [--flags ..] [--full]`: one grade. It prints a masked diff and appends to ATTEMPTS.log. It reads the `// Leaf:` and `// Flags:` lines.
- `tryv.sh <addr> <leaf> <src> "<idea>"`: grade, then dump the build to tmp/m.txt.
- `dd.sh <addr>`: diff tmp/m.txt against `ref/<addr>/retail_dis.txt`. The retail_dis files were deleted at wrap-up. Regenerate one with `python3 $W/tools/pedis.py "$FABLE_EXE" <va> <hexlen> > ref/<addr>/retail_dis.txt`.
- `vland.sh <addr...>`: runs mkpayload.py, which writes payload.json / oracle.tsv for those rows, then the strict vland. Log in tmp/vland.log.
- `mkpayload.py`, `setstatus.py <addr> <grade> <bytes> <test> <n> <notes>`, `zs.py` (front-end /Zs check), `cc.py` (compile only), `vs.sh` / `sweep.py` (grade many variants).
- `probe.py` (x87 term orders, no grade), `pedis.py` (retail disassembly), `fwcalls.py` (FableWin calls), `egor_dis.py` (Ego_r with names), `rdconst.py` (float constants), `dumpobj.py`, `baseline.sh`.
- `sched/` holds the c2 DAG scheduler simulator used for 0x00C1C330 (t20.py + sim.py).
- **`codaobj.py <lib> <symbol>`** (new): finds a function in a COFF .lib (e.g. GameCODA `CODAApi.lib` / `CODAApiDbg.lib` at `$S/sdk/gamecoda/tree/CODA/lib/Win32`) and prints its member, size and relocations by name.
- `probe/c40/pp.py <src...>`: a no-ATTEMPTS compile+diff harness, hard-wired to one row (`addr`, and `LEAF=` env). Copy it to sweep another row cheaply; it takes about 0.1 s per compile.
- Repo `tools/decomp_pipeline/linux_lane/lane_tools/q.py <addr> <src> <leaf>`: grades a same-TU helper at its own address.

## Key lessons (full list in LESSONS.md)
- **GameCODA range (0x00C3xxxx..0x00C8xxxx):** byte-search the SDK `CODAApi.lib` first. The release member gives the names. The Dbg member gives the statement shape. Toolkit source (`CtkVector.h` etc.) gives the header-inline bodies. That closed 0x00C40590 in one grade after 80 grades of sweeps failed. Next in range: 00c41fb0.
- Write rows in the FableWin statement shape on the shared headers. Making same-TU helper or template bodies visible fixes frame slots.
- Debug ASSERTs from FableWin can act as ordering levers.
- Brackets are allowed on scheduling residues (owner rule 2026-11-19).
