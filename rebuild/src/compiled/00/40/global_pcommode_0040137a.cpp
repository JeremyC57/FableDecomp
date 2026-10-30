// __p__commode import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ___p__commode -- 6 bytes verified against Fable.exe 0x0040137A
//
// Retail 0x0040137A is `jmp dword ptr [__imp____p__commode]` (FF 25 84 01 44 01). The IAT slot 0x01440184 is
// MSVCR71.dll!__p__commode in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ___p__commode is a short import record (CODE, NAME_NOPREFIX): the linker emits this
// thunk for it whenever a caller references ___p__commode rather than __imp____p__commode. The address was outside
// the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 0040137a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
