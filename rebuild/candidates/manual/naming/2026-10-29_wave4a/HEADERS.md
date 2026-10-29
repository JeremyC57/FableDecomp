# w4a header items (not edited: each changes a header with ~1,270 includers; recheck every includer when applied)

## rebuild/include/fable_string.h (class CWideString) -- 1,269 includers under rebuild/src/compiled + rebuild/tests
Wide text stays `wchar_t` here only because the header is compiled without /Zc:wchar_t (stddef.h typedef =
unsigned short, so it mangles G); no type change is proposed.

| add / change | retail | PDB (Ego_r / FableWin) | G-form symbol | blocked rows |
|---|---|---|---|---|
| add `const wchar_t* GetAsCharArray() const;` | 0x0099AF10 | `?GetAsCharArray@CWideString@@QBEPB_WXZ` | `?GetAsCharArray@CWideString@@QBEPBGXZ` | 0041a430, 0055cfb0 (call `operator const wchar_t*` where retail calls 0x0099AF10) |
| add `void __cdecl Sprintf(const wchar_t* format, ...);` | 0x0099BA70 | `?Sprintf@CWideString@@QAAXPB_WZZ` | `?Sprintf@CWideString@@QAAXPBGZZ` | 0055e6b0 (free `FormatWideString` stand-in) |
| add `long FindCharPos(wchar_t c) const;` | 0x0099B070 | `?FindCharPos@CWideString@@QBEJ_W@Z` | `?FindCharPos@CWideString@@QBEJG@Z` | 0055e6b0 (`CWideStringOperations : CWideString` adapter) |
| `AllocStringData` private -> protected | 0x0099B3C0 | `?AllocStringData@CWideString@@IAEPAVCWideStringData@@PB_WJ@Z` | `...@@IAEPAVCWideStringData@@PBGJ@Z` | every includer that references it (none emits it today; changes the AAE spelling only) |

Adding members is symbol-neutral for the other includers (a declaration only emits where it is called).
After the header change, 0041a430 / 0055cfb0 need the call sites `x` -> `x.GetAsCharArray()` where the
retail call targets 0x0099AF10 (the w4a GETAS family recipe), and 0055e6b0 the SPRINTF recipe.
