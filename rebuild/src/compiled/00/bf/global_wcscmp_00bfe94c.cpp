// wcscmp import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _wcscmp -- 6 bytes verified against Fable.exe 0x00BFE94C
//
// Retail 0x00BFE94C is `jmp dword ptr [__imp__wcscmp]` (FF 25 68 01 44 01). The IAT slot 0x01440168 is
// MSVCR71.dll!wcscmp in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _wcscmp is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _wcscmp rather than __imp__wcscmp.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe94c -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
