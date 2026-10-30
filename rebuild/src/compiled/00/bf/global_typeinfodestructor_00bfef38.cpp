// type_info::~type_info import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ??1type_info@@UAE@XZ -- 6 bytes verified against Fable.exe 0x00BFEF38
//
// Retail 0x00BFEF38 is `jmp dword ptr [__imp_??1type_info@@UAE@XZ]` (FF 25 3C 00 44 01). The IAT slot 0x0144003C
// is MSVCR71.dll!??1type_info@@UAE@XZ in the retail import directory. The VS .NET 2003 Vc7/lib/msvcrt.lib member
// `MSVCR71.dll` that defines ??1type_info@@UAE@XZ is a short import record (CODE, NAME): the linker emits this
// thunk for it whenever a caller references ??1type_info@@UAE@XZ rather than __imp_??1type_info@@UAE@XZ.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfef38 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
