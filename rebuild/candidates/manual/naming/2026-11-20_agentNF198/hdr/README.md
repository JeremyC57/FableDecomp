# NF198 overlay headers (2026-11-20)

Base copies of every overlaid header are in ../orig/ (copied from the repo working tree when the overlay was made).
Every opt-in is add-only, inside a new member-specific #ifdef; with the macro undefined each header is token-identical to its orig/ copy.

## engine/CThing.h (copy of rebuild/include/engine/CThing.h)
- Added FABLE_RETAIL_CTHING_SETTHINGSCANINCLUSION (nested in FABLE_RETAIL_CTHING_STATEACCESS, right after SetAsGameStatePersisted):
  declaration `void SetThingScanInclusion(bool);` (0x004C9C10; Ego_r.pdb 0x004CB6B0 + FableWin.pdb 0x01CF8070 ?SetThingScanInclusion@CThing@@QAEX_N@Z).
  Used by rows/004c9c10.cpp. Declaration only, no layout change.
- pin_asserts.py: pinned 0 (no FABLE_STATIC_ASSERT_AT after the insertion point needed re-pinning).
- Landed includers of engine/CThing.h: 3177 rows (grep -rlE 'engine/CThing\.h' rebuild/src/compiled); none defines the new macro (0), so the
  opt-in changes none of them. NF196 / NF197 overlays do not carry CThing.h.
