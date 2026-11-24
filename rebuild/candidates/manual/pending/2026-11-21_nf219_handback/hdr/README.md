# NF219 overlay

Base copies in ../orig/ (copied from the repo working tree at the start of the run).

## fable_string.h
- New add-only opt-in `FABLE_RETAIL_CCHARSTRING_NULLSTRING` (inside the `FABLE_RETAIL_CCHARSTRING` public block, right after the
  `CCharString(const char*, long = -1)` declaration): `static CCharString NullString;` (0x013BD804), declaration only.
  Evidence: Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CCharString::NullString'` (0x0101AAD0 / 0x0400A4F0) and
  `dynamic atexit destructor ..` (0x0101E240 / 0x04012360); Ego_d.pdb PUB32 `?NullString@CCharString@@2V1@A` (public static).
  Mirrors the existing `FABLE_RETAIL_CWIDESTRING_NULLSTRING`. Used by rows 0x01223EC0 and 0x0122A5E0 (and the 0x0122A5E0 test).
- Default path token-identical: the only change is the new `#ifdef` block. pin_asserts.py: pinned 0 (the asserts carry explicit
  `FABLE_STATIC_ASSERT_AT` numbers).
- Header users: 4403 landed rows under rebuild/src/compiled include fable_string.h; none defines FABLE_RETAIL_CCHARSTRING_NULLSTRING
  (grep of rebuild/src, rebuild/tests, rebuild/include), so the opt-in changes none of them.
- Other overlays read first: agentNF215/hdr/fable_string.h is identical to HEAD; NF216 / NF218 carry no fable_string.h.
