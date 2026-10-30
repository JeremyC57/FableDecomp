// GetFileVersionInfoSizeA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: version.lib(VERSION.dll) _GetFileVersionInfoSizeA@8 -- 6 bytes verified against Fable.exe 0x00CB558E
//
// Retail 0x00CB558E is `jmp dword ptr [__imp__GetFileVersionInfoSizeA@8]` (FF 25 44 04 44 01). The IAT slot
// 0x01440444 is VERSION.dll!GetFileVersionInfoSizeA in the retail import directory. The import library is the
// Platform SDK version.lib (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `VERSION.dll` that
// defines _GetFileVersionInfoSizeA@8 is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk
// for it whenever a caller references _GetFileVersionInfoSizeA@8 rather than __imp__GetFileVersionInfoSizeA@8.
// The build links version.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb558e -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
