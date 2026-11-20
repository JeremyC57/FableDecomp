# 00ddae40 (p25A, Account B, 2026-11-20): NScript::CQ_PrisonRaceScript::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00DDAE40 | NScript::CQ_PrisonRaceScript::Main() | 1180 | RELOCATION_MATCH (no flags) | PASS (36 targeted + 4,000 random runs) | 27 / 27 killed |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings. Purity: plain C++, no asm.
Grades: 5 (the port matched at grade 2; the rest are the commented final and the assert pinning). No local class model.

## Source shape
Ego_d.pdb line table (egod_lines/egod_lines.tsv, lines 75..94) and locals (`_Fail_Val`, `RaceStart`): the generated quest-script text
of the landed Mains 0x00D27460 / 0x00E92A50 (`TRetVoid _Fail_Val;`, one line per binding, `PostAddScriptedEntities();Game->PostAddScriptedEntities();`
on one line, the wait loop and its trailing terminate test on one 220-byte line, the spawn).
- `// Bracket:` CScriptThing::GetAngleXY's forwarded call is parenthesised, `return (PImp.Data->GetAngleXY());` -- the landed text of
  0x00DD9520 (chunk Q13 lever, same retail site shape). The PAREN tuple puts the teleport angle's `fstp [esp+18h]` after the GetPos null
  re-test (0x00DDB144..0x00DDB14A); unbracketed it follows the call (1180v1180, one swapped pair -- the residue of the 2026-11-07/08
  drafts' ~120 grades, which predate the Q13 lever). The parked 0x00E03F70 has the same residue at 0x00E04064 / 0x00E044CF: try the same text.
- No `// Inferred:` lines.

## Header overlay (hdr/, hdr.diff; additive: the only `-` lines are the three existing size asserts respelled FABLE_STATIC_ASSERT_AT by
pin_asserts.py because the new block moves them)
- engine/CQ_PrisonRaceScript.h: NEW opt-in `FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN` -- `void WatchForHeroWinOrLose();` (0x00DDB2F0) and the
  nested entity scripts CPrisonRaceWarden / CPrisonRacer / CPrisonRaceStartingGuard / CWaypointSpeedCheck (Alloc, GetParentScript,
  private Main / Init, ParentClass / PMasterData (/ RaceWaypointIndex) per Ego_r.pdb; sizes 0x1c / 0x20 / 0x1c / 0x1c asserted;
  vtable slot addresses read from Fable.exe 0x012DCA10 / 0x012DCA2C / 0x012DCA48 / 0x012DCA64). The row also uses the existing
  FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD.
- Regrade: the one landed includer, 0x00DDC070 CPrisonRaceWinningPostGuard::Main, stays RELOCATION_MATCH with the overlay.
- No new header file. Landed rows of these classes still on local models of CQ_PrisonRaceScript (not newly flagged; re-port backlog):
  the Alloc / Init / GetParentScript / Main rows of CPrisonRaceWarden, CPrisonRacer, CPrisonRaceStartingGuard, CWaypointSpeedCheck.

## Test (test/00ddae40.cpp)
Links the real row object. Script object raw storage (Game +0x40, MasterScriptData +0x44), fake Game vtable (slots 7, 12, 64, 70, 72, 473),
the marker thing {vptr, PImp.Data, PImp.Info} over a count block (refcount 1 and 2: release / delete observable), a fake implementation
for the expanded GetPos / GetAngleXY (and an empty marker: NullCoord / 0.0f), scriptable operator new failures (each binding and the
spawn take the null path), scriptable region loading and termination. Checks the event order against an independent model, every binding
(new(0x1C) block, vtable, copied name, parent, factory = the aliased Alloc stub, valid byte, flags 0, slot 0 GetEntityScriptName), the
spawned function (new(0x3C), CSpawnedFuncBase(name "WatchForHeroWinOrLose", 0), PClass, group "", slot 4 calls WatchForHeroWinOrLose on
the script and sets StopRunning), the flag clear, the teleport arguments, string balance and the marker's destruction.

## Mutants (mutants/)
27 one-decision mutants, 27 killed (one rerun at occurrence 2: occurrence 1 of its text is in the header comment).
