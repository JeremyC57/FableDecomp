# Hand chunk 409 (CAFile .. GFUIntToCharString): 33 of 40 byte-exact

All 40 rows are `_global` in the catalog. By body they are CAFile path statics, CWideString/CCharString helpers, STLport
`basic_string<unsigned short>` members, the TLC error-dialog Win32 helpers (FableWin.pdb names) and the GF string
parsers. Every src/ file grades RELOCATION_MATCH in grades.txt. Its test prints PASS under g++ (tall.sh) and VC7.1
(tall.py).

## What matched (33)
- **CAFile path statics** (`__fastcall` statics; a by-value result takes the hidden pointer in ecx).
  - 00999140 ConvertPathToFullPath(wide, wide&) is `fullPath = ExpandPath(path, GetCurrentPath()); return true;`.
  - 009990b0 is the narrow overload. Its ToWideString result must be a named local (the inline wide overload is
    expanded), otherwise VC caches eax in edi.
  - 00998c30 GetAbsoluteFilePath(wide, wide) narrows both arguments and widens the answer.
  - 009993b0 IsFileReadOnly, 00999420 SetFileReadOnly, 00999a30 IsPathFile, 009992a0 GetFileModifiedTime and 00999320
    GetFileModifiedTimeAsSomeWackyWin32BigNumber share the PathExists prologue
    (`CWideString fullPath; fullPath = ExpandPath(...)`).
    - 00999320 keeps `__allmul` through `(u64)high * 0x100000000ui64`.
  - 00999760 StartFileScan(..., wchar_t*) (/GS): a positive `if (handle != -1) {...; return true;}` puts the failure arm
    last.
  - 00999c50 StartFileScan and 00999cb0 ScanNextFile (the CWideString& overloads, /GS) copy a 262-character stack buffer
    into the string.
  - 00999180 GetDiskSectorSize and 00999970 ValidatePathnameIsOnWriteableDrive (/GS) need the split buffers in an
    inner block, so they share the dead temporary's slot.
  - 00998170 ParserReadPastDriveLabel and 00997e90 ParserIsSeparatorNext need `__forceinline` on both the CCharString
    and the CBasicString `operator==` levels.
  - 00998210 ParserIsDriveLabelNext uses the retail 0x28-byte CStringParser.
  - 00998e70 CopyFileW: modelling the buffer as an STLport `vector<unsigned char>` (malloc/free allocator) is what
    gives retail's 0x18 frame.
- **CWideString / CCharString**:
  - 0099b770 CWideString::SetAsSubString. The inline UnassignString must decrement through a LOCAL copy of the block
    pointer and re-read the member for `delete`. That is the shape of the landed 0x0099B4D0: `mov edx,[eax+0xc]; dec
    edx; mov ecx,edx; test ecx,ecx; mov [eax+0xc],edx`.
  - 0099bff0 operator+(const wchar_t*, const CWideString&) replaces the landed asm-bake. It needs same-TU noinline
    bodies of every address-taking callee (ctor, +=, UnassignString, AllocStringData, MakeOwnCopyOfData), so the
    result's block stays in esi.
  - 0099f690 operator+(const char*, const CCharString&).
  - 0099f830 GFIntToCharString ("%d") and 0099f8b0 GFUIntToCharString ("%u") use the landed 0x0099F930 copy/dtor model.
  - 0099e620 GFIsCharStringNumber, 0099e7f0 GFCharStringToInt and 0099e870 GFCharStringToUHUGE use the fetch-ahead loop
    (`char c = GetChar(i); while (i < len) {...; c = GetChar(++i);}`).
    - 0099e7f0 and 0099e870 use an unsigned index.
- **STLport basic_string<unsigned short>** (`namespace _STL`, `class char_traits`, `class allocator`):
  - 0099c990 reserve is the landed char reserve 0x00777DB0 at element size 2.
- **TLC error-dialog helpers** (all `__fastcall`; the extern "C" ones use the leafs `@Name@N`):
  - 0099d3a0 RetrieveStringByLanguage walks the string-table block with an unsigned index; the terminator goes at a
    signed min(n, size-1).
  - 0099d470 RetrieveString tries the module language, then 0x409, then LoadStringA.
  - 0099d4d0 LocalizedDialogBoxByLanguage needs a separate early `return -1` for each failure. Nested ifs put the
    shared failure block after the success call.
  - 0099d520 LocalizedDialogBox: with the plain same-TU form, VC7.1 itself inlines the first by-language call and keeps
    the 0x409 retry as a call.
  - 0099d7d0 ConvertStaticToHyperlink (/GS) is Neal Stublen's CodeProject routine verbatim.
  - 0099dbe0 SetDirtyExitFlag (/GS) only reads ExitFlag and returns 0.
  - 0099dc50 ClearDirtyExitFlag writes "clean".

## Parked for Fable 5.1 (7)
- 00997f80 CAFile::GetCurrentPath DIFFER(132v132): both `return CurrentPath;` copies emit `mov ecx,esi` before `push offset CurrentPath` (retail pushes first) | tried: inline bool `operator==(L"")` (neg/sbb/inc), the goto form with a `__forceinline` STLport `operator==` (traits length inline, compare out of line at 0x403C60), the 0x005BDA1D two-test form; 4 grades | next: lane gap: scheduling (instruction order only)
- 0099e690 GFCharStringToFloat DIFFER(180v180): VC7.1 enregisters the divisor (three `fld` at entry, `fiadd` in the whole branch, the negative flag spilled); retail keeps the divisor in a frame word (`mov [esp+8],1.0f` / `fdiv dword [esp+8]`), loads the digit with `fild` before `whole*10`, and keeps the flags in bl/dl | tried: local declaration orders, `whole * 10.0f + (float)digit` vs `(float)(whole * 10.0f + digit)`, `divisor *= 10.0` vs `divisor = divisor * 10.0` (probe/e690_a.cpp, e690_b.cpp); 4 grades | next: make the divisor's address escape (pass `&divisor` to a same-TU noinline no-op) or try `#pragma optimize("y")` variants
- 0099bb80 CWideString::SprintfDynamic DIFFER(199v197): `lea esi,[eax+0x1000]` after the wcslen call is `mov esi,eax ; add esi,0x1000` in ours; every other instruction is relocation-identical | tried: size before or after the vector (only vector-first folds the resize), a same-TU noinline `_M_fill_insert` stand-in (keeps begin() in edi), `__forceinline` operator=/UnassignString, int vs unsigned size; 4 grades | next: declare wcslen through the CRT header (intrinsic attributes), or compute the size inside the resize argument
- 0099c670 basic_string<unsigned short>::_M_range_initialize DIFFER(98v97): only the trailing 0xCC after the final noreturn `_M_throw_length_error` call; RELOCATION_MATCH against the 98-byte row read from Fable.exe (probe/n1.py) | tried: STLport verbatim, 1 grade | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row)
- 0099ca70 basic_string<unsigned short>::basic_string(size_type, unsigned short, const allocator&) DIFFER(118v117): only the trailing int3-after-noreturn byte; RELOCATION_MATCH against the 118-byte row | tried: STLport `_String_base(a, n + 1)` + `uninitialized_fill_n` (VC7.1 emits the rep stosd/stosw itself), 1 grade | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row)
- 0099cb00 basic_string<unsigned short>::append(size_type, unsigned short) DIFFER(145v144): only the trailing int3-after-noreturn byte; RELOCATION_MATCH against the 145-byte row | tried: STLport verbatim, 1 grade | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row)
- 0099cc80 basic_string<unsigned short>::push_back DIFFER(141v140): only the trailing int3-after-noreturn byte; RELOCATION_MATCH against the 141-byte row | tried: the verbatim inline append (139v140, c and count homed wrong), then the landed 0x0099B9A0 inline form `fill = c; count = n;`; 2 grades | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row)

The four int3 rows have behaviour tests next to their sources (differ/<addr>.test.cpp, PASS under g++ and VC7.1).

## Skipped
None. Every row holds exactly one function. The only length mismatches are the four int3-after-noreturn rows above,
which are one byte short in the chunk.

## Label corrections
- 00997f80 "CInputProcessMain_GetCameraPath" -> CAFile::GetCurrentPath (FableWin.pdb `?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ`).
- 00999320 "GetFileModifiedTime" -> CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber.
- 00999760 "CAFile_FindFirstFile" -> CAFile::StartFileScan(const CWideString&, CFileScanInfo&, wchar_t*).
- 00999c50 "ScanNextFile" -> CAFile::StartFileScan(const CWideString&, CFileScanInfo&, CWideString&).
- 0099b770 "CCharString_Replace" -> CWideString::SetAsSubString(const wchar_t*, long).
- 0099bb80 "CCharString_FromWideFormatted" -> CWideString::SprintfDynamic(const wchar_t*, ...) (`?SprintfDynamic@CWideString@@QAAXPB_WZZ`).
- 0099c670 "CCharString_AllocStringData" -> _STL::basic_string<unsigned short>::_M_range_initialize.
- 0099c990 "String_ResizeBuffer" -> _STL::basic_string<unsigned short>::reserve.
- 0099ca70 "WideString_Allocate" -> _STL::basic_string<unsigned short>::basic_string(size_type, unsigned short, const allocator&).
- 0099cb00 "std_vector_resize" -> _STL::basic_string<unsigned short>::append(size_type, unsigned short).
- 0099cc80 "CTCElectrocutedEffect_PushEffect" -> _STL::basic_string<unsigned short>::push_back(unsigned short).
- 0099d470 "Utility_LoadStringByLanguageWithFallback" -> RetrieveString (`@RetrieveString@16`).
- 0099e870 "ParseLongLong" -> GFCharStringToUHUGE.
- 0099f690 "CCharString_OperatorPlus_API" -> operator+(const char*, const CCharString&).
- 0099f830 "GFIntToCharString_API" -> GFIntToCharString(long).
- 0099f8b0 "CCharString_Format" -> GFUIntToCharString(unsigned long).
- Callee 0x005149F0 "vector_insert_erase" -> _STL::vector<unsigned short>::_M_fill_insert(iterator, size_type, const unsigned short&).
- Callee 0x00403C60 "MemCmp_Unsigned16" -> char_traits<wchar_t>::compare.
