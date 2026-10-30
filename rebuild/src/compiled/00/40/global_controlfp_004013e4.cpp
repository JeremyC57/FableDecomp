// _controlfp import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __controlfp -- 6 bytes verified against Fable.exe 0x004013E4
//
// Retail 0x004013E4 is `jmp dword ptr [__imp___controlfp]` (FF 25 6C 01 44 01). The IAT slot 0x0144016C is
// MSVCR71.dll!_controlfp in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __controlfp is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __controlfp rather than __imp___controlfp.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 004013e4 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
