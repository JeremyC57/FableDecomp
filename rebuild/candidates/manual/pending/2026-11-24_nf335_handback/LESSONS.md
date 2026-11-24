# NF335 lessons (2026-11-24)

- Of NF324's 79-81 "own test calls X" rows, the test was the blocker for only 25: the other 56 stop on the ROW side (31 Copy thunks with an
  extra stack parameter to a made-up `_impl` target, edx used, template types, `_PASS` switches, duplicate strong definitions, ...). Re-run
  the row-side triage with the test guard lifted before planning test work.
- Of those 25, the full precheck dropped 3 more: a switch model can lack the flat members the flattened body reads (`interrupted`,
  `__vftable`, `transform`), and needs base-model prerequisites the dry triage (`--no-pre`) does not see.
- A virtual PDB member cannot be added to the test's own data model (the vptr shifts every field and ends aggregate initialisation). The
  form that works: rename the test's data model to `<Class>Layout` and declare a separate declaration-only call model `<Class>` with only
  the PDB member; call `((Class*)(&obj))->Class::Method(...)`. Never constructed, so no `??_7` / `??_R` in the test object (checked on the
  objects).
- The rename must skip comments and strings, and must NOT touch a file-scope stub whose parameter names the class: the row object binds
  that stub with the real class in its mangled name (0x008421C0's `CCreatureAction_Base_FinishAction(CCreatureAction_GetOutOfBed*)`).
  Likewise a stub member defined on the class (`CThingSwitchDef::base_clone`) moves to the call model (0x004644FA).
- Tests that include the class's shared header (0x004E187E, 0x00890B50) cannot take a second model of the class: define the row's own
  switches in the test, bind references to zero-initialised static storage of `sizeof(Class)`, and call qualified.
- A protected PDB member is reached through a test-local friend class (`<Class>_TestCaller`), not by widening access.
- Literal `this` values (`(T*)0`, `(T*)0x12345678`, `*(CPlayer*)0xAAAA`) become zero-initialised static storage; the rows read none of it.
- Control for the test move: HEAD source + moved test must LINK_FAIL (the moved test names no flat symbol), recorded per row in GRADE.tsv.
- Restarting a grade: never `pkill -P`; record the PIDs of every layer (timeout, python, vland) and kill them by number.
