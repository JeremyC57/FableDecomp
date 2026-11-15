# acctC workflow hand-back wfC1 (big rows, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00CBFB7D | RunCutsceneMacro | 74,778 | RELOCATION_MATCH | PASS |

Verified by the workflow verifier and the coordinator (`coordverify.log`), then fixed up (shared engine/CCutsceneDef.h in
place of a local class) and verified again. No `// Flags:` line, no inline pragma. 20,435 instructions, 4,068 calls.

LANDING NOTE (include resolution): the row needs the overlay's fable_string.h opt-ins. With an overlay DIRECTORY the repo's
fable_definition_table.h includes the repo's fable_string.h (MSVC searches the including file's folder first) and the row
fails with C2011; an unchanged copy of fable_definition_table.h next to the overlay fable_string.h cures it (that is how
it was verified). Once the fable_string.h opt-ins are landed into rebuild/include the problem does not exist.

REVIEW ITEMS (all marked in the source, line 2 lists them; owner test: byte match yes, same behaviour yes, portable yes):
- 36 dead release asserts (`FABLE_RETAIL_ASSERT`, `// Inferred:` block): invented conditions; count (34..38 would do) fitted
  to the caller-size inline budget, places fitted to three c2 record-id facts (0x00CCB3C6, 0x00CCE8EA, 0x00CCED7D).
- FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT (fable_string.h opt-in): force-inline StartsWith (evidenced) carrying one
  dead `FABLE_RETAIL_ASSERT_ARM(PStringData != 0)` (new macro in fable_assert.h: the dead arm without the do/while
  wrapper). FITTED: FableWin's debug StartsWith has no assert; the arm exists only to add caller size per expansion. It
  emits no bytes and is empty under FABLE_PORTABLE. This is the item most likely to need an owner ruling.
- FITTED locals `fraction = 0.0f` (dead initial store), `offset`, hoisted `long frames = 0;`; row-local inline `CSPositionY`
  (descriptive name; carrier of an evidenced x87 operand order at 0x00CC597E).
- Header opt-ins FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE / FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE (claimed evidenced:
  retail expands these ctors at every site).
- Descriptive (made-up) names: g_CutsceneSoundFrame 0x013B83C8, g_CutsceneMusicOverridden 0x0143E8FD, all locals (callee
  parameter names of the Anniversary PDB used as a weak third source); last parameter `skippable`.
- `stayOnLastFrame` in .PlayCombatAnim: retail calls IsTrue(params[1]) at 0x00CC167B and drops the result.
- Behaviour fix found late: TeleportToHSP needs a CUTSCENE_FRAME() after the teleport (retail 0x00CCA417 jumps to a frame
  tail). Tools that mask jump targets hid it; q.py + vland are the grade.
- 25 RELOC_NAME label warnings (LABELS.tsv); 0x013D2838 has four bound spellings in reloc_bindings.tsv (NAME_FIXES.tsv).
- Other lanes' matched rows were not regraded against the header opt-ins this row adds (all additive).
