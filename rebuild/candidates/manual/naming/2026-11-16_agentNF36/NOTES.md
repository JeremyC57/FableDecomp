# NF36 (round 2) name-fix notes, 2026-11-16

Slice: QUEUE.tsv, 466 entries (land1115ga .. land1116p23), continuing NF34. Nothing landed, no git, nothing written
under /home/user/FableDecomp. Every changed landed row is claimed in $S/w22/nflocks/<addr> (owner NF36).

## Method
1. `work/classify.py` split the queue (screen, PDB name-set hit, round-1 status).
2. Probe: `callers.py` (E8/E9) plus `work/imm.py` (imm32/disp32) listed every landed row that references each target.
   `work/probe.py` compiled each such row once in the private `pS/probe` (one cl at a time, timeout) and recorded the
   symbol bound at the retail target (`nftools/xref.py`). Self rows were compiled too (`selfprobe*.tsv`).
3. `work/ev.py` gives each entry ok / bad / unverified binders and SELF_OK / SELF_DIFF, and writes `WRONG_BINDERS.tsv`
   (every landed row still binding a wrong spelling: the merge worklist).
4. Edits went into `rows/<addr>.cpp`, graded with `work/g.py` (q.py with `S=pS`, `FABLE_HDR_OVERLAY=hdr`, row flags).
   g.py prints the binding at every queue target, so each edit shows OK/BAD against the claim.
5. One final `vland.py --recheck-landed` over all 74 changed rows: `recheck_final.log`, with 74/74 MATCH or
   RELOCATION_MATCH and every test PASS (7 MATCH, 67 RELOCATION_MATCH).

## Results (APPLIED.tsv)
| status | entries |
|---|---|
| carry | 289 (56 data globals, 86 libpk/CODA descriptive-name entries, 147 other functions; all probed except entry 2, each note carries the worklist) |
| blocked | 72 (port 60, authoring 7 incl. the tails 22/24, ICF 2, library 3) |
| done | 47 |
| rejected | 38 (one coordinator decision: GameCODA-SDK-only evidence) |
| partial | 10 |
| done-already | 10 |

- **Rows changed:** 74 (STATUS.tsv). All graded MATCH (7) or RELOCATION_MATCH (67) by q.py. Max 4 grades on any row
  (0x0049D610, 0x00B990B0; two of the 0x0049D610 runs were tool-environment failures before cl started).
- **RENAMES.tsv:** section 1 has 33 self renames, section 2 has 61 callee respellings.
- **This session's ports onto shared headers** (the stand-in views were dropped):
  - 0x0049D610 and 0x0049D770 moved onto engine/CWorld.h (`FABLE_RETAIL_CWORLD_LOAD_ORIGINAL`), plus fable_string.h and
    CAFile.h. The callee 0x0041A060 is respelled `GetLevelsDirectory_0041a060` -> `NResourceDirectoryNames::GetLevelDir`.
  - 0x00434870 is `CFadeInFadeOutBase::Update` (`_DECLS`).
  - 0x00434C90 is `CDisplayEngine::EndScreenFadeOut` (`_ENDSCREENFADEOUT`, with ScreenFadeOutInfo typed).
  - 0x00B990B0 is `CEnginePrimitiveManagerRepeatedStaticMeshes::UpdateBoneMatrixForWindSheer` (private, PDB signature;
    preamble copied from row 0x00B9AC30).
  - 0x00892610 respells the local `CScriptModuleMoveBetweenLookFromAndLookTo` (no such class in either PDB) to
    `NCameraModes::CScriptModuleFullTransition` (Ego_r 0x00827260 / FableWin 0x0283C7F0 ctor). That class has no shared
    header.
- **Tests:** the tests of this session's rows are standalone behaviour models: they copy the body and never link against
  the row, so they are unaffected. Tests changed earlier: `rows/00401f30_test.cpp` (FableFullBuildVersion ->
  GVersionString) and `rows/00a205c0_test.cpp` (NoOfStates* -> NumberOf*). Both fixed the round-1 recheck LINK_FAIL. `rows/008fd7d9_test.cpp` is new: the first final recheck gave LINK_FAIL because
  the test still called the old free `GetActionName`. The new test calls `CAIStateGroup_ButterflyWander::GetName()` and checks
  the retail literal address 0x0128954C and length -1. (The landed row passed the wrong literal "DoCreatureAction"; it still
  graded RELOCATION_MATCH because the literal is a relocation.) The first run is kept in `work/recheck_final_run1.log`.

## Labels
- **rejected (38):** these claims cite the "official GameCODA SDK / release-debug symbols" (pass 22/23) as their only name
  source, and the evidence does not mark the name descriptive. EXTRA_RULES (owner research 2026-11-08) says no legitimate
  GameCODA SDK exists. On this lane I checked only the Ego_r / FableWin PDB name sets, and they have no hit. This is
  **one coordinator decision**, not 38 findings. Each REJECTED.tsv reason carries the probe data (ok/bad counts, wrong rows).
- **Done before the conflict was noticed:** three self renames rest on the same pass-23 SDK evidence: 0x00C609A0 (371),
  0x00C5DD70 (388) and 0x00C49D50 (416); done-already 419 / 89 cite it too. Revert them if the coordinator refuses that
  evidence.
- **Not rejected:**
  - Descriptive libpk/CODA names, allowed by the 2026-11-10 made-up-name rule. 62 entries first matched "SDK" but say
    descriptive; they are now carry or blocked.
  - Real-header STL symbols, e.g. `??0logic_error@std@@QAE@ABV01@@Z`.
- **Not settled (carry):** for 420, 421, 422 and 439 the self row and every probed binder already spell the claim, but
  some rows could not be probed (static helpers or a stale Leaf). Check those by hand before marking done-already.
- **blocked port (60):** the claim's class has a shared engine/<Class>.h, but every wrong row reaches the target through a
  local stand-in (address-suffixed classes, `_Methods` / `_Retail` views, `T` / `S` / `C<addr>` shells, free
  `Class_Method` functions). Respelling would create a new local model of a header class, which round-1 lessons forbid.
  The rows must be moved onto the header. The APPLIED note lists every row.
- **partial (10):** 4, 38, 45, 81, 148, 329, 330, 334, 335, 362. The note names the rows changed and the rows still
  wrong. Entry 4 has rows 0x00596CCE / 0x00599D5C **deferred: held by NF35**.
- **carry (289):** probed but not edited for lack of budget (entry 2 never reached). The worklist is in the APPLIED note and in
  WRONG_BINDERS.tsv:
  - 60 are data globals, where the binders bind other names or types.
  - The rest are mostly STLport template spellings, typed-signature differences and rows with stand-ins.
  - Where the claim class has no shared header, the note says "local respell possible" (cheap, like 0x00892610).
  - Entry 111 waits on a coordinator decision.

## Side effects and gaps for the coordinator
- **0x00B99710 ClipSubSections:** the self row's leaf is `QAE..@3`, but the PDB has `AAE..@1` (nested
  CClipListElement). The binder 0x00B9AC30 now binds the PDB form; the self row was not changed.
- **CacheMaps binder 0x005063E0** still binds the public, non-nested form.
- **Entry 148:** 0x0049DDD0 inlines the 3-argument overload 0x0049D550, so retail is 232 bytes. On the real class, the
  row would have to define that overload again, which duplicates the symbol landed by 0x0049D550 (graded DIFFER 32v232
  without it). Fixing it needs a same-TU port of both rows. The landed 0x0049DDD0 keeps the local
  `g_bRuntimeLevelFiles` extern for 0x013B8616, which fable_gfmain.h names `g_FableNavigatorEnabled_013B8616`. That
  makes two names for one address; it is not in my slice.
- **Entry 4:** the CWideStringVector stand-ins remain at 0x0042F50B, 0x0047E07F, 0x00597006, 0x00599E3F and 0x00E64F80.
- **Entry 362:** WriteULONG is still wrong at 0x00A99840, 0x00A9BA70 and 0x00AA4AF0 (`X_<addr>` stand-in classes).
- **Entry 45:** GetBossPhase is still wrong at 0x009547D0, 0x00954E30 and 0x0096C4F0; 0x00954410 is unverified.
- **Entry 81:** AddEnableConsoleVariable is still wrong at 0x00BA1A70, 0x00BABB50, 0x00BAD040, 0x00BB6E00, 0x00BBA480
  and 0x00BC7E90.
- **Entries 329 / 330:** the binders are fixed, but the self rows define CountMoneyBags on a local CThing. They need a
  port onto CThingBuilding.
- **Entry 61 (blocked authoring):** self row 0x008FE93B emits `_Forward`.
- **Entries 199 / 201 (blocked authoring):** the anonymous-namespace hash is compiler-generated: the claim has
  `?A0xfe46a155`, the rows emit `?A0x82efa2bb`.
- **0x00416953** (changed by me for entry 148) still binds `?GetLevelsDirectory@@` at 0x0041A060. 0x0049D610 / 0x0049D770 now
  bind `NResourceDirectoryNames::GetLevelDir` there. Respell it when that row is next touched.
- **Locks released:** I dropped my locks on the claimed-but-unchanged rows 0059db82, 005e4430, 0064db2b, 006a7610,
  008f8440 and 00984a20.
- **RENAMES section 1 `old_catalog_name`** is checked against `auto-re-candidates.tsv` column 2: 3 corrected (0x005FABA0, 0x0049D610, 0x0049D770).
- **Unverifiable rows:** libpk rows that define `static` helpers, and rows with a bare or stale `// Leaf:`, make xref
  choose the wrong function, so their bindings stay unverified (entry 298: 0x00BE6F70 / 0x00BF4570).
- **Entry 72:** 0x00AB9F60 / 0x00ABA360 are ICF-adjacent twins (noted in the header). The 0x00A6CDxx callers are in no
  landed row.
- **Entry 18:** the claim line is malformed (missing `@std@@`). Binder 0x0049A0B0 already binds the correct vector
  push_back. Self row 0x0049B6B0 is a descriptive stand-in (carry).
- **Entry 308:** blocked port. 0x01228920 (atexit dtor) still binds the stand-in `g_AutoTinyThunkObject_013b85e0`.

## Rule slips
- A few Bash calls exited non-zero:
  - claim.sh asserted on the NF35-held rows 0x00596CCE / 0x00599D5C (the rest were claimed on rerun);
  - `ls` of a not-yet-written selfprobe.tsv;
  - `head` of a mistyped recheck log path.
- One background grep loop read stdin (empty `$f`) and was stopped with TaskStop. An earlier repo-wide grep was moved
  to the background after 120 s.
- Two q.py runs for 0x0049D610 / 0x0049D770 failed before cl started because the toolchain env (`$S/wine/env.sh`) was
  not sourced. They are counted as grades.
- The probe once hung on a winedevice pipe; I killed only my own PIDs and restarted it with `< /dev/null`.
- No `__asm` / naked / `_emit` / `register` / `__forceinline` and no new `volatile`. No row exceeded 10 grades. One
  compile at a time throughout.
