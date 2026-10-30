// DirectInput8Create import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: dinput8.lib(DINPUT8.dll) _DirectInput8Create@20 -- 6 bytes verified against Fable.exe 0x00BFEFF8
//
// Retail 0x00BFEFF8 is `jmp dword ptr [__imp__DirectInput8Create@20]` (FF 25 3C FC 43 01). The IAT slot 0x0143FC3C
// is DINPUT8.dll!DirectInput8Create in the retail import directory. The import library is the Platform SDK
// dinput8.lib (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `DINPUT8.dll` that defines
// _DirectInput8Create@20 is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it
// whenever a caller references _DirectInput8Create@20 rather than __imp__DirectInput8Create@20.
// The build links dinput8.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfeff8 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
