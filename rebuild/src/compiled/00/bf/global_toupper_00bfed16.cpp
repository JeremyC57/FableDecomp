// toupper import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _toupper -- 6 bytes verified against Fable.exe 0x00BFED16
//
// Retail 0x00BFED16 is `jmp dword ptr [__imp__toupper]` (FF 25 DC 00 44 01). The IAT slot 0x014400DC is
// MSVCR71.dll!toupper in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines _toupper is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references _toupper rather than __imp__toupper. Re-graded 2026-10-30: the row was first landed
// as C++ (a `toupper_thunk` wrapper around the dllimport `toupper`) that compiled to the same six bytes; a linker
// thunk has no source, so that body was a stand-in, not a reconstruction.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed16 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
