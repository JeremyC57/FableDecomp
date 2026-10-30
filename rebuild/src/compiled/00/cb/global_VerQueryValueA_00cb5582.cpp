// VerQueryValueA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: version.lib(VERSION.dll) _VerQueryValueA@16 -- 6 bytes verified against Fable.exe 0x00CB5582
//
// Retail 0x00CB5582 is `jmp dword ptr [__imp__VerQueryValueA@16]` (FF 25 4C 04 44 01). The IAT slot 0x0144044C is
// VERSION.dll!VerQueryValueA in the retail import directory. The import library is the Platform SDK version.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `VERSION.dll` that defines _VerQueryValueA@16
// is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller
// references _VerQueryValueA@16 rather than __imp__VerQueryValueA@16.
// The build links version.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5582 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
