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
| `lib_verify.py` | Re-proves a row: `lib_verify.py <addr>...` or `--source <stub.cpp>`; prints `MATCH` / `MISMATCH`, exit 1 on any mismatch |
| `libsig_cross.py` | Crosses the oracle with the landed catalog, the manifest and the pending pool; lists the landed hits with their purity class |
| `fetch_vs2003_libs.sh` | Fetches the VS 2003 `Vc7/lib` files (above) |

Rerun (lane env sourced, `FABLE_EXE` set; about 45 s):
```
python3 -I tools/decomp_pipeline/libsig/libsig_scan.py --log $S/libsig/logs/scan
python3 -I tools/decomp_pipeline/libsig/libsig_cross.py
python3 -I tools/decomp_pipeline/libsig/lib_verify.py 0040138c 004013c7 00bfe9f9 00401284
```

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
| landed rows | 11 | 15 | vendor bytes the catalog landed (4 now `toolchain_library`, see below) |
| unlanded manifest functions | 12 | 98 | link work, not reconstruction |
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

Landed under the grade (2026-10-30): 0x0040138C `__SEH_prolog` (59), 0x004013C7 `__SEH_epilog` (17) from
`sehprolg.obj`; 0x00BFE9F9 `@__security_check_cookie@4` (14) from `secchk.obj` (both relocations proven:
`___security_cookie` 0x0139C8A8 holds `seccook.obj`'s 0xBB40E64E, `_report_failure` 0x00BFE9C8 holds
`secchk.obj`'s `report_failure`); 0x00401284 `_exit` thunk (IAT 0x014401A0 = MSVCR71.dll!exit). The first three were
`asm_bake` rows; 0x00401284 had no candidate-catalog entry (only `build_bootstrap.ps1`) and gained one.

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
