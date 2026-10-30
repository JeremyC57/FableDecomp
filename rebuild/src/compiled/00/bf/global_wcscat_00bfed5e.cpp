// wcscat import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) _wcscat -- 6 bytes verified against Fable.exe 0x00BFED5E
//
// Retail 0x00BFED5E is `jmp dword ptr [__imp__wcscat]` (FF 25 A8 00 44 01). The IAT slot 0x014400A8 is
// MSVCR71.dll!wcscat in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll` that
// defines _wcscat is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it whenever a
// caller references _wcscat rather than __imp__wcscat.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfed5e -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
