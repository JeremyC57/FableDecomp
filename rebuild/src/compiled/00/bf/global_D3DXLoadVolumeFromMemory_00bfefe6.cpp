// D3DXLoadVolumeFromMemory import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: d3dx9.lib(d3dx9_25.dll) _D3DXLoadVolumeFromMemory@44 -- 6 bytes verified against Fable.exe 0x00BFEFE6
//
// Retail 0x00BFEFE6 is `jmp dword ptr [__imp__D3DXLoadVolumeFromMemory@44]` (FF 25 DC 05 44 01). The IAT slot
// 0x014405DC is d3dx9_25.dll!D3DXLoadVolumeFromMemory in the retail import directory. The import library is the
// DirectX SDK (April 2005) d3dx9.lib, which is not on the lane's disk; a thunk's bytes do not depend on it.
// _D3DXLoadVolumeFromMemory@44 is the __stdcall decoration of the d3dx9 SDK prototype (11 arguments: pDestVolume,
// pDestPalette, pDestBox, pSrcMemory, SrcFormat, SrcRowPitch, SrcSlicePitch, pSrcPalette, pSrcBox, Filter,
// ColorKey), matching the eleven pushes and the absent stack cleanup at the retail call site 0x00A6765B: the
// linker emits this thunk for it whenever a caller references _D3DXLoadVolumeFromMemory@44 rather than
// __imp__D3DXLoadVolumeFromMemory@44. The address was outside the function manifest until the 2026-10-30 boundary
// pass added it (agent_source boundary).
// The build links d3dx9.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfefe6 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
