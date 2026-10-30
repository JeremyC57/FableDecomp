// CoInitialize import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: ole32.lib(ole32.dll) _CoInitialize@4 -- 6 bytes verified against Fable.exe 0x00CB5A7A
//
// Retail 0x00CB5A7A is `jmp dword ptr [__imp__CoInitialize@4]` (FF 25 44 06 44 01). The IAT slot 0x01440644 is
// ole32.dll!CoInitialize in the retail import directory. The import library is the Platform SDK ole32.lib (named
// from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `ole32.dll` that defines _CoInitialize@4 is a short
// import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _CoInitialize@4 rather than __imp__CoInitialize@4. The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary).
// The build links ole32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5a7a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
