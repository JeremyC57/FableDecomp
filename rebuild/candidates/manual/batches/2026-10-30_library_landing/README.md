# Vendor-library landing pass (2026-10-30): 128 rows now grade `toolchain_library`; 125 callers respelled to library names

Follows the signature pass (`../2026-10-30_library_signatures/`). Brief BRIEF.md; ROWS.tsv (128 library rows + 131 caller
rows with lib_verify / recheck results); BINDINGS.tsv (69 invented spellings retired, 9 library spellings added);
NAME_FIXES.tsv (35); RENAMES.tsv (none needed); REMOVED.tsv (13 parked copies deleted from pending/); OUTSIDE_CATALOG.tsv
(the 312 oracle rows outside the manifest with proposed names and paths, for a boundary pass); LESSONS.md; logs/.
Re-graded (14): the landed MSVCR71 import thunks that were fake C++ are comments-only stubs; catalog names fixed
(00401356 `MSVCR71.DLL::_initterm`; the C++ operator thunks keep the `MSVCR71.DLL::operator_new` style because
reloc_identity's CRT-claim check rejects the raw `??2` export spelling). Landed (110): the 12 parked CRT sections
(_report_failure, __chkstk, __ftol2, __allmul, __aullrem, __allrem, __aullshr, __aulldiv, eh vector ctor iterator, __alldiv,
__allshr, __aulldvrm) and the 98 catalogued import thunks (68 MSVCR71, 13 imm32, 10 d3dx9_25, 3 version, d3d9, dinput8,
winmm, wsock32), each `global_<export>_<addr>.cpp` with a catalog entry and an auto-re row; d3dx9.lib is not on disk, so
lib_verify.py proves those thunks from the import directory (IAT slot = d3dx9_25.dll!<export>). All 128: lib_verify.py MATCH
and vland --recheck-landed LIBRARY_MATCH. The 8 genuine CRT-source compiles carry a `// Library: msvcrt.lib(<member>)
<symbol>` provenance line and are counted as `library_source` (8) on the dashboard.
Callers: 125 rows + fable_boot.h / fable_user_profile.h respelled to the library names (69 `free` stand-ins -> `extern "C"
free`; 25 `push ecx; call 0x00BFEB1C; pop ecx; ret` copies of an operator_new stand-in -> `::operator delete`; rand, sprintf,
_stricmp (two rows had strcmp), wcscmp, the _wchdir/_wmkdir/_getdrive/_chdrive/_wsplitpath/_wmakepath family, _onexit,
timeSetEvent, two D3DX functions, terminate; invented `Finalize` / `sub_bfea70` callees were `__ftol2` after a float-returning
call). 0x00689370 is the float `floor`, not `ceil` (its thunk's IAT slot is MSVCR71!floor; the float ceil is 0x0057A2D0): the
row and 4 callers spell `?floor@@YAMM@Z`. Agent recheck 139/139 PASS under catalog flags; coordinator spot recheck 32/32 PASS
(coordinator_spot_recheck.log) and 12 random lib_verify proofs MATCH; multi-check 0.
Dashboard: toolchain_library 4 -> 128, asm_bake 13, library_source 8, genuine known landed +110 (51,910), matched bytes +1,747.
The "retail function bytes reproduced" figure moved 38.6% -> 49.8% because landing the 149-byte __aulldvrm at 0x00F3FD50
removed a 2.95 MB gap in the unlanded-bytes estimator (next known start 0x01210A00); the estimator now caps each unlanded
gap at 64 KB (tools/landed_metrics.py, 1 gap capped), so the figure is stable from here.
Proved but not landed: 312 outside-manifest rows (OUTSIDE_CATALOG.tsv); 0x00785B40 links __aullrem where retail calls
__allrem (its modulo should be signed __int64; the file has a retail_asm block, NAME_FIXES); `__ArrayUnwind` / `??_M` keep
extern "C" spellings (VC7.1 rejects __thiscall there, C4234); `__alloca_probe` is a legitimate second label at 0x00BFEA30.
NAME_FIXES also: the 25 delete rows are CCountedPointer<T>::DeleteData (18 proven by callers); 0x00689370's catalog/file
name should become floor; 0x00F3FBE0 is a funclet inside 0x00F3FB96; 0x00401490 is a wcscmp equality test, not
PeekWaterHasType.
