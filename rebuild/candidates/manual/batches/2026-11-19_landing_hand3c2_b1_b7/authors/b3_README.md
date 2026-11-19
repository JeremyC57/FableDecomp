# b3 (hand3c2, Account B, 2026-11-19): 0x00D6C830 NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus -- READY

- Grade: RELOCATION_MATCH (g.py + strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1). Purity: genuine (plain C++).
  ATTEMPTS.log: 3 grades (DIFFER(587v607) first port, DIFFER(607v607) do/while form, RELOCATION_MATCH) + 1 PROBES summary line
  (no-ATTEMPTS pp.py sweep, probe/c830). Flags: `/d2inlT62` (Inline-budget line in the source).
- Leaf ?CheckBanditFollowStatus@CQ_HangingTreeEvilScript@NScript@@QAEXXZ, 607 bytes, plain `ret`.

## Identity
Ego_r.pdb and FableWin.pdb, same spelling. Locals exactly as Ego_d.pdb (debug length 880): _Fail_Val (TRetVoid), CorrectExit (CScriptThing).

## What closed it
1. Port onto the shared quest header in the generated-script form of the landed TU-sibling WatchForBooty 0x00D8A240
   (`TRetVoid _Fail_Val;`, every terminate return `return _Fail_Val.Return();`, `for (;; Game->NewScriptFrame())`): 587v607.
   The residue: retail calls ~CScriptThing 0x004AA840 at the outer loop's after-wait return and expands it at the others.
2. Plain loop forms first (`while (!term)` 587v607, `if (!term) do .. while (!term)` 607v607 return blocks reordered, frame at body
   end 587v607), then the inline budget (owner rule 2026-11-08 preference 2, WatchForBooty precedent): /d2inlT62.
   Measured: T40/T50 620v607, T57/T60/T61 607v607, T62..T71 RELOCATION_MATCH, T72..T74 and default 587v607; smallest N = 62.

## Overlay header (hdr/engine; copy of today's repo header + NEW opt-in; default path unchanged; asserts pinned with pin_asserts.py)
| Header | New opt-in | What |
|---|---|---|
| CQ_HangingTreeEvilScript.h | FABLE_RETAIL_CQ_HANGINGTREEEVIL_CHECKBANDITFOLLOWSTATUS | declaration `void CheckBanditFollowStatus(); // 0x00D6C830` |
No overlay of this header in ../hand3c/hdr. Landed users of the header (0x00D6D670 Conclusion, 0x00D6A560 CHTE_FreedomFighter::Main)
do not define the new opt-in; the diff is the declaration block + pinned assert lines only.

## Inferred / FITTED
None. No bracket.

## Test (test/00d6c830.cpp)
Links the REAL row object built with /d2inlT62. Native-layout fixtures (Game +0x40, SwitchingRegions +0x51, ShouldFollowHero +0x58,
Game slots 7 / 70 / 72), exit points as refcounted raw storage, CScriptThing::operator= share defined in the test, unused virtuals
aliased to one trap. A per-frame world script drives the flags; an independent reference model is compared event by event
(frames, terminate tests, distance queries with arguments, exit fetches / assignments, releases, CCharString balance) over targeted
cases + 3000 random runs. Prints `00d6c830_TEST PASS`.

## Mutants (mutants.tsv / mutants.log): 22 / 22 KILLED
All 9 terminate-return sites (each `if (0)`), the exit-point name, the loop step frame, both flag waits, the flag stores, the
distance test (negated, 20 -> 25), the refetch, and the frame calls removed. (An earlier list line asked for a 10th terminate
site that does not exist -- NOT_FOUND in the log -- and was dropped from mutants.tsv; only 9 exist in the source.)

## Names
No NAME_FIXES (header-only file). Callee names checked: IsDistanceBetweenThingsUnder 0x00CBE2FF, CScriptBase::IsActiveThreadTerminating
0x00CB7940, CScriptThing::operator= 0x008AB980, ~CScriptThing 0x004AA840 (vland no RELOC_NAME warning).

## Files
src/00d6c830.cpp, test/00d6c830.cpp, hdr/engine/CQ_HangingTreeEvilScript.h, CQ_HangingTreeEvilScript.h.diff, payload.json,
oracle.tsv, mutants.tsv, mutants.log, vland.log, NAME_FIXES.tsv.
