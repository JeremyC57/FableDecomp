// RegSetValueExW import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: advapi32.lib(ADVAPI32.dll) _RegSetValueExW@24 -- 6 bytes verified against Fable.exe 0x00CB5A4A
//
// Retail 0x00CB5A4A is `jmp dword ptr [__imp__RegSetValueExW@24]` (FF 25 E8 FB 43 01). The IAT slot 0x0143FBE8 is
// ADVAPI32.dll!RegSetValueExW in the retail import directory. The import library is the Platform SDK advapi32.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `ADVAPI32.dll` that defines _RegSetValueExW@24
// is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller
// references _RegSetValueExW@24 rather than __imp__RegSetValueExW@24. The address was outside the function
// manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links advapi32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5a4a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
