# Vendor-library signature pass (2026-10-30): 448 retail functions proven to be toolchain bytes; 4 CRT rows landed as `toolchain_library`

Owner rule (2026-10-30): identify, isolate and signature known runtime / third-party library bytes so the build links the
same libraries instead of reconstructing them, and the dashboard separates vendor bytes from Fable logic. Brief BRIEF.md;
oracle SIGNATURES.tsv (= rebuild/oracles/library_signatures.tsv, 448 rows); logs/ (scan, crossing table cross.tsv, proofs,
rechecks, dashboard deltas, VS2003 member SHA-1s). Method and re-run instructions: docs/pipeline/LIBRARY_SIGNATURES.md;
tools in tools/decomp_pipeline/libsig/ (coff_lib.py archive parser, libsig_scan.py ~45 s, lib_verify.py, libsig_cross.py,
fetch_vs2003_libs.sh).
**Library premise corrected:** the lane's `vc71rtm/lib` is a symlink to the WinDDK 3790 CRT (imports msvcrt.dll, no secchk.obj,
a different 22-byte cookie check). The pass fetched the real VS .NET 2003 `Vc7/lib` (21 files, SHA-1 checked, from the archive
item docs/BUILDING.md already names; Microsoft binaries, kept in the lane at `$S/wine/vs2003lib`, `FABLE_VS2003_LIB` in env.sh,
never committed). Its msvcrt.lib imports MSVCR71.dll like retail, and the matched objects' `@comp.id` stamps (MASM 0x0F/3077,
C 0x5F/3077, C++ 0x60/3077) all appear in retail's Rich header.
Hits (all high confidence; a masked byte match alone was not enough, every hit carries relocation proof: IAT slot, thunk, RTTI
TypeDescriptor or bytes the same library defines): 28 CRT code sections + 420 import thunks.
- (a) 26 landed rows: the 4 now `toolchain_library` (0040138c `__SEH_prolog`, 004013c7 `__SEH_epilog`, 00bfe9f9
  `@__security_check_cookie@4`, 00401284 the MSVCR71 `exit` thunk); 8 genuine VC7.1 compiles of CRT source that byte-match the
  library object (WinMainCRTStartup, _onexit/atexit, the two RTC walkers, _setdefaultprecision, __ArrayUnwind, eh vector dtor
  iterator: kept `genuine`, listed in the oracle); 14 MSVCR71 import thunks landed as fake C++ (00401356, 004013de, 00bfe9bc,
  00bfea0e, 00bfea14, 00bfea1a, 00bfeae6, 00bfeaf2, 00bfeb1c, 00bfeb22, 00bfeb3a, 00bfed16, 00bfed40, 00bfed4c; several with
  wrong stand-in names) -> follow-up: re-grade as toolchain_library.
- (b) 110 unlanded catalog functions = link work, not reconstruction: 12 CRT sections (all 12 sit parked in the pending pool:
  _report_failure, __chkstk, __ftol2, __allmul, __aullrem, __allrem, __aullshr, __aulldiv, eh vector ctor iterator, __alldiv,
  __allshr, __aulldvrm) and 98 import thunks (68 MSVCR71, 13 imm32, 10 d3dx9_25, 3 version, d3d9, dinput8, winmm, wsock32).
- (c) 312 outside the catalog: 5 CRT functions (_check_managed_app, the type_info copy ctor picked by RTTI out of 149 identical
  copies, ??_Etype_info, ArrayUnwindFilter, ___security_init_cookie) and 307 thunks (kernel32 120, user32 75, MSVCR71 37, winmm
  22, wsock32 20, advapi32 9, ole32 7, gdi32 5, imm32 4, MSVCP71 3, shell32 2, oleaut32 2, d3dx9_25 1).
Excluded and logged: 152 ambiguous copies (type_info / comsupp shapes), 37 patterns found only in libraries retail does not link
(libc/libcp/debug: STL header code compiled in Fable files), 27 compiled `jmp [IAT]` tail jumps that are not thunks.
Grade `toolchain_library`: purity.py classifies a comments-only stub carrying `// Toolchain-library: <lib>(<member>) <symbol>
-- <n> bytes verified against Fable.exe <addr>`; verify_and_land.py never compiles or lands it and `--recheck-landed` re-proves
it with lib_verify.py (LIBRARY_MATCH; LIBRARY_UNVERIFIED without FABLE_EXE, never a pass); build_candidates.ps1 records
status=LIBRARY; the dashboard counts it as genuine (the bytes are the toolchain's; the build links them) and the asm-bake line
excludes it. Coordinator re-ran lib_verify.py on all four (MATCH), a wrong address (MISMATCH), and the no-exe path.
Dashboard: toolchain_library 0 -> 4; asm_bake 16 -> 13 (00401284 had no catalog entry: added, with its auto-re-candidates row
`MSVCR71.DLL::exit`); genuine catalog rows +4.
Vendor code scoped, not on disk: D3DX9 (DirectX SDK April 2005, d3dx9_25.dll: thunks only, no static d3dx9 code found); the era
Platform SDK import libs (thunks were named from the WinDDK libs); statically compiled vendor SOURCE outside this grade: zlib
1.2.1, libvorbis 1.0.1 (20030909), libogg, vorbisfile, LZO (compile from upstream source, as chunks 585-593 do); OpenAL loaded at
run time; no STLport or dxerr library traces (STLport is header-only here).
Open: rich_header.py labels prodid 0x0F "Linker" and 0x5F "Utc1310_CPP" but the objects' stamps say 0x0F MASM 7.10, 0x5F C,
0x60 C++ (not changed); name candidates from library symbols queued (prefix libsig); 307 thunks + 5 CRT functions are outside
the manifest (a boundary pass could add them).
