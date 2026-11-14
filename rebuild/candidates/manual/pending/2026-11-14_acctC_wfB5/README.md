# acctC workflow hand-back wfB5 (parked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x007BC38A | NThingShot::CTCShotDefinitionThrowPredictive::InitMovement | 880 | RELOCATION_MATCH | PASS |
| 0x007EA580 | CTCTargeted::GetAllTargetPoints | 457 | RELOCATION_MATCH | PASS |
| 0x00735FD0 | CTCCombat::AttemptToStartParticle | 973 | RELOCATION_MATCH | PASS |
| 0x006823D0 | CTCCreatureOpinionOfHero::FrameUpdate | 962 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). No flags, no asm, no devices; shared headers throughout.
REVIEW ITEMS (FITTED marks are in NOTES.md, not in the source header comments):
- 0x007BC38A: FITTED named intermediate `double level_range = (distance / sin(a)) * cos(a);`. Pragmas optimize("y",off) /
  ("s",on) / intrinsic(sqrt, sin, cos) as landed sibling 0x007BC6FA (sin, cos added: retail has inline fsin / fcos).
  `THROW_ANGLE` is a descriptive (made-up) name. Label: 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (NAME_FIXES.tsv).
- 0x007EA580: the row TU defines three same-TU callees above the row (0x007EA2D0, 0x007EA2F0, 0x007EA490; retail order). The
  body of 0x007EA2F0 is the landed text; the other two are shared-header respellings of landed rows that use local models.
  Why it matters: c2 reads visible callee bodies to decide whether a local's address escapes (LESSONS.md).
- 0x00735FD0: FITTED `weapon_def->FlourishParticles.<member>` at each use (no `set` reference local). New additive opt-ins
  FABLE_RETAIL_CTCCOMBAT_ATTEMPTTOSTARTPARTICLE, FABLE_RETAIL_CWEAPONDEF_FLOURISHPARTICLES.
- 0x006823D0: FITTED named locals `over` and `fps`. New opt-in FABLE_RETAIL_COPINIONSET_INDEX (operator[]; one PDB source,
  FableWin only). The test models the unique id as 32-bit (header: unsigned __int64). HDR_README.md line 1 is stale.
`open/`: best drafts of 0x007E15C0 (963v963), 0x009FDE80 (1269v1265), 0x007BFE30 (1643v1648); residues in NOTES.md.
