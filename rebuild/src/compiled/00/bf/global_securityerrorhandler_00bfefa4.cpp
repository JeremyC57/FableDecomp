// __security_error_handler import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(MSVCR71.dll) ___security_error_handler -- 6 bytes verified against Fable.exe 0x00BFEFA4
//
// Retail 0x00BFEFA4 is `jmp dword ptr [__imp____security_error_handler]` (FF 25 38 00 44 01). The IAT slot
// 0x01440038 is MSVCR71.dll!__security_error_handler in the retail import directory. The VS .NET 2003
// Vc7/lib/msvcrt.lib member `MSVCR71.dll` that defines ___security_error_handler is a short import record (CODE,
// NAME_NOPREFIX): the linker emits this thunk for it whenever a caller references ___security_error_handler rather
// than __imp____security_error_handler.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefa4 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
