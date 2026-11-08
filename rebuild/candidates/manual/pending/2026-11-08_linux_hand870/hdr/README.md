# Chunk 870 header overlay

One additive opt-in. The default path is unchanged: `hdrcheck.py` reports `default=same`,
`FABLE_RETAIL_CSTRING_ALLOCCOPY=same` and `FABLE_RETAIL_CSTRING_BUILDERS=same` against `rebuild/include/engine/CString.h`.

## engine/CString.h: `FABLE_RETAIL_CSTRING_LEFT`

- **Row:** 0x00C0E1F0 `CString::Left(int nCount) const` (`?Left@CString@@QBE?AV1@H@Z`).
- **What it adds:** the MFC strcore.cpp declaration of `Left`, and afx.inl's inline default constructor
  `CString() { Init(); }`, which the body's empty `dest` uses.
- **Depends on:** `FABLE_RETAIL_CSTRING_ALLOCCOPY` (Init, AllocCopy) and `FABLE_RETAIL_CSTRING_BUILDERS` (copy constructor,
  destructor). The row defines all three macros.
- **Placement:** directly after the `FABLE_RETAIL_CSTRING_ALLOCCOPY` block.
- **Diff against the repo copy:** the 7-line `#ifdef FABLE_RETAIL_CSTRING_LEFT ... #endif` block only.

## Other header use (no overlay change)

0x00C08D40 and 0x00C1A510 now include the shared `libpk/PKCProtectionInstance.h` in place of their local lock copies.
0x00C1A870 already did.
