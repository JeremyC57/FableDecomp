# NF97 overlay headers (round 8, 2026-11-19)

Each overlay is a copy of the CURRENT repo header (base copy in `../orig/`, byte-equal to the repo at hand-back) with `#ifdef` opt-ins
only, under member-specific macro names. Neither header is in NF93's or NF96's overlay.

Default path token-identical: checked with `cpp -P` of the default path (main class macro defined, no opt-ins; and nothing defined)
against `orig/`, and of the one landed includer below: identical output (work/tok/).

## engine/CGameEventDataPackage.h (base: rebuild/include/engine/CGameEventDataPackage.h)
- `FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE` (NF95's held opt-in, carried unchanged): declaration of
  `?GetUHUGE@CGameEventDataPackage@@QAE_KXZ` (FableWin.pdb 0x023D5E10), out of line in retail at 0x00628E80. Users: row 0x00628E80
  and its test.
- `FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS` (new, NF97): inline `GetUBYTE` / `GetUWORD` / `GetFloat` / `GetSLONG`
  (FableWin.pdb names; out of line only in FableWin, none in Ego_r.pdb). Bodies `// Inferred:` (one GetData of the value's size).
  Users: 0x00629420, 0x00629B10, 0x00629C40, 0x00629DE0, 0x0062ABA0, 0x0062CE60 (each DIFFERs with a direct GetData read into its
  local, vland work/b1.log, and matches with the inline reader, work/b2.log).
- pin_asserts.py: pinned 0 (the one assert is already `FABLE_STATIC_ASSERT_AT(22, ...)`, its repo line).
- Landed includers: none (`grep -rlE 'CGameEventDataPackage\.h' rebuild/src/compiled` is empty), so the opt-ins change no landed row.

## engine/CIsHornTaskWithThisHorn.h (base: rebuild/include/engine/CIsHornTaskWithThisHorn.h)
- `FABLE_RETAIL_CISHORNTASKWITHTHISHORN_CTOR_OUTOFLINE` (NF95's held opt-in, carried unchanged): the ctor
  `??0CIsHornTaskWithThisHorn@@QAE@V?$CIntelligentPointer@VCThing@@@@@Z` declared only (out of line in retail at 0x0065F0FA); the
  inline ctor is the `#else`. Users: row 0x0065F0FA and its test.
- pin_asserts.py: pinned 0 (the assert is already `FABLE_STATIC_ASSERT_AT(18, ...)`, its repo line).
- Landed includers: 1, rebuild/src/compiled/00/65/CTCVillage_UpdateHornTasks_0065be6c.cpp. It does not define the opt-in; its `cpp -P`
  output is identical with and without the overlay, so the opt-in changes no landed row.
