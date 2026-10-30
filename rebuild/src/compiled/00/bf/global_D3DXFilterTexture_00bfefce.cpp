// D3DXFilterTexture import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: d3dx9.lib(d3dx9_25.dll) _D3DXFilterTexture@16 -- 6 bytes verified against Fable.exe 0x00BFEFCE
//
// Retail 0x00BFEFCE is `jmp dword ptr [__imp__D3DXFilterTexture@16]` (FF 25 EC 05 44 01). The IAT slot 0x014405EC
// is d3dx9_25.dll!D3DXFilterTexture in the retail import directory. The import library is the DirectX SDK (April
// 2005) d3dx9.lib, which is not on the lane's disk; a thunk's bytes do not depend on it. _D3DXFilterTexture@16 is
// the __stdcall decoration of the d3dx9 SDK prototype (4 arguments), matching the argument pushes at the retail
// call site(s): the linker emits this thunk for it whenever a caller references _D3DXFilterTexture@16 rather than
// __imp__D3DXFilterTexture@16.
// The build links d3dx9.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefce -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
