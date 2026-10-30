// timeSetEvent import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: winmm.lib(WINMM.dll) _timeSetEvent@20 -- 6 bytes verified against Fable.exe 0x00F3FE0A
//
// Retail 0x00F3FE0A is `jmp dword ptr [__imp__timeSetEvent@20]` (FF 25 A4 04 44 01). The IAT slot 0x014404A4 is
// WINMM.dll!timeSetEvent in the retail import directory. The import library is the Platform SDK winmm.lib (named
// from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `WINMM.dll` that defines _timeSetEvent@20 is a short
// import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _timeSetEvent@20 rather than __imp__timeSetEvent@20. This is the one linker thunk for _timeSetEvent@20: it sits
// in the run of ten linker thunks 0x00F3FDE6..0x00F3FE21 (three MSVCP71 basic_string members, timeKillEvent,
// timeBeginPeriod, timeEndPeriod, timeSetEvent, _strupr, _strlwr, _strrev). The six identical bytes at 0x00CB0C10
// are a compiled function, DirectShow wxutil.cpp CompatibleTimeSetEvent built with WINVER < 0x0501 (CC-padded and
// 16-byte aligned among compiled code, called by CBaseRenderer::SendEndOfStream 0x00CA6F90); see NAME_FIXES of the
// boundary pass. The address was outside the function manifest until the 2026-10-30 boundary pass added it
// (agent_source boundary).
// The build links winmm.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fe0a -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
