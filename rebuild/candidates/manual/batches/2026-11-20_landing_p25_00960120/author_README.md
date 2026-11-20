# 0x00960120 CAIStateGroup_DeedReactionRoot::Update (p25O, Account B)

**Grade:** RELOCATION_MATCH (1600/1600), `// Flags: none`. Same TU as the landed 0x009609B0 IsRunnable.
- **Strict vland:** passes with no warnings (`vland.log`; `LANDVERIFY_STRICT_HEADERS=1`, overlay `hdr/`, a fresh LANDVERIFY_WORK). vland reads the 5-entry jump table at 0x0096074C, and its targets 0x009603A9 / B0 / B7 / BB / C2 match.
- **Real-object test:** passes, 52 scenarios (`test/`). It links the row object, and every out-of-line callee is an `/alternatename` stand-in.
  - The TU's own COMDATs stay real: CVectorMap::LowerBound, plus the CIntelligentPointer copy constructor and destructor.
  - The trace records the arguments of each call: attitudes, deed types, dialogue tags, the LookAt frame count and speed bits, and the PlayAISpeech and PlayAnimation arguments.
  - After each run the test also checks DeliveredReaction and DeliveredSpeech, and that the CDefPointer refcounts are back to their start.
  - It covers every exit, the virtual-deed and player paths, both greeting deeds, each of the six flirt tests, all five wife-attitude fallbacks (4 variants each), the speech retry loop with and without DoNothing failing, and the interruption and talking waits.
- **Mutants:** 40 of 42 source/header mutants killed (coordinator count: 42 mutants after the baseline line) (`mutants/`; the first line, "baseline SURVIVED", is the unmutated pass). Two survivors, both equivalent:
  - `ret == STARTED` -> `ret != FAILED_FINAL`: after the loop `ret` can only be STARTED or FAILED_FINAL, because the enum has 3 values and RETRY loops.
  - The `dummy_attitude = NONE` initialiser -> LOVE: a dead store. The switch covers all five wife attitudes, and the attitudes stop at WIFE_HATE (17).

## What closed it
1. **The strict-headers port** onto engine/ headers, with the new engine/COpinionDeedReaction.h. Then Ego_d's locals and scopes, and the Ego_d-shaped `if (ptc_deedlog) {...} else { DeliveredReaction = true; Terminate(); return; }` (advisor ruling). That reached DIFFER(1600v1600) with `/d2inlS25`.
2. **The LookAt speed literal.** Retail pushes 0x3B360A13, which is `0.0027777f`, the spelling the landed 0x009357F0 / 0x0093A080 use. `0.0027777778f` is 0x3B360B61. g.py masks immediates, so only vland caught it.
3. **The IsEmpty expansion.** The flag's only effect on this row is that CCharString::IsEmpty gets expanded: the call lists of the no-flag and S25 objects differ by `?IsEmpty@CCharString@@QBE_NXZ` alone. A per-TU FABLE_FORCEINLINE opt-in does the same with no flag. It is in the same family as the existing FORCEINLINE_TEXT_EQ / _EQUALSCI / _LESS opt-ins. See the flag-versus-device note below.

## Flag v device (for main / the advisor)
The ladder ranks "plain inline + per-row flag" above a device, and `/d2inlS25` alone matches this row (`evidence/flag_form_d2inlS25.cpp`, window S21..S27, or T275 / T290 alone). It fails the single-compile check:
- Retail compiled 0x00960120 and 0x009609B0 in one TU, so both must match under one flag set.
- With the define and include blocks unioned (`evidence/tu_union_*.cpp`):
  - IsRunnable is RELOCATION_MATCH with no flag.
  - IsRunnable DIFFERs under every `/d2inlS` from 21 to 100 (S21: 1371v1352) and under T275 / T290 (1397v1352). S21 also expands the CIntelligentPointer<const CThing> pointer constructor and operator=, which retail calls.
- So the flag cannot be how retail built this TU. The batch therefore carries the device.
- With FORCEINLINE_ISEMPTY, the union grades RELOCATION_MATCH for both rows with no flag (IsRunnable never calls IsEmpty).

## Ablation table (sources in `evidence/`, each one change from `src/`, overlay `hdr/`)
| change | grade |
|---|---|
| final | RELOCATION_MATCH |
| a1: without FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ | DIFFER(1576v1600), 303 lines |
| a2: without FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY | DIFFER(1596v1600), 248 lines (IsEmpty called) |
| a2b: without ISEMPTY, `// Flags: /d2inlS25` | RELOCATION_MATCH (the flag form; fails the TU-mate check above) |
| a3: FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE instead of _COPY | DIFFER(1580v1600) |
| a4: without CINTELLIGENTPOINTER_COPY | DIFFER(1584v1600) |
| a5: LookAt speed `0.0027777778f` | DIFFER(1600v1600) (vland: the 0x009604A9 immediate) |
| a6: deed-log fail block first (`if (!ptc_deedlog) {fail}`), not the Ego_d else arm | DIFFER(1604v1600) |
| a7: attitude as one ternary instead of GetAttitudeType then `if (!ptc_source) blur` | DIFFER(1608v1600) |
| a8: without FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR | RELOCATION_MATCH, but vland warns RELOC_NAME (~CCreatureAction_PlayAnimation -> 0x00693EF0 labelled ~CCreatureActionBase) |
| a9: no `anim_name = NULL` initialiser (Ego_d line 245) | RELOCATION_MATCH (neutral) |
| l245: `dialogue_tag = 0` at line 245 instead | RELOCATION_MATCH (neutral) |

The grades come from the workspace grader (g.py, the same compile as vland). Every grade is for the full flag set shown, which is none unless the row says otherwise.

## Inferred
- `const CCharString* anim_name = NULL;` at Ego_d line 245. Ego_d has a 7 B dword zero store between the ptc_source initialiser (244) and its PeekTC (246).
  - `dialogue_tag = 0` there grades the same, and so does no initialiser.
  - anim_name was chosen because the text tests `anim_name == NULL`.
  - Ego_d's frame offsets (anim_name -40, dialogue_tag -28) do not decide it.

## Header changes (`hdr/`, `hdr.diff`: strictly add-only, 0 removed lines)
- **New** engine/COpinionDeedReaction.h (Ego_r TPI, 0x90): Animations CCharString[18] +0x00, DialogueTags ulong[18] +0x48. Plain FABLE_STATIC_ASSERT, since it is a new header with no older typedef name to keep.
- **engine/CTCHeroOpinionDeedLog.h**: FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDREACTIONS, nested in _COMPLETE.
  - Adds the typed `COpinionDeedReaction DeedReactions[92]` (+0xE8) and the inline GetDeedReactionAnimName / GetDeedReactionDialogueTag (Ego_r.pdb mangled names in the comment).
  - The existing byte-array line is untouched, now under the new `#else`, so the default and every existing macro are unchanged (hdrcheck2: same).
  - The access label returns to `private:` as before.
- **engine/COpinionReactionManager.h**: FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS adds:
  - inline GetAttitudeType, whose body is SAME as the landed out-of-line 0x00681500 by cmpcomdat (relocations only);
  - inline CanPlayDeedReactionSpeech;
  - declarations of GetBlurredAttitude, GetThingAttitudeToPlayer, LogDeedReactionPlayingSpeech and the private GetAttitudeOrReactionType, guarded against the opt-ins that already declare them.
- **fable_string.h**: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY puts FABLE_FORCEINLINE on the existing IsEmpty, which is inline in retail per FableWin 0x01D34B90 and the expansion at 0x00960376..0x00960381.
- **pin_asserts.py** run on all three changed headers: 0 to pin (their asserts are already FABLE_STATIC_ASSERT_AT). **hdrcheck2.py**: every existing macro is "same"; the only DIFF is the new DEEDREACTIONS macro.

## Newly flagged rows / name fixes
- `newly_flagged_rows.tsv` lists two rows:
  - 0x006C5D70 ~CTCHeroOpinionDeedLog has a local COpinionDeedReaction model.
  - 0x00681500 GetAttitudeType has a local COpinionReactionManager model.
- `NAME_FIXES.tsv`: 0x00681500's landed parameter types `(const CThing*, CTCOpinionSource*)` differ from the PDB `(const CThing&, const CTCOpinionSource*)` (Ego_r 0x005520D0, FableWin 0x01F15820). The name is unchanged.
- TU-mate 0x009609B0: its define block unions cleanly with this row's (`evidence/tu_union_isrunnable.cpp`: RELOCATION_MATCH, no flag).
