// DeleteObject import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: gdi32.lib(GDI32.dll) _DeleteObject@4 -- 6 bytes verified against Fable.exe 0x00CB5A20
//
// Retail 0x00CB5A20 is `jmp dword ptr [__imp__DeleteObject@4]` (FF 25 74 FC 43 01). The IAT slot 0x0143FC74 is
// GDI32.dll!DeleteObject in the retail import directory. The import library is the Platform SDK gdi32.lib (named
// from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `GDI32.dll` that defines _DeleteObject@4 is a short
// import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _DeleteObject@4 rather than __imp__DeleteObject@4. The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary).
// The build links gdi32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5a20 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
