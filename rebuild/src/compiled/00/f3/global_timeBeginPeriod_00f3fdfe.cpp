// timeBeginPeriod import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: winmm.lib(WINMM.dll) _timeBeginPeriod@4 -- 6 bytes verified against Fable.exe 0x00F3FDFE
//
// Retail 0x00F3FDFE is `jmp dword ptr [__imp__timeBeginPeriod@4]` (FF 25 AC 04 44 01). The IAT slot 0x014404AC is
// WINMM.dll!timeBeginPeriod in the retail import directory. The import library is the Platform SDK winmm.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `WINMM.dll` that defines _timeBeginPeriod@4 is
// a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _timeBeginPeriod@4 rather than __imp__timeBeginPeriod@4. The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary).
// The build links winmm.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fdfe -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
