// D3DXVec3Hermite import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: d3dx9.lib(d3dx9_25.dll) _D3DXVec3Hermite@24 -- 6 bytes verified against Fable.exe 0x00BFEFDA
//
// Retail 0x00BFEFDA is `jmp dword ptr [__imp__D3DXVec3Hermite@24]` (FF 25 E4 05 44 01). The IAT slot 0x014405E4 is
// d3dx9_25.dll!D3DXVec3Hermite in the retail import directory. The import library is the DirectX SDK (April 2005)
// d3dx9.lib, which is not on the lane's disk; a thunk's bytes do not depend on it. _D3DXVec3Hermite@24 is the
// __stdcall decoration of the d3dx9 SDK prototype (6 arguments), matching the argument pushes at the retail call
// site(s): the linker emits this thunk for it whenever a caller references _D3DXVec3Hermite@24 rather than
// __imp__D3DXVec3Hermite@24.
// The build links d3dx9.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefda -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
