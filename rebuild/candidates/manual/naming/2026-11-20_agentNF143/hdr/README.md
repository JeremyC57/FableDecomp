# NF143 overlay headers

## engine/CUserProfileManager.h (base: rebuild/include/engine/CUserProfileManager.h, copy in orig/engine/, byte-equal to the repo at hand-back)
- Adds one opt-in block, right after the first FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES block:
  `FABLE_RETAIL_CUSERPROFILEMANAGER_GETMANUALSAVEBASEFILENAME_INLINE` declares
  `CWideString GetManualSaveBaseFileName() const { return L"Save"; }` (Ego_r.pdb 0x00405250 / FableWin.pdb 0x0187B570
  ?GetManualSaveBaseFileName@CUserProfileManager@@QBE?AVCWideString@@XZ, retail 0x004065F0). Needs FABLE_RETAIL_CWIDESTRING (implicit literal
  conversion). Users: rows 0x00406610 (expands it) and 0x004065F0 (emits it as the pick-any COMDAT); the 0x00406610 test.
- Without the macro the header is token-identical to the base (checked: deleting the block reproduces orig/ exactly).
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT lines that moved).
- Landed includers: 50 rows under rebuild/src/compiled include engine/CUserProfileManager.h; none defines the new macro, so the opt-in
  changes none of them. NF141 / NF142 have no overlay of this header.
