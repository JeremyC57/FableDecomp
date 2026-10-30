// _wgetcwd import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __wgetcwd -- 6 bytes verified against Fable.exe 0x00BFED40
//
// Retail 0x00BFED40 is `jmp dword ptr [__imp___wgetcwd]` (FF 25 C0 00 44 01). The IAT slot 0x014400C0 is
// MSVCR71.dll!_wgetcwd in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __wgetcwd is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __wgetcwd rather than __imp___wgetcwd. Re-graded 2026-10-30: the row was first
// landed as C++ (an `extern "C" _wgetcwd` calling through a defined `__imp__wgetcwd` pointer to an invented
// `_wgetcwd_impl`) that compiled to the same six bytes; a linker thunk has no source, so that body was a stand-in,
// not a reconstruction.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed40 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
