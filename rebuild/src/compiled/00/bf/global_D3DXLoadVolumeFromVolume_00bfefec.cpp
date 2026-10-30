// D3DXLoadVolumeFromVolume import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: d3dx9.lib(d3dx9_25.dll) _D3DXLoadVolumeFromVolume@32 -- 6 bytes verified against Fable.exe 0x00BFEFEC
//
// Retail 0x00BFEFEC is `jmp dword ptr [__imp__D3DXLoadVolumeFromVolume@32]` (FF 25 D8 05 44 01). The IAT slot
// 0x014405D8 is d3dx9_25.dll!D3DXLoadVolumeFromVolume in the retail import directory. The import library is the
// DirectX SDK (April 2005) d3dx9.lib, which is not on the lane's disk; a thunk's bytes do not depend on it.
// _D3DXLoadVolumeFromVolume@32 is the __stdcall decoration of the d3dx9 SDK prototype (8 arguments), matching the
// argument pushes at the retail call site(s): the linker emits this thunk for it whenever a caller references
// _D3DXLoadVolumeFromVolume@32 rather than __imp__D3DXLoadVolumeFromVolume@32.
// The build links d3dx9.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefec -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
