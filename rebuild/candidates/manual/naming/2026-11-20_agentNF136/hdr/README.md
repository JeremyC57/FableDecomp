# NF136 header overlay

Every overlay header is a copy of the repo header at session start (base copies in ../orig/) plus `#ifdef` opt-ins only. With no
NF136 macro defined each header is token-identical to its repo copy (the added lines sit inside `#ifdef` blocks that nothing else
defines). NF134 / NF135 have no overlay headers (their hdr/ directories are empty), so nothing here is declared twice.

## engine/CPixelFormat.h
- Base: rebuild/include/engine/CPixelFormat.h (orig/engine/CPixelFormat.h).
- Added after `void Initialise(_D3DFORMAT);` in the retail (FABLE_RETAIL_CPIXELFORMAT) class, public section:
  `FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX`: `static long __fastcall FindFormatInfoIndexFromD3DFormat(_D3DFORMAT format);`
  (0x009E3620, Ego_r.pdb 0x00B35470 / FableWin.pdb 0x0308CAB0 ?FindFormatInfoIndexFromD3DFormat@CPixelFormat@@SIJW4_D3DFORMAT@@@Z).
  Declaration only, no layout change. Used by rows/009be420.cpp and its test.
- pin_asserts.py: pinned 0 (the only asserts are FABLE_STATIC_ASSERT_AT lines of the generated default branch, line-number explicit).
- Landed includers: work/includers_CPixelFormat.txt (direct includers; others reach it through CDisplayManager.h / CTexture.h ...) are unchanged:
  no landed row defines FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX (grep of rebuild/src/compiled: 0 hits).

## engine/CPersistTraits.h
- Base: rebuild/include/engine/CPersistTraits.h (orig/engine/CPersistTraits.h).
- Added a partial mode `FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY` (the FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY pattern): under it the header
  forward-declares CPersistContext / CStringParser / CCharString instead of including engine/CPersistContext.h + engine/C3DVector.h, keeps
  class CPersistTraitsSLONG (unchanged text: the static __fastcall TransferIn 0x009E2590 and its inline long-returning overload), and
  leaves out everything after it (the generic CPersistTraits<T> family, which needs the retail string / stream models). Four
  preprocessor-only edits: `#ifdef ... #else` before the two includes, `#endif` after them, `#ifndef ...` before
  `template<class T> class CPersistTraits`, `#endif` before the include guard's `#endif`.
- Token check: `cpp -P` of orig/ and hdr/ copies with no NF136 macro gives identical output (md5 bf84fef65dca0657210a135c0581f794).
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Used by 21 rows of q5199 / q5220 that keep pre-existing local CCharString / CStringParser / CPersistContext / CPersistTraits models
  (rows/005dc3c0 005dc450 005dc4d0 005dc550 005dc650 005dc730 005dc9b0 00580194 00686240 00755f40 007b47b0 0040fb70 00463334 00566330
  005e8cc0 006144c0 0057f6e9 00475026 0047550f 00a38670 006ba480). The full-header rows (0x005D8167, 0x00679910 and the
  CPersistContext_TransferableComponent instances) use the default path.
- Landed includers: 226 rows under rebuild/src/compiled include engine/CPersistTraits.h (work/includers_CPersistTraits.txt); none defines FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY
  (0 hits), so the opt-in changes none of them.
