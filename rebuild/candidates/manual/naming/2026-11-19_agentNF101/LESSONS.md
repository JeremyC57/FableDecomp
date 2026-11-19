# NF101 lessons (round 9)

- **`_CLASS` opt-in on a generated header:** put the `#endif` after the generated FABLE_STATIC_ASSERTs, or the asserts see the opt-in class and break; pin_asserts then pins them.
- **Leaf respell on port:** a CCreatureAction PerformAction / Initialise row ported onto CCreatureActionBase becomes UAE (virtual); record the QAE -> UAE leaf in RENAMES s1.
- **GetDef body visibility:** omit GETDEF_GLOBAL_BODY when retail calls GetDef out of line; a visible body lets VC7.1 cache the pointer and drop the release's null test.
- **CTCMapwho.h needs a forward declaration** (`class CTCMapwho;`) ahead of the include in CThingSearchTools instantiation rows, else SRC_FAIL.
- **CTCCombat prerequisites:** CTCCOMBAT needs CCOMBATABILITY + CCOMBATABILITYDATA defined first.
- **New template body opt-in:** append it at the end of the header under its own guard so no line (and no assert) moves; pin_asserts pins 0.
- **Shared EGOD body vs. small retail instance:** if retail's instance is far smaller (0x0078954D 354 vs 640), its helpers were out of line; do not spend grades, mark blocked: port with the callee list.
- **Count rows left with cs.py plus a declaration grep** before promising an entry done; a stand-in declared in 57 rows is a family job, not a round.
