// _amsg_exit import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) __amsg_exit -- 6 bytes verified against Fable.exe 0x0040128A
//
// Retail 0x0040128A is `jmp dword ptr [__imp___amsg_exit]` (FF 25 98 01 44 01). The IAT slot 0x01440198 is
// MSVCR71.dll!_amsg_exit in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member `MSVCR71.dll`
// that defines __amsg_exit is a short import record (CODE, NAME_NOPREFIX): the linker emits this thunk for it
// whenever a caller references __amsg_exit rather than __imp___amsg_exit.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 0040128a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
