// RegCreateKeyExA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: advapi32.lib(ADVAPI32.dll) _RegCreateKeyExA@36 -- 6 bytes verified against Fable.exe 0x00CB5A68
//
// Retail 0x00CB5A68 is `jmp dword ptr [__imp__RegCreateKeyExA@36]` (FF 25 F0 FB 43 01). The IAT slot 0x0143FBF0 is
// ADVAPI32.dll!RegCreateKeyExA in the retail import directory. The import library is the Platform SDK advapi32.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `ADVAPI32.dll` that defines _RegCreateKeyExA@36
// is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller
// references _RegCreateKeyExA@36 rather than __imp__RegCreateKeyExA@36. The address was outside the function
// manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links advapi32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5a68 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
