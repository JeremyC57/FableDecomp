// Direct3DCreate9 import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: d3d9.lib(d3d9.dll) _Direct3DCreate9@4 -- 6 bytes verified against Fable.exe 0x00BFEFB0
//
// Retail 0x00BFEFB0 is `jmp dword ptr [__imp__Direct3DCreate9@4]` (FF 25 A0 05 44 01). The IAT slot 0x014405A0 is
// d3d9.dll!Direct3DCreate9 in the retail import directory. The import library is the Platform SDK d3d9.lib (named
// from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `d3d9.dll` that defines _Direct3DCreate9@4 is a short
// import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _Direct3DCreate9@4 rather than __imp__Direct3DCreate9@4.
// The build links d3d9.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefb0 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
