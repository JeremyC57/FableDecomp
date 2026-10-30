# Toolchain-library signatures (grade `toolchain_library`)

Retail `Fable.exe` contains bytes that no Fable source produced: the VC7.1 CRT objects the linker pulled out of
`msvcrt.lib` (`sehprolg.obj`, `secchk.obj`, `crtexew.obj`, the 64-bit math helpers, ...) and the
`jmp dword ptr [__imp_X]` thunks the linker writes for import-library records. Owner rule (2026-10-30): identify
and signature them, link the same libraries instead of reconstructing them, and keep them apart from Fable logic
on the dashboard. This page covers the scan, the oracle it writes, the `toolchain_library` grade and how to rerun
the proof.

## Which library

Retail imports **MSVCR71.dll** and **MSVCP71.dll**, so it linked the `/MD` import libraries of **VS .NET 2003**
(`Vc7/lib/msvcrt.lib`, `msvcprt.lib`, cl 13.10.3077 RTM) and their static members. Two facts confirm it:

* the scan matches 28 code sections of that `msvcrt.lib` byte for byte, with every relocation that can be resolved
  agreeing with retail (IAT slots, import thunks, RTTI vftables, same-object labels);
* the matched objects' `@comp.id` stamps are entries of retail's Rich header: 11 are `0x000F` build 3077 (MASM
  7.10: `sehprolg`, `chkstk`, `ftol2` and the eight 64-bit math helpers; retail lists 12 such objects), 5 are
  `0x005F`/3077 (C: `crtexew`, `atonexit`, `fp8`, `secchk`, `seccinit`; retail lists 51) and 4 are `0x0060`/3077
  (C++: `initsect`, `ehvecdtr`, `ehvecctr`, `ti_inst`; retail's 1,369 C++ objects include them).

**The lane's `$FABLE_VC71/lib` is not that library.** `$S/wine/vc71rtm/lib` is a symlink to `$S/wine/vc71/lib`,
which is the WinDDK 3790.1830 CRT (`lib/crt/i386`): its `msvcrt.lib` imports `msvcrt.dll`, has no `secchk.obj`
(the DDK's `/GS` check lives in `bufferoverflowu.lib`), and its `__security_check_cookie` is a different 22-byte
body (an extra `test ecx,0FFFF0000h`) that does **not** match retail 0x00BFE9F9. Its `sehprolg.obj` and the math
helpers are byte-identical to the VS 2003 ones (they are listed as alternates in the oracle `notes`).

The VS 2003 files come from the archive.org item the lane's RTM `cl.exe` came from (`docs/BUILDING.md`), fetched
member by member with SHA-1 checks:

```
bash tools/decomp_pipeline/libsig/fetch_vs2003_libs.sh $S/wine/vs2003lib     # 21 files, ~20 MB; never commit them
export FABLE_VS2003_LIB=$S/wine/vs2003lib                                    # tools also try $S/wine/vs2003lib, $S/libsig/work/vs2003_lib
```
On Windows the RTM toolset's own `D:\Tools\vc71\lib` is the same library (`build_bootstrap.ps1` defaults
`FABLE_VS2003_LIB` to `$vcRoot\lib`).

## Tools (`tools/decomp_pipeline/libsig/`, stdlib Python 3)

| Tool | What it does |
|---|---|
| `coff_lib.py` | Reads `!<arch>` archives (linker members, longnames, COFF objects, short import records `00 00 FF FF`) and i386 objects: sections, relocations, symbols, COMDAT selection, `@comp.id`. `python3 coff_lib.py <lib> --members / --code / --imports / --member NAME` |
| `libsig_scan.py` | The scan below; writes `rebuild/oracles/library_signatures.tsv` and, with `--log DIR`, `scan.log` (thunk classification, ambiguous and dropped matches) and `unlinked_matches.tsv` |
| `lib_verify.py` | Re-proves a row: `lib_verify.py <addr>...` or `--source <stub.cpp>`; prints `MATCH` / `MISMATCH`, exit 1 on any mismatch. Import libraries are found in the library directories, then in the import-library directories (`default_implibdirs`, the WinDDK `wnet/i386` copies); an import library that is not on disk (`NOT_ON_DISK`: only the DirectX SDK `d3dx9.lib`) is proven from the retail import directory alone and the reason says so |
| `libsig_cross.py` | Crosses the oracle with the landed catalog, the manifest and the pending pool; lists the landed hits with their purity class |
| `fetch_vs2003_libs.sh` | Fetches the VS 2003 `Vc7/lib` files (above) |

Rerun (lane env sourced, `FABLE_EXE` set; about 45 s):
```
python3 -I tools/decomp_pipeline/libsig/libsig_scan.py --log $S/libsig/logs/scan
python3 -I tools/decomp_pipeline/libsig/libsig_cross.py
python3 -I tools/decomp_pipeline/libsig/lib_verify.py $(awk -F"'" '/Grade = .toolchain_library./{print a} /Address = /{a=$2}' rebuild/build_candidates.ps1)
```
(the awk lists every `toolchain_library` catalog row: 128 since 2026-10-30, all MATCH.)

## How the scan works

1. **Library side.** Every code section of at least 8 bytes in every object of every archive (and loose `.obj`) in
   the library directories (`vs2003rtm` = the VS 2003 copy first, then `ddk3790` = `$FABLE_VC71/lib`; override with
   `FABLE_LIBSIG_LIBDIRS=tag=dir:tag=dir`) is masked at its relocation fields. 21,052 sections give 5,671 distinct
   masked patterns.
2. **Search.** Each pattern's longest unmasked run is searched in retail `.text` and every hit is checked byte for
   byte outside the relocations.
3. **Linked libraries only.** Retail links `msvcrt.lib`, `msvcprt.lib`, `kernel32.lib`, `oldnames.lib`,
   `comsupp.lib` and the loose `Vc7/lib` objects of VS 2003. A pattern found only in `libc*`, `libcp*` or a debug
   library is not linked code: it is STL header code a Fable TU compiled (`std::ctype<char>` dtor shapes,
   `exception::scalar deleting destructor` with 494 copies) or an identical small body. Those 37 patterns go to
   `unlinked_matches.tsv` with their relocation evidence and never into the oracle.
4. **One retail address, one library symbol.** A pattern with one retail copy takes the provider whose
   relocations agree best (`msvcrt.lib` first, then `msvcprt.lib`, other linked libraries, unlinked, debug). A
   pattern with several copies keeps only the copy the relocations prove (`type_info`'s copy constructor has 149
   identical copies; the RTTI check of its vftable relocation picks 0x00BFE960) or, when one object has as many
   identical sections as there are copies, assigns them in object section order (`initsect.obj`:
   `__RTC_Initialize` 0x004012CE, `__RTC_Terminate` 0x00401312). The rest are logged as ambiguous (152: 148
   `type_info` copies, 4 `comsupp` shapes). Identical bytes in two members of one library go to the member another
   matched section proves was linked (`check_managed_app` -> `crtexew.obj`, which also gives `_WinMainCRTStartup`).
   Aliases at one offset are one row (`__chkstk` / `__alloca_probe`).
5. **Relocations.** Each fixup is resolved against retail: `__imp_X` must be the IAT slot the import directory
   names X; a code import must be the `jmp [__imp_X]` thunk of X; a `??_7<class>6B@` vftable must carry that
   class's RTTI TypeDescriptor; a same-section label or another matched library symbol must be at its matched
   address. `[ok]` is a proof, `[sec]` only says the target is in the right section, `[bad]` is a contradiction.
   Confidence `high` = unique (or proven) with no contradiction and a proof or >= 24 unrelocated bytes.
6. **Import thunks.** Every `FF 25 <slot>` whose slot is in the IAT is a thunk when it is a direct call/jmp
   target, a manifest function start, next to another thunk, or next to a matched library section (0x0040136E
   `__setusermatherr` follows `fp8.obj`). 420 of the 447 sites are thunks; the other 27 are compiled tail jumps
   through the IAT inside functions (EH funclets, 16-byte-aligned API wrappers). Each thunk is tied to the short
   import record that makes the linker emit it (VS 2003 `msvcrt.lib` / `msvcprt.lib` / `kernel32.lib`, else the
   WinDDK `wnet/i386` import library; `FABLE_LIBSIG_IMPLIBDIRS`).

## What it found (2026-10-30)

`rebuild/oracles/library_signatures.tsv`: **448 rows**, all `high`: 28 CRT code sections + 420 import thunks.

| Bucket | Sections | Thunks | Meaning |
|---|---:|---:|---|
| landed rows | 11 | 15 | vendor bytes the catalog landed: 3 sections + 15 thunks are now `toolchain_library`, 8 sections stay genuine CRT-source compiles (`// Library:` line) |
| unlanded manifest functions | 12 | 98 | link work, not reconstruction: all 110 landed as `toolchain_library` 2026-10-30 |
| outside the manifest | 5 | 307 | not catalogued as functions |

Sections: `msvcrt.lib` `crtexew.obj` (`check_managed_app`, `_WinMainCRTStartup`), `atonexit.obj` (`__onexit`,
`_atexit`), `initsect.obj` (`__RTC_Initialize`, `__RTC_Terminate`), `fp8.obj`, `sehprolg.obj` (`__SEH_prolog`,
`__SEH_epilog`), `ti_inst.obj` (`type_info` copy ctor, vector deleting dtor), `secchk.obj` (`report_failure`,
`__security_check_cookie`), `chkstk.obj`, `ftol2.obj`, `llmul/ullrem/llrem/ullshr/ulldiv/lldiv/llshr/ulldvrm.obj`,
`ehvecdtr.obj` (`ArrayUnwindFilter`, `__ArrayUnwind`, `eh vector destructor iterator`), `ehvecctr.obj`,
`seccinit.obj`. Thunks: 120 `msvcrt.lib` (MSVCR71), 3 `msvcprt.lib` (MSVCP71), 120 `kernel32.lib`, 75 user32, 23
winmm, 21 wsock32, 17 imm32, 9 advapi32, 7 ole32, 5 gdi32, 3 version, 2 shell32, 2 oleaut32, 1 d3d9, 1 dinput8,
11 d3dx9_25.

## The grade

`crawl/purity.py` classifies a landed file as `toolchain_library` when it carries

```
// Toolchain-library: <lib>(<member>) <symbol> -- <n> bytes verified against Fable.exe 0x<ADDR>
```

and has no function body (no `{` outside comments and strings) and no `__asm`, `naked` or `_emit` anywhere. The
check runs before the asm rules. The grade counts with the genuine rows (`is_genuine_kind`), like `retail_asm`: the
bytes are the toolchain's and the build links that library. The stub is documentation plus the proof line.

* **Landing:** by hand only. Write the stub, set `Grade = 'toolchain_library'` on the catalog entry, run
  `lib_verify.py <addr>` (MATCH). `verify_and_land.py` never lands one from a payload.
* **Recheck:** `recheck.py` + `vland.py --recheck-landed` prints `LIBRARY_MATCH` / `LIBRARY_MISMATCH` (or
  `LIBRARY_UNVERIFIED` without `FABLE_EXE` or the library) from `lib_verify.py` instead of compiling.
  `reloc_audit.py` writes `LIBRARY` and `reloc_rename.py` skips the row. `build_candidates.ps1` records
  `status = LIBRARY` and compiles nothing; `build_bootstrap.ps1` proves 0x00401284 with `lib_verify.py` when
  `FABLE_EXE` is set.
* **Dashboard:** "Landed rows that are toolchain/vendor library bytes (grade `toolchain_library`)"; the asm-bake
  line no longer counts them.

Landed under the grade (2026-10-30): **128 rows**, every one `lib_verify.py` MATCH and `vland.py --recheck-landed`
LIBRARY_MATCH (landing-pass hand-back: `lib_verify_all.log`, `vland_recheck_library.log`).

* First pass (4): 0x0040138C `__SEH_prolog` (59), 0x004013C7 `__SEH_epilog` (17) from `sehprolg.obj`; 0x00BFE9F9
  `@__security_check_cookie@4` (14) from `secchk.obj` (both relocations proven: `___security_cookie` 0x0139C8A8 holds
  `seccook.obj`'s 0xBB40E64E, `_report_failure` 0x00BFE9C8 holds `secchk.obj`'s `report_failure`); 0x00401284 `_exit`
  thunk (IAT 0x014401A0 = MSVCR71.dll!exit). The first three were `asm_bake` rows; 0x00401284 had no catalog entry.
* Re-graded (14): the MSVCR71 import thunks that had landed as C++ stand-ins compiling to the same `jmp [slot]`
  (0x00401356 `_initterm`, 0x004013DE `__dllonexit`, 0x00BFE9BC `operator delete`, 0x00BFEA0E `malloc`, 0x00BFEA14 `free`,
  0x00BFEA1A `operator new`, 0x00BFEAE6 `memmove`, 0x00BFEAF2 `puts`, 0x00BFEB1C `operator delete[]`, 0x00BFEB22
  `operator new[]`, 0x00BFEB3A `strstr`, 0x00BFED16 `toupper`, 0x00BFED40 `_wgetcwd`, 0x00BFED4C `_wsplitpath`); most
  were a generic `operator_delete` calling a `free` function pointer. Their test files stay but are not built.
* Landed (110): the 12 CRT sections that sat parked as unauthored DIFFERs (`_report_failure`, `__chkstk`, `__ftol2`,
  `__allmul`, `__aullrem`, `__allrem`, `__aullshr`, `__aulldiv`, `` `eh vector constructor iterator' `` (98 bytes: the
  section includes its `__finally` funclet, so the manifest row 0x00F3FBE0 is that funclet), `__alldiv`, `__allshr`,
  `__aulldvrm`; parked copies removed) and the 98 catalogued import thunks (68 MSVCR71, 13 imm32, 10 d3dx9_25, 3
  version, d3d9, dinput8, winmm, wsock32).
* Not landed: the 312 oracle rows outside the manifest (5 CRT functions, 307 thunks) wait for a manifest boundary
  pass; the proposed names and stub paths are in the landing-pass hand-back `OUTSIDE_CATALOG.tsv`.

The 8 genuine VC7.1 compiles of CRT source (WinMainCRTStartup, `_onexit`, `atexit`, the two RTC walkers,
`_setdefaultprecision`, `__ArrayUnwind`, `eh vector destructor iterator`) keep grade `genuine` and carry a provenance
line `// Library: <lib>(<member>) <symbol>`; `tools/landed_metrics.py` counts them as `library_source` and the
dashboard lists them as vendor source.

Callers respelled in the landing pass (one retail address, one name): 133 landed rows plus the two headers
`fable_boot.h` and `fable_user_profile.h`, each rechecked MATCH/RELOCATION_MATCH + PASS with `vland.py --recheck-landed`.

* 69 `free` callers that called an invented `my_free` / `FreeThing` / `AutoTinyOptionalTarget` now declare
  `extern "C" void __cdecl free(void*)`; 25 `push ecx ; call 0x00BFE9BC ; pop ecx ; ret` rows (copies of an `operator_new`
  stand-in calling an invented `helper_bfeb1c`) and 0x004C73B0 / 0x00C49C90 now call `::operator delete`,
  `::operator delete[]` and `::operator new[]`.
* `rand`, `sprintf`, `_stricmp` (two rows had `strcmp`), `wcscmp`, `_getdrive` / `_chdrive` / `_wchdir` / `_wmkdir` /
  `_wsplitpath` / `_wmakepath`, `_onexit`, `timeSetEvent`, `D3DXFilterTexture`, `D3DXMatrixInverse` and `terminate`
  take their library spelling; two invented callees (`Finalize`, `sub_bfea70`) were the compiler's `__ftol2` after a
  float-returning call, so the callee's return type is now `float`.
* 0x00689370 is the float `floor` overload (it calls 0x00BFEB8A, IAT slot 0x01440100 = MSVCR71!floor); it had landed as
  `ceil`, whose float overload is 0x0057A2D0 (calls 0x00BFEB9C). The row and its four callers now spell
  `?floor@@YAMM@Z`.
* `reloc_bindings.tsv`: 69 invented spellings retired, 9 library spellings added; `bindings_multi_check.py` prints 0.

Library spellings that stay different, with the reason:

| Address | Library symbol | Landed spelling | Why |
|---|---|---|---|
| 0x00BFEA30 | `__chkstk` | `__alloca_probe` (12 callers) | `chkstk.obj` defines both labels at offset 0; VC7.1 emits `__alloca_probe` for `_alloca` |
| 0x00BFEE7A | `?__ArrayUnwind@@YGXPAXIHP6EX0@Z@Z` | `___ArrayUnwind@16` | the `__thiscall` destructor pointer cannot be spelled: VC7.1 rejects `__thiscall` on a non-member pointer (C4234), and `vc71()` rewrites it to `__fastcall` |
| 0x00BFEC30 | `__allrem` | `__aullrem` (0x00785B40) | the row's modulo is unsigned; it should be signed `__int64`. The row carries a `retail_asm` block, so it is left for the coordinator (`NAME_FIXES.tsv`) |

### Landing a future oracle row

1. `lib_verify.py <addr>` must print MATCH from the oracle row (for an import thunk whose library is not on disk, add
   the library to `NOT_ON_DISK` only with a cited source and write the decorated symbol in the stub).
2. Write the stub at `rebuild/src/compiled/<xx>/<yy>/global_<leaf>_<addr>.cpp` (`<leaf>` = the export or C symbol,
   letters and digits only, as `make_base` would spell it): a header line, the `// Toolchain-library:` claim with the
   **decorated** symbol the linker resolves (`_free`, `__CxxThrowException@8`, `_ImmGetContext@4`,
   `??_L@YGXPAXIHP6EX0@Z1@Z`), the bytes and relocations it proves, and the `Proof:` line. No braces, no asm.
3. Catalog entry in `build_candidates.ps1` (CRLF): `Address`, `Module = '_global'`, `Source`,
   `Grade = 'toolchain_library'` (no test). `auto-re-candidates.tsv` row: address, name, oracle length, retail bytes.
   Names: a CRT section takes its C symbol (`__allmul`, `_report_failure`); a thunk takes `<DLL>::<export>` as the
   import directory spells it (`MSVCR71.DLL::_amsg_exit`, `IMM32.DLL::ImmGetContext`) except the C++ operator exports,
   which stay `MSVCR71.DLL::operator_new` / `operator_delete` / `operator_new[]` / `operator_delete[]`
   (`reloc_identity.py` compares a caller's operator claim against that spelling and would otherwise reject every
   `??2@YAPAXI@Z` caller).
4. Recheck: `recheck.py` + `vland.py --recheck-landed` (LIBRARY_MATCH); remove any parked copy from the pending pool.
5. One retail address, one name: the callers' symbol for a thunk is the import symbol (`_rand`, not `?rand@@YAHXZ`;
   `__imp__rand` when they call through the IAT slot), so declare CRT functions `extern "C"` without `dllimport`
   (a `dllimport` declaration emits `call [__imp__X]`, a different instruction). A caller that called an invented
   stand-in (`my_free`, `Finalize` for `__ftol2`) is respelled to the real call and rechecked; its behaviour test
   replaces the CRT function instead (`extern "C" void __cdecl free(void*)` in the test object, counting only after
   `main` starts because the CRT frees its own blocks during start-up). VC7.1 predeclares only the scalar
   `operator new` / `operator delete`, so a row calling `::operator new[]` or `::operator delete[]` declares it.
6. Find the callers from the relocation pairs of every landed row (`reloc_identity.check` on each compiled row),
   not from `reloc_bindings.tsv` alone: the table records a symbol once, so a stand-in spelling first seen elsewhere,
   or never recorded, does not list its other callers.

## Other vendor code (scoped, no library bytes on disk)

| Vendor | Evidence | Status |
|---|---|---|
| D3DX9 (DirectX SDK April 2005, `d3dx9_25.dll`) | 11 import thunks to `d3dx9_25.dll` | import library `d3dx9.lib` not on disk; thunk bytes do not depend on it |
| Direct3D 9 / DirectInput 8 import libraries | `d3d9.dll` (1 thunk), `DINPUT8.dll` (1) | named from the WinDDK `wnet/i386` libraries |
| zlib 1.2.1 | strings ` deflate 1.2.1 Copyright 1995-2003 Jean-loup Gailly`, ` inflate 1.2.1 ...`; `CCompressorZlib` | compiled from source into Fable, no prebuilt library: reconstruction or a future vendor-source grade |
| libVorbis 1.0.1 / libogg / vorbisfile | `Xiph.Org libVorbis I 20030909`; landed `ogg_stream_packetout` (/Gr); `_ov_d_seek_lap` callees take `vf` in esi (whole-program optimised) | compiled from source; same as zlib |
| OpenAL | `/OpenAL32.dll`, `/wrap_oal.dll` strings | loaded at run time, no static code expected |
| XMV video | `CXMVPlayer`, `.xmv` / `.wmv` files | player is Lionhead code; any decoder library is unidentified |
| STL | 9 `std::` RTTI names, MSVCP71 imports | header templates compiled in Fable TUs, not a library |
