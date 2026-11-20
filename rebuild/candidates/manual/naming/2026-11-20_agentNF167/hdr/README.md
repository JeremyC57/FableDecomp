# NF167 header overlay (2026-11-20)

Every overlay header is a copy of the repo working-tree header (base copy in ../orig/) plus `#ifdef` opt-ins only, with
member-specific macro names. Without the opt-in macros each header is token-identical to its repo copy (checked by diff:
the only differences are the added `#ifdef ... #endif` blocks and the pin_asserts line pin).

## engine/CTCDrunkenness.h
- Base: rebuild/include/engine/CTCDrunkenness.h (orig/engine/CTCDrunkenness.h).
- Opt-in `FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES` (q4151, row 0x0093A8C0), after `IsDrunk`: inline public `DrankFrames(long)`,
  `GetDrunkenness()`, `SetDrunkenness(float)` (Ego_r / FableWin spellings) and the private static
  `DrunkennessDeltaPerFrameOfDrinking` (0x013BAE70, `@@0MB`, Ego_d PUB32). Inline members and a static: no layout change.
- pin_asserts.py: pinned 1 (the `FABLE_STATIC_ASSERT(sizeof(CTCDrunkenness)==0x20)` line, moved down by the opt-in block).
- Landed includers: rebuild/src/compiled/00/68/CTCCreatureOpinionOfHero_GetGreetDialogueGroupIndex_00683c10.cpp only; it does not
  define the new macro, so the opt-in changes nothing for it.

## engine/CTCCarryable.h
- Base: rebuild/include/engine/CTCCarryable.h (orig/engine/CTCCarryable.h).
- Opt-in `FABLE_RETAIL_CTCCARRYABLE_GETCARRYSLOTPASSIVE` (q4475, row 0x00968BC0), after GetCarrySlotActive: declaration of
  `long GetCarrySlotPassive() const` (0x00705D00, Ego_r / FableWin). Declaration only: no layout change.
- pin_asserts.py: pinned 1 (`FABLE_STATIC_ASSERT(sizeof(CTCCarryable) == 0x24)`).
- Landed includers: 19 rows (`grep -rlE 'CTCCarryable\.h' rebuild/src/compiled`); none defines the new macro, so the opt-in changes none of them.

## engine/CTCEnemy.h
- Base: rebuild/include/engine/CTCEnemy.h (orig/engine/CTCEnemy.h).
- Opt-in `FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES` (q4949 / q4988; rows 0x0072B160, 0x0082B630, 0x009098E0, 0x0093C4E0, 0x006A7550 and its
  test), after the FABLE_RETAIL_CTCTARGETINGAI_ATTACK declaration, guarded `!defined(FABLE_RETAIL_CTCTARGETINGAI_ATTACK) &&
  !defined(FABLE_RETAIL_CTCENEMY_QUERIES)` so the static is never declared twice: the public static
  `AreThingsEnemies(const CThing&, const CThing&)` (0x0076D2F0) without FABLE_RETAIL_CTCENEMY_QUERIES's ETCInterfaceType.h include (the rows
  keep a row-local interface enum). Declaration only: no layout or vtable change.
- pin_asserts.py: pinned 0 (the size assert is already the pinned FABLE_STATIC_ASSERT_AT(59, ...), which keeps its recorded line).
- Landed includers: 29 rows; none defines the new macro, so the opt-in changes none of them.

## engine/CTCCreatureOpinionOfHero.h
- Base: rebuild/include/engine/CTCCreatureOpinionOfHero.h (orig/engine/CTCCreatureOpinionOfHero.h).
- Opt-in `FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_CLEARPENDINGDEEDS` (q5043, row 0x00960070), just before the
  FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS block and guarded `!defined(FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS)` (that block declares
  the same member): `void ClearPendingDeeds();` (0x00683B00) without the typed deed lists. Declaration only: no layout change.
- pin_asserts.py: pinned 0 (the header's asserts are already FABLE_STATIC_ASSERT_AT).
- Landed includers: 60 rows; none defines the new macro, so the opt-in changes none of them.
