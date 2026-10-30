// _wchdir import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __wchdir -- 6 bytes verified against Fable.exe 0x00BFEAFE
//
// Retail 0x00BFEAFE is `jmp dword ptr [__imp___wchdir]` (FF 25 34 01 44 01). The IAT slot 0x01440134 is
// MSVCR71.dll!_wchdir in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __wchdir is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __wchdir rather than __imp___wchdir.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeafe -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
