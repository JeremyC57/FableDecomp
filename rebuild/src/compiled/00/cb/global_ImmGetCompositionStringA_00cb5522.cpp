// ImmGetCompositionStringA import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: imm32.lib(IMM32.dll) _ImmGetCompositionStringA@16 -- 6 bytes verified against Fable.exe 0x00CB5522
//
// Retail 0x00CB5522 is `jmp dword ptr [__imp__ImmGetCompositionStringA@16]` (FF 25 F0 FC 43 01). The IAT slot
// 0x0143FCF0 is IMM32.dll!ImmGetCompositionStringA in the retail import directory. The import library is the
// Platform SDK imm32.lib (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `IMM32.dll` that defines
// _ImmGetCompositionStringA@16 is a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for
// it whenever a caller references _ImmGetCompositionStringA@16 rather than __imp__ImmGetCompositionStringA@16.
// The build links imm32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5522 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
