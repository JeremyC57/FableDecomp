# Landing nf171b (Account A, 2026-11-20): CGamePlayerInterface::IsGameActionEventInQueue self rename (1 row)

Follow-up to nf171 (agent NF171's row 0x00446B10 and its test, unchanged from the hand-back).

- 0x00446B10's leaf moves from the stand-in `?IsGameActionEventInQueue@CGamePlayerInterface_Methods@@QBE_NW4EGameAction@@W4EInputButton
  EventType@@J@Z` to `?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z`. Its call
  to 0x00445CB0 moves off the same stand-in class.
- Self-rename rule (nf120 / nf139): no landed row binds the old leaf, and the new one is already bound by the rows nf171 respelled
  (0x00582A16, 0x00687D50, 0x006881D0, 0x007E0750) and others. 0x0068EB20's separate `QAE_NJJJ` binding is not the old leaf and
  is untouched here (pre-existing; queued in NF171's notes).
- Recheck: RELOCATION_MATCH, test PASS. Split gate 0 created; 15 referencing rows good; tablecheck: 0x00445CB0 2 -> 1. Link: no
  LNK2005.
