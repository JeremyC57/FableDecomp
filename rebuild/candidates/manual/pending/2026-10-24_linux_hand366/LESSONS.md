# Chunk 366 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Three address-taken C2DVector locals passed to one call want a `C2DVector p[3]` array filled member by member.
  Separate locals take the reverse frame order, and a `C2DVector(float, float)` constructor copies one Y through
  `fld/fstp` (00404e10).
- A colour cut from a dword as `b = (uchar)c; c >>= 8; g = (uchar)c; c >>= 8; r = (uchar)c;` gives retail's
  `mov cl,al ; shr eax,8 ; mov edx,eax ; shr edx,8` chain; `(uchar)(c >> 16)` gives a separate `shr 0x10` (00404e10;
  the same chain is in the IME helpers 0x00404CF0 and 0x00404D20).
- `GFGetSystemManager()->DisplayManager->RM2D->Call(args)`: retail loads `+0x60` before the argument pushes and
  `+8` between them. That needs a named `CDisplayManager*` local. A named render-manager local loads both before
  the pushes, and a fully chained call loads both after them (00404e10).
- An 8-byte struct local whose address is taken inside a loop gives the `and esp,-8` frame. This is confirmed
  again at 00404e10 (C2DVector array).
- STLport `vector<char>::_M_insert_overflow` in a speed TU is vector.c verbatim:
  - `(max)` picks by reference (`lea ecx,[fill_len]` / `lea ecx,[old_size]`).
  - `allocate(n)` is `n ? malloc(n) : 0`.
  - The inline `__copy_trivial` is `last == first ? result : (char*)memmove(...) + n`.
  - `fill_n(char*)` is an intrinsic memset (`rep stosd ; rep stosb` with a bl/bh broadcast).
  - It matched on the first grade (0040f6b0). The unsigned-char twin 0x00485B6C is a size TU that calls the
    `__fastcall` 0x00405BA0 instead.
- STLport copy and destroy helpers used inside a template member need `__forceinline`; otherwise VC7.1 keeps them
  out of line (00412450).
- A block-scoped `CPersistTraits<T>` object shares a frame word with the count read and the resize temporary
  (00412d20, 00412c70).
- In GetMaxAntialiasing, the branch order follows `if (cap != -1) {...} return size()-1;`. A named
  `CSystemManager*` local avoids `add eax,0x60`, and `if (index != -1) return index; return 0;` gives retail's
  tail (00409090).
- CRT `_RTC_Terminate`: a `__try { if (*f) (**f)(); } __except (1) {}` loop under `#pragma optimize("s", on)`
  reproduces the `__SEH_prolog` frame (00401312).
- VC7.1 has no NRVO: `T value; return value;` copy-constructs into the hidden result (0040e1e0). The same applies
  to the named path in GetProfileDir (004082d0).
- VC7.1 has no native wchar_t: L"" literals need a `typedef unsigned short WCHAR`/`wchar16` in prototypes
  (00403480, 00406780).
- When the inline overload of a member template is also emitted, the leaf must be the exact mangled symbol
  (`?resize@?$CRetailVector@VCWideString@@@@QAEXI@Z`, 00412b70).
- Uninitialized fill rows: the element type comes from the inlined copy constructor (vtable stores, CCharString copy
  0x0099EC30, counted-pointer add-ref). `ret 8` means a by-value empty allocator is present, `ret 4` means it is not
  (00b6b700 vs 00b6bb10).
- Relocation masking cannot check which literal goes with which case in a jump-table switch. Verify the case table
  by hand (00415260).

Review at landing:
- ALREADY-LANDED ERROR: rebuild/src/compiled/00/41/Global_GetFontBankName_00415260.cpp maps `case 16` → FRENCH and
  `case 12` → ITALIAN. Retail's case table gives 0x0C → TEXT_FRENCH_MAIN and 0x10 → TEXT_ITALIAN_MAIN (Windows
  LANG_FRENCH = 0x0C, LANG_ITALIAN = 0x10). It grades RELOCATION_MATCH only because relocation masking hides the
  string pairing. This chunk's 00415260 has the correct mapping. The lander may need verify_land_jumptable.py for
  this row.
- Rows that already have earlier-lane landed files but no catalog row:
  - 004014a0: Global_CCRC_Calc_004014a0.cpp
  - 00401b20: Global_GFFreeConfigDetection_00401b20.cpp
  - 00401b80: Global_GFUninitialise_00401b80.cpp
  - 00403480: Global_WinMain_00403480.cpp
  - 00404440: CFileInstallerSingleton_Get_00404440.cpp
  - 00413120: Global_GFInitialiseSetupProgressDisplay_00413120.cpp
  - 00415260: Global_GetFontBankName_00415260.cpp
  Decide whether to replace or skip each one.
- 00405bc0 is a byte twin of the landed global_CopyTrivialBackward_00410d60.cpp, so landing it creates a second
  `_global::CopyTrivialBackward` catalog entry.
- Local base models next to engine headers:
  - 00a2aed0 uses a local virtual CMeshDataBankEntry. engine/CMeshDataBankEntry.h is data-only (`void* __vftable`)
    and cannot express the polymorphic copy.
  - 00b6b700/00b6bb10 use a local virtual CVertexShader.
  - 00412d20/00412c70 derive a reader adapter from engine/CMemoryDataInputStream.h.
- Retail STLport containers are smaller than the Ego_r.pdb (Dinkumware) layouts: vector is 12 bytes and deque 40, so
  CUserProfileManager members after a container sit lower than the PDB says. For example, PendingTutorialCategories
  is at +0x98 in retail and +0xA8 in the PDB (0040bb30).
- 00406780's identity (CUserProfileManager::GetDebugSaveDir) is inferred from the PDB name pair and the body, not
  from a caller.
- 00412d20's T is ICF-ambiguous (any 4-byte T); unsigned long is a choice.
- No row needed more than 4 grades. The highest was 00404e10 with 3.
