// operator delete import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ??3@YAXPAX@Z -- 6 bytes verified against Fable.exe 0x00BFE9BC
//
// Retail 0x00BFE9BC is `jmp dword ptr [__imp_??3@YAXPAX@Z]` (FF 25 64 01 44 01). The IAT slot 0x01440164 is
// MSVCR71.dll!??3@YAXPAX@Z in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ??3@YAXPAX@Z is a short import record (CODE, NAME): the linker emits this thunk for
// it whenever a caller references ??3@YAXPAX@Z rather than __imp_??3@YAXPAX@Z. Re-graded 2026-10-30: the row was
// first landed as C++ (a function calling through a function-pointer global (`__imp_free_01440164`) under the
// invented name `operator_delete`) that compiled to the same six bytes; a linker thunk has no source, so that body
// was a stand-in, not a reconstruction.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe9bc -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
