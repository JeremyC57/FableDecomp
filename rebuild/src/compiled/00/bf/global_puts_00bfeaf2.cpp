// puts import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _puts -- 6 bytes verified against Fable.exe 0x00BFEAF2
//
// Retail 0x00BFEAF2 is `jmp dword ptr [__imp__puts]` (FF 25 3C 01 44 01). The IAT slot 0x0144013C is
// MSVCR71.dll!puts in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _puts is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _puts rather than __imp__puts. Re-graded 2026-10-30: the row was first landed as C++ (an
// `extern "C" puts` calling a dllimport stand-in `real_puts_target`) that compiled to the same six bytes; a linker
// thunk has no source, so that body was a stand-in, not a reconstruction.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeaf2 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
