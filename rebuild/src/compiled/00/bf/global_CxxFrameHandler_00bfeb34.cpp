// __CxxFrameHandler import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ___CxxFrameHandler -- 6 bytes verified against Fable.exe 0x00BFEB34
//
// Retail 0x00BFEB34 is `jmp dword ptr [__imp____CxxFrameHandler]` (FF 25 10 01 44 01). The IAT slot 0x01440110 is
// MSVCR71.dll!__CxxFrameHandler in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ___CxxFrameHandler is a short import record (CODE, NAME_NOPREFIX): the linker emits
// this thunk for it whenever a caller references ___CxxFrameHandler rather than __imp____CxxFrameHandler.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeb34 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
