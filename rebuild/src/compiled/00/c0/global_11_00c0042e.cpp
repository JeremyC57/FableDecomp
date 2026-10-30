// #11 import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: wsock32.lib(WSOCK32.dll) _inet_ntoa@4 -- 6 bytes verified against Fable.exe 0x00C0042E
//
// Retail 0x00C0042E is `jmp dword ptr [__imp__inet_ntoa@4]` (FF 25 28 05 44 01). The IAT slot 0x01440528 is
// WSOCK32.dll!#11 in the retail import directory. The import library is the Platform SDK wsock32.lib (named from
// the WinDDK 3790.1830 lib/wnet/i386 copy); its member `WSOCK32.dll` that defines _inet_ntoa@4 is a short import
// record (CODE, ORDINAL): the linker emits this thunk for it whenever a caller references _inet_ntoa@4 rather than
// __imp__inet_ntoa@4. The address was outside the function manifest until the 2026-10-30 boundary pass added it
// (agent_source boundary).
// The build links wsock32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00c0042e -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
