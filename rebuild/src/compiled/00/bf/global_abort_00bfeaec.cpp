// abort import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _abort -- 6 bytes verified against Fable.exe 0x00BFEAEC
//
// Retail 0x00BFEAEC is `jmp dword ptr [__imp__abort]` (FF 25 40 01 44 01). The IAT slot 0x01440140 is
// MSVCR71.dll!abort in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _abort is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _abort rather than __imp__abort.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeaec -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
