// operator new import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ??2@YAPAXI@Z -- 6 bytes verified against Fable.exe 0x00BFEA1A
//
// Retail 0x00BFEA1A is `jmp dword ptr [__imp_??2@YAPAXI@Z]` (FF 25 50 01 44 01). The IAT slot 0x01440150 is
// MSVCR71.dll!??2@YAPAXI@Z in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ??2@YAPAXI@Z is a short import record (CODE, NAME): the linker emits this thunk for
// it whenever a caller references ??2@YAPAXI@Z rather than __imp_??2@YAPAXI@Z. Re-graded 2026-10-30: the row was
// first landed as C++ (an `extern "C" operator_new` calling through the slot address cast to a function pointer)
// that compiled to the same six bytes; a linker thunk has no source, so that body was a stand-in, not a
// reconstruction.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfea1a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
