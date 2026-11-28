# AUTH30B brief (2026-11-27): continue AUTH30 under the advisor's final rulings

You replace agent AUTH30, whose context ran out. Your locks are AUTH30B in `$S/w22/nflocks/` (16 rows in rows.txt, plus 00d04ae0,
00d05910, 00d05fe0). Read, in this order:
1. `$S/w22/auth30/BRIEF.md`: environment, tools, grading, hard rules. All of it still applies.
2. `$S/w22/auth30/PAIR_FACTS.md` and `STATUS.tsv`: what AUTH30 found. Ignore any R1 wording in PAIR_FACTS round 3 that conflicts
   with the rulings below; the rulings win.
3. The rulings: the last section of `/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`
   ("2026-11-27 advisor decision (owner delegated), AUTH30").
4. Names: `$S/codex30/NAME_ANSWERS.tsv` (answer key) and
   `/home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-27_codex_pass30/NAMING_FIRST_TIME.md`. Every symbol you write is
   spelt from NAME_ANSWERS; anything UNSETTLED is settled from two sources (PDBs in `/tmp/claude-0/egod_pdb/`) BEFORE you write it. The
   owner wants no new name-queue lines: any landed spelling you contradict, report in NAME_CONFLICTS.tsv with both sources. Naming work
   beyond these rows is paused (owner): do not rename anything else.

## Hard rules (unchanged)
Never write into /home/user/FableDecomp; never run git; never run `wineserver -k`; give every Bash call a plain description;
`apply_table.py` only with `--dry` or `FABLE_ROOT=<sandbox>`. No `__asm`/naked/`_emit` in row sources or tests. Shared headers only:
header edits go in your overlay `$S/w22/auth30/hdr/` as add-only opt-ins. Never edit T3_PORT_QUEUE text.

## Tasks, in order
1. **0x00D06130 pick-any "2/4 EQ" (blocking).** Name the two NEQ copies, their owners' retail addresses, and why they differ. Fix it if
   a header-route text does; else say exactly what blocks.
2. **R1 header route, both Main rows.** Body in `engine/CQ_BanditCampBossBattleScript.h` behind the existing opt-in
   `FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_INCLASS_MAIN_PASS30`, marked `// Inferred:` (Ego_d .hpp lines 997 / 1053; FableWin inline flag with
   the register_scripts.cpp control). No row-local `inline`. Drop `inline_depth(2)` and the counted-pointer specialisations; regrade.
3. **Control:** grade 0x00D04AE0 (landed CAssassin1::Main) on the same route without `/d2inlS15`. Report its grade.
4. **R2:** port the landed Alloc rows 0x00D05910 and 0x00D05FE0 onto the header (nf88 form: Alloc and the ctor in the opt-in, a helper
   takes Alloc's address; GOTCHAS nf135 / nf140). Each port must grade MATCH / RELOCATION_MATCH with its test and pick-any. Only if a
   port DIFFERs, use the constructing helper and say so. If Main matches only under the constructing helper, stop: the pair waits for
   object mode.
5. **Witness retries:** 009F3DA0 and 009F4590 against the 36-byte `??_GCSurface` witness 0x004979A0; 007D75A0 against a landed row that
   emits the 77-byte `??4CDefClassBase`. Then the other held rows, at most 25 grades each.
6. **No strong duplicates (R3):** no row of yours may strongly define a function another landed row owns; declare it from its header.
   Check every row object with dumpbin /symbols.

## Return
Update `$S/w22/auth30/STATUS.tsv` (one line per row: state, best grade, gates), write `$S/w22/auth30/ROUND4.md` (findings per task,
with grades and evidence), `NAME_CONFLICTS.tsv`, and for each row ready to land a folder `$S/w22/auth30/land/<addr>/` with the row
source, test, header overlay diff, LEAF and flags. Final message: a short table of rows and what the coordinator should land.
