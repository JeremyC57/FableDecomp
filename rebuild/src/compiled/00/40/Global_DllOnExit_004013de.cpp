// __dllonexit import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ___dllonexit -- 6 bytes verified against Fable.exe 0x004013DE
//
// Retail 0x004013DE is `jmp dword ptr [__imp____dllonexit]` (FF 25 74 01 44 01). The IAT slot 0x01440174 is
// MSVCR71.dll!__dllonexit in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines ___dllonexit is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references ___dllonexit rather than __imp____dllonexit. Re-graded 2026-10-30: the row was
// first landed as C++ (a function calling through a function-pointer global (`__imp_free_01440174`) under the
// invented name `operator_delete`) that compiled to the same six bytes; a linker thunk has no source, so that body
// was a stand-in, not a reconstruction.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 004013de -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
