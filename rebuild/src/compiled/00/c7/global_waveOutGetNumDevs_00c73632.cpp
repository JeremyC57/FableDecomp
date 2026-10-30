// waveOutGetNumDevs import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: winmm.lib(WINMM.dll) _waveOutGetNumDevs@0 -- 6 bytes verified against Fable.exe 0x00C73632
//
// Retail 0x00C73632 is `jmp dword ptr [__imp__waveOutGetNumDevs@0]` (FF 25 98 04 44 01). The IAT slot 0x01440498
// is WINMM.dll!waveOutGetNumDevs in the retail import directory. The import library is the Platform SDK winmm.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `WINMM.dll` that defines _waveOutGetNumDevs@0
// is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller
// references _waveOutGetNumDevs@0 rather than __imp__waveOutGetNumDevs@0. The address was outside the function
// manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links winmm.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00c73632 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
