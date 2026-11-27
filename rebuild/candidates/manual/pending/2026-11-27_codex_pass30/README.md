# Codex pass 30 (2026-11-27): the 100 largest parked rows, names right the first time

**Owner (2026-11-27):** "Create new codex handoff with 100 more largest parked rows. Ensure the handoff contains the files and tables that contain accurate names so the codex doesn't accidentally use wrong names. And make sure to include rules to accurately name the first time so no queuing is needed."

## The rows
`rows.tsv` and `RESERVED.tsv` hold 100 rows, 120,543 B in all, 752 to 2,288 B each. The claim line is in COORDINATION_CLAIMS, and the locks are CODEX30 in nflocks.

**Selection:** `tools/select_rows.py --allow-ungraded --order longest --n 100 --locks <nflocks>`. These are the largest parked rows that are:
- not landed, locked or claimed;
- not a funclet;
- not on record as matched or "not a function start";
- not had by any earlier Codex pass.

**Ungraded rows:** the new `--allow-ungraded` option admits parked rows with a draft but no DIFFER grade. That gave 82 of the 100 rows; their length is UNMATCHED_ROWS' estimate to the next function start. The other 18 have DIFFER grades, but their drafts are stubs, 1,135 to 1,554 B short.

**Larger rows held back:** 97 unlanded, unlocked rows of 752 B or more were had by Codex passes 11-24 and not matched. They are listed in `CODEX_SEEN_LARGER.tsv` (the largest is 2,880 B) and are left out under pass 28's rule. The owner can swap them in.

**Name evidence on the selected rows:**
- 69 have Ego_d records; 84 have Ego_r and 86 have FableWin procedure records.
- 17 carry a NAME CONFLICT flag.
- Selves: 49 are PROVEN, 1 is PROVEN_LANDED_WRONG, 10 are UNSETTLED, and 40 are named by no landed code yet.
- The 1,938 retail call / jump / data targets the rows reference break down as:

| Verdict | Targets |
|---|---:|
| PROVEN | 776 |
| PROVEN_LANDED_WRONG | 465 |
| REGISTERED_GUESS | 43 |
| FOLDED | 15 |
| UNSETTLED | 247 |
| NOT_NAMED_YET (mostly literals and tables) | 392 |

## What is new for names
- **`names/` is split into four folders** (`NAMES_README.md`): `answers/` (spell these), `pdb/` (evidence), `state/` (what the tree binds, and `LOCKS.tsv`) and `hints/` (labels, not evidence). Pass 29 had them in one folder, with no registry of best guesses.
- **`names/answers/NAME_ANSWERS.tsv`** comes from `tools/name_answers.py`, run over the tracker record at bebf762648 + NF479 (0dd7df1b2e); its sha256 is `cc883624dd...`. It gives 64,151 addresses a verdict and the spelling to use:

  | Verdict | Addresses |
  |---|---:|
  | PROVEN | 35,842 |
  | PROVEN_RTTI | 2,458 |
  | PROVEN_LANDED_WRONG | 4,401 |
  | REGISTERED_GUESS | 2,017 |
  | FOLDED | 44 |
  | UNSETTLED | 19,389 |

  Its gate fails the build on a placeholder-looking spelling marked usable.
- **106 vftable addresses are demoted** to UNSETTLED. 97 name a class that retail RTTI contradicts, for example `CGameEnvironmentInteraction` where RTTI says `CParticleEnvironmentInteraction`. 9 carry a stand-in base qualifier such as `CComponentBase3_Retail`. The tracker counts these `??_7` spellings as agree_std (a compiler name), but the class inside is not proven. That overstates the tracker's proven figure by about 106: 35,948 against 35,842 here. This should be fixed in `naming_status.py`, not by renaming anything.
- **`rows/<addr>/NAMES_TO_USE.tsv`:** every row's own address, plus each target its retail bytes reference (capstone over the retail extent), joined to the answer table.
- **`NAMING_FIRST_TIME.md`** (shipped as `docs/`) is the rule set, written from what pass 29 cost:
  - the 159 NAME_FIXES lines and NF479's 100 blocked lines;
  - the 7 back-outs;
  - the 5 rows without a Retail-asm line;
  - the 11 overridden guesses.

  The no-queue rule: Codex's rows leave no new disagreement behind. Any landed spelling a Codex name contradicts comes back fixed in the same pass, as a RESPELL unit (a spelling change or a port, gated like a row). Splits that already exist (PROVEN_LANDED_WRONG) are optional fixes. Only rows in `LOCKS.tsv` / `T3_PORT_QUEUE.tsv`, failed ports and true rulings go to `NAME_CONFLICTS.tsv`.
- **Standing rulings carried into the rules:** wide characters spell `G`, with no `/Zc:wchar_t` (owner 2026-10-29). The anonymous-namespace hash is per TU. Two original names at one address go to NAME_CONFLICTS with both sources.

## Build (the zip lives in scratch, never in git; no PDB, exe, retail bytes or SDK files)
1. `tools/prep_evidence.py`, with EGOD_PDB, EGOD_EXTRACT, REL_EXTRACT, EGOR_SYMBOLS and FABLEWIN_SYMBOLS set (PDBs and dumps in `/tmp/claude-0/egod_pdb/`).
2. `tools/name_answers.py <rec.json> NAME_ANSWERS.tsv --locks <nflocks> --rtti rebuild/manifest/rtti-vtables.tsv`.
3. `build.py`, with EGOD_EXTRACT, REL_EXTRACT, PDBX, FABLE_EXE, NAME_ANSWERS and EGOD_PDB_LINK set.

The result is `fabledecomp_codex_pass30_2026-11-27.zip`: 53,090,857 B, 5,878 files with SHA256SUMS. Its sha256 is `7a07782a9b8efaa9738905242c634594f2259aac0c35c62b5d7a6538235c16c1`.
