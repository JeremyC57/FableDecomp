# Naming pass wave 15 (2026-11-01): 566 landed rows respelled to the proven c651-c666 names and the resolved wave 14 decisions

Inputs: brief `naming/briefs/BRIEF_W15.md`, `2026-11-01_wave14/DECISIONS_W14_RESOLVED.tsv` + DEFERRED.tsv, `naming/ESTABLISHED_SPELLINGS.tsv`,
QUEUE_W15_input.tsv (the 277 NAME_FIXES_QUEUE rows c651-c666). One Opus 5.5 agent authored the overlay in `$S/naming/w15` (scripts/ here:
compose15.py recomposes every stage on fresh copies of the repo, then foldtests.py; fam_*.py per family; hand15/ per-row stages); the coordinator
copied ov_final (575 files: 566 sources + 9 tests), applied BINDINGS (217 RETIRE / 45 ADD / 23 KEEP) and LABELS (48), and rechecked every changed
row against HEAD: **566/566 (13 MATCH, 553 RELOCATION_MATCH) + PASS** (`logs/coordinator_recheck.log`). `bindings_multi_check.py` 0; the repo
bindings table equals the agent's `runs/reloc_bindings.after.tsv`. Name-only: no byte changed. No file renamed (RENAMES.tsv empty; files whose
names carry the old function name are a follow-up: CTCPhysicsRigidBody_vfunc75_00759a00, CTCTurncoatSpell_FinalCleanup_007c75e0, CXMVPlaybackInfo_CXMVPlaybackInfo_007f7700).

Families, committed in this order (row lists `runs/<fam>.rows.txt`): bcn 37, bip 53, cab 17, cmb 4, scb 6 (base-destructor stand-ins respelled to
the proven ??1 symbols), sai 54 (SuspendAI), anong 48 (anonymous-namespace CCharString globals), dec 6 (the three wave 14 decisions), newline 1,
w14def 2, queue 336 (30 caller-respell targets), leaf 20 (own-row respells). Outcomes for the 277 queue rows (QUEUE_W15.tsv): 63 respelled,
40 respelled-partial, 150 deferred, 23 already-correct, 1 hold (0x00440730).

Decisions: `DECISIONS_W15_RESOLVED.tsv` (coordinator under the owner delegation 2026-11-01): friend access and `mutable NoEventScansRunning`
accepted as retail-consistent inferences; audio slot 5 ReleaseBuffer kept descriptive under CVtable_012b9174; the libpk copy of
CHandleBasedMemoryPool::Free 0x00C28690 deferred until the libpk class is named (one mangled symbol cannot link twice).

Deferred (DEFERRED.tsv, 208 lines): byte-moving family rows with the diff (tcb 13, bip 5, cmb 4, cab 2, bcn 2); family remainders on free-function
or destructor-view stand-ins (bo 29, scb 29, bcn/bip/cmb, sai 3); re-authoring work (~22 ??_G stand-in rows, ~40 STLport own rows, 00861d00 needs a
polymorphic CModeStandBase header, 00634870 C2011); the mega families not reached (ConstantFPS 314, bitset::test 239, CBasicString::Compare 138,
GetPItem 903, ~CCharString 135, GetSubDef, GetInstance, Transfer<>, AddEvent) -> naming wave 16 with the c667-c674 queue rows.
