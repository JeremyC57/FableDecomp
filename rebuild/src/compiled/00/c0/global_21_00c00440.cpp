// #21 import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: wsock32.lib(WSOCK32.dll) _setsockopt@20 -- 6 bytes verified against Fable.exe 0x00C00440
//
// Retail 0x00C00440 is `jmp dword ptr [__imp__setsockopt@20]` (FF 25 34 05 44 01). The IAT slot 0x01440534 is
// WSOCK32.dll!#21 in the retail import directory. The import library is the Platform SDK wsock32.lib (named from
// the WinDDK 3790.1830 lib/wnet/i386 copy); its member `WSOCK32.dll` that defines _setsockopt@20 is a short import
// record (CODE, ORDINAL): the linker emits this thunk for it whenever a caller references _setsockopt@20 rather
// than __imp__setsockopt@20. The address was outside the function manifest until the 2026-10-30 boundary pass
// added it (agent_source boundary).
// The build links wsock32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00c00440 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
