# Chunk 775 notes: the last 4 oracle bakes, rewritten as plain C++

Toolchain: VC7.1 RTM cl 13.10.3077 under Wine, on the full mirror (the repo headers plus the `hdr/` overlay). There is no `__asm`, `naked`, `_emit`, `volatile`, shared asm helper or `#pragma` workaround in any row source or test. All four rows grade `genuine` (purity), so ASM_RULES rules 2-4 never apply.

Read-only vland (`vland_result.txt`, run with `--recheck-landed` because all four addresses carry landed bakes): **4 wins, 0 rejects, all behaviour PASS.** The remaining warnings are listed under "vland warnings" below.

All four tests build and pass under g++ and under VC7.1 /Od (`tools/vctest.sh`). Note that vland compiles the tests with VC7.1, so they have to be C++98: no `shared_ptr`, no `<strings.h>`. A missing header made Wine's cl.exe hang until the timeout.

## Results

| address | name | grade | attempts | flags | key spelling |
|---|---|---|---|---|---|
| 0x004167A7 | CMainGameComponent::GetFrameDifferenceFromCurrent | RELOCATION_MATCH | 10 grades + 53 probe compiles | /O1 | `float currentWorldFrame = (float)(ConvertGTToWF(GetCurrentGameTime())); return (long)(currentWorldFrame - (float)frame);` |
| 0x005DACF0 | CTCWeapon::IsAbleToBeUsedToBlock | RELOCATION_MATCH | 1 grade + 23 probe compiles | none | `if (!IsUsableBy(blocker)) return false; if (attacker == NULL) return true; return true;` |
| 0x009A8840 | CBankFileManager::OpenRetailBank | RELOCATION_MATCH | 9 grades | none | shared headers only; the plain in-class CCountedPointer bodies |
| 0x009A8170 | CBankFileManager::OpenIniFile | RELOCATION_MATCH | 6 grades | none | shared headers; inline IsEOF on the fields; a named CWideString for the dev path |

Matched 4, parked 0. `ATTEMPTS.tsv` records every grade and probe compile, with the spelling and what VC7.1 emitted. `LOG.tsv` is the raw grade log.

### 0x004167A7 GetFrameDifferenceFromCurrent
Asm round 2b parked this row after 57 spellings plus flag sweeps.

**The translation unit.**
- The entry is unaligned and the TU has no padding.
- There are `leave` frames and `and dword ptr [mem],0` stores, and `push dword ptr [esp+4]` forwards.
- So it is a `/O1` TU. `#pragma optimize("s", on)` under the base /O2 gives the same bytes here.
- ConvertGTToWF has an unreferenced out-of-line copy in the same TU (0x0041623E), yet retail inlines it into every caller.

**What sets the pop.**
- VC7.1 pops the dead game time with `fstp st(1)` at a float conversion point:
  - after `fisub` in the sibling IsTimeForServerUpdate, whose `float framesAhead = ... - frame` gives its retail bytes;
  - folded into `fdivp` when a plain float local takes the converted value;
  - after `__ftol2` with no float at all.
- The unfolded `fdivr st,st(1); fstp st(1)` right after the divide needs both of these:
  - the **parenthesised** cast `(float)(ConvertGTToWF(...))` (the unparenthesised `(float)ConvertGTToWF(...)` folds to `fdivp`, 31 bytes: k02);
  - a `(float)frame` operand (with `- frame` it folds too: h01).
- A `(float)` cast inside ConvertGTToWF also gives the pop, but adds a copy (d01/e01). Because of that it would break the sibling IsTimeForServerUpdate, so it is not the original.

### 0x005DACF0 IsAbleToBeUsedToBlock
- Retail's `test al,al; jne; ret 0Ch; mov al,1; ret 0Ch` is VC7.1's shape for a `mov al,1` target with two predecessors. The false path returns the call's own zero `al`; landed 0x009FD7F0 IsResourceLoaded and 0x0085EE90 have the same tail.
- Here the second predecessor is an early-out whose two arms both return true: `if (attacker == NULL) return true; return true;`.
- VC7.1 merges the arms only after it has laid out the branches, so the `setne` if-conversion never fires.
- `attacker` is not tested in retail, which is consistent with this. `__assume`, an empty `if` and a scope guard all still give `setne`.

### 0x009A8840 OpenRetailBank
- The model is the shared headers with their plain in-class bodies.
- VC7.1's per-function inline budget alone then reproduces retail's split, with no pragma:
  - **called:** the two (T*) ctors 0x009A9B60 / 0x009A9D10, `operator=` 0x009A9BF0, `Reset` 0x009A9C80, `list::push_back` 0x009AAAA0, `map::operator[]` 0x009AC530, and the version-mismatch exit's `~CCountedPointer<CDiskFileWin32>` 0x009A9BB0;
  - **expanded:** the other two exits' releases.
- The 2026-10-23 local stand-in model declared those members without bodies. It needed a mid-function `#pragma inline_depth(0)` and still differed (1581v1565).
- `#pragma intrinsic(memcmp)` is not needed (/O2 has /Oi). The magic test is `memcmp(magic, "BIGB", 4)`; the literal is 0x0129B07C.

### 0x009A8170 OpenIniFile
- **IsEOF.** It must compare `StreamPos` with `GetLength()` in place. Through the virtual `GetPosition()` it stays a vtable call, and esi is saved for the whole function instead of only around the loop.
- **The dev path.** `CWideString dev_bank_file_path = dev_path.ToWideString();` before `Banks[bank_name] = CBankInfo(dev_bank_file_path, ...)`.
  - Built inline as the first ctor argument, the ToWideString result is passed in eax.
  - That relays out every loop temporary: DIFFER 1719v1732, 248 lines. This is the "slot residue" the 2026-10-23 attempt parked on at 1708v1732.
  - With the named local, the whole function matches.
- **Keywords.** They use the existing inline `CCharString::EqualsCaseInsensitive` (`_stricmp` against a CCharString temporary of each literal).

## Header overlay (`hdr/`, mirror rebuilt with mkmirror.sh)
`hdrcheck2.py $D/hdr` prints `default=same` for every changed header. Every pre-existing FABLE_RETAIL_* configuration is `same` as well; only the new opt-in macros differ.

| header | macro | addition |
|---|---|---|
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_GAMETIME` (nested in FABLE_RETAIL_CMAINGAMECOMPONENT) | protected GetCurrentGameTime (0x00416231), inline ConvertGTToWF, GetFrameDifferenceFromCurrent |
| engine/CTCWeapon.h | `FABLE_RETAIL_CTCWEAPON_BLOCK` (nested in FABLE_RETAIL_CTCWEAPON) | IsAbleToBeUsedToBlock(const CThing&, const CThing*, const CThing*) const |
| engine/CDiskFileWin32.h | `FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR` (nested) | inline `CDiskFileWin32() { Opened = false; }` (FableWin ??0CDiskFileWin32@@QAE@XZ) |
| engine/CAFile.h | `FABLE_RETAIL_CAFILE_SPLITPATH_WIDE` | static __fastcall SplitPath(const CWideString&, CWideString* x4) 0x00997620 |
| engine/CDataInputStream.h | `FABLE_RETAIL_CDATAINPUTSTREAM_TEXT` (nested) | ReadLine 0x00994B50, ReadUntilCharacter(char, bool&) 0x00994DC0, ReadUntilWhiteSpace 0x00994E90, inline IsEOF |
| engine/CMemoryDataInputStream.h | `FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_CHARSTRING` (nested) | ctor(const CCharString&) 0x00995F50 |
| engine/CMemoryFile.h | `FABLE_RETAIL_CMEMORYFILE` (new retail branch) | CMemoryFile : CAFile, inline ctor/dtor, Open 0x009E94A0, Close 0x009E9390, inline GetPathname; sizeof 0x24 |
| fable_string.h | `FABLE_RETAIL_CCHARSTRING_WHITESPACE` (nested) | CCharString::RemoveWhiteSpaceBeginEnd 0x0099F560 |
| fable_retail_banks.h | `FABLE_RETAIL_CBANKFILEMANAGER` (new retail branch) | see below |
| fable_threaded_file.h | `FABLE_RETAIL_CTHREADEDFILE` (new retail branch) | see below |

**fable_retail_banks.h, `FABLE_RETAIL_CBANKFILEMANAGER`.**
- `NBankFileManager::CContainedBank` with the Ego_r member names.
- `NBankFileManager::CRetailBank : CBaseClassNonCopyable`:
  - DiskFile and ThreadedFile are CCountedPointers;
  - UseDMA;
  - `std::map<CCharString,CContainedBank> ContainedBanks`;
  - FilePath.
- `CBankFileManager : CBaseClass`:
  - CBankInfo, with its inline 0- and 4-argument ctors;
  - Banks, `std::map<CCharString,CBankInfo>` at +0x04;
  - RetailBanks, `std::list<CCountedPointer<CRetailBank>>` at +0x10;
  - UsingRetailBanks;
  - BankAliasMap and CreatedRetailBanks (opaque 12 bytes);
  - BasePath at +0x24;
  - FileVersion at +0x34;
  - the private ReadQuotedString 0x009A77D0.

**fable_threaded_file.h, `FABLE_RETAIL_CTHREADEDFILE`.**
- A CThreadedFile with its ctor and Open.
- It deliberately has no `CCountedPointer<CThreadedFile>` specialisation (the default branch keeps one), so it works with engine/CCountedPointer.h.

**Siblings (SIBLINGS.tsv).**
- 3529 landed sources include a changed header, directly or through another header.
- Each was preprocessed under base_include and under the mirror. `fable_static_assert_N` was normalised, as hdrcheck2 does.
- 0 differ, so no landed row can change bytes, and no vland recheck of siblings was needed.

## Header debt
- fable_retail_banks.h now has two models.
  - The default branch keeps the raw-field model, used by the landed CRetailBank ctor/dtor rows and the runtime.
  - Once the runtime switches to these sources, the retail branch should become the only one.
  - BankAliasMap and CreatedRetailBanks stay opaque, because no `stlport` set model exists.
- fable_threaded_file.h's default branch still specialises `CCountedPointer<CThreadedFile>`. That conflicts with engine/CCountedPointer.h in any TU that includes both.
- The landed CCountedPointer helper rows 0x009A9B60 / 0x009A9BB0 / 0x009A9BF0 / 0x009A9D10 are local-model `__fastcall` stand-ins under fake names (NAME_FIXES.tsv). They are worth re-authoring as the engine/CCountedPointer.h instantiations.

## Callee names (bound this chunk, LABELS.tsv)
Every call target bound to exactly one symbol in each row (`tools/callmap.py`).

**Fixes to landed or manifest spellings (NAME_FIXES.tsv).**

| address | correct spelling |
|---|---|
| 0x0099B2C0 | CWideString::ToCharString (not ConvertToCharString) |
| 0x009A9BB0 | ~CCountedPointer<CDiskFileWin32> |
| 0x009A9BF0 | its operator= |
| 0x009A9B60 | the (T*) ctor of CCountedPointer<CDiskFileWin32> |
| 0x009A9D10 | the (T*) ctor of CCountedPointer<CRetailBank> |
| 0x009A9C80 | CCountedPointer<CThreadedFile>::Reset |
| 0x00994700 | CFileDataInputStream(CAFile&, unsigned long) |
| 0x00994780 | the virtual ~CFileDataInputStream |
| 0x00994930 | CMemoryDataInputStream(const void*, unsigned long) |
| 0x00999230 | CAFile::PathExists |
| 0x00997620 | CAFile::SplitPath, the wide overload |
| 0x00994B50 | CDataInputStream::ReadLine |
| 0x009AA7C0 | `_Rb_tree<...CBankInfo...>::_M_erase` |
| 0x009AC5D0 | `map<CCharString,CBankInfo>::operator[]` |

## vland warnings (non-blocking)
- RELOC_NAME on 0x009A9BB0 and 0x009A9BF0: the labels `_global::CCountedPointer_Release_009a9bb0` / `_Set_009a9bf0` are the stand-in names (fixed in NAME_FIXES).
- RELOC_ALIAS on `__stricmp` (the MSVCR71 import thunk). Every `_stricmp` caller gets this.

## Files
- `src/` holds 4 sources and `test/` holds 4 tests; `differ/` is empty.
- `ATTEMPTS.tsv`, `STATUS.tsv`, `NAME_FIXES.tsv`, `LABELS.tsv`, `SIBLINGS.tsv`, `payload.json`, `vland_result.txt`.
- Tools: `tools/fx.sh` (compile-only compact disassembly), `tools/var4167.py` / `tools/var5da.py` (probe sweeps), `tools/callmap.py` (call-target binding), `tools/sibcmp.sh` (sibling preprocessing check), `tools/vctest.sh` (VC7.1 test run), `tools/mkpayload.py`.

## GOTCHAS one-liners
- VC7.1 x87: `(float)(f(x))` with parentheses and `(float)(f(x))` without them compile differently. The parenthesised cast of an inlined double result keeps `fdivr st,st(1); fstp st(1)` unfolded; the bare cast folds to `fdivp` (0x004167A7).
- An inlined double function's dead argument is popped at the next float-conversion point, or else after `__ftol2` at the end of the statement. Steer the `fstp st(1)` with a float local or cast, not with flags (0x004167A7, 0x0041674A).
- `test al,al; jne; ret; mov al,1; ret` (no setne) is a `return true` reached from two branches. A redundant early-out with both arms returning true (`if (p == NULL) return true; return true;`) reproduces it (0x005DACF0).
- Large bodies whose retail calls some CCountedPointer/STL members out of line and expands others: include the shared header with plain in-class bodies and let VC7.1's inline budget choose. Declared-only local models need pragmas and still differ (0x009A8840).
- A by-value conversion result (`x.ToWideString()`) passed straight to a const-ref ctor parameter goes in eax and relays out the loop temporaries. Retail named it first (`CWideString w = x.ToWideString();`; 0x009A8170).
- An inline helper that calls a virtual (`IsEOF` through `GetPosition()`) stays a vtable call even on a local object. Read the field directly (0x009A8170).
- vland's purity check is textual. The words `_emit`, `naked` or `__asm` in a comment grade the row NOT_GENUINE.
- vland compiles behaviour tests with VC7.1 /Od, so keep tests C++98 (no shared_ptr, no `<strings.h>`). A missing header can hang Wine's cl.exe until the timeout.
