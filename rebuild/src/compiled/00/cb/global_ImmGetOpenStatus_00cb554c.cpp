// ImmGetOpenStatus import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: imm32.lib(IMM32.dll) _ImmGetOpenStatus@4 -- 6 bytes verified against Fable.exe 0x00CB554C
//
// Retail 0x00CB554C is `jmp dword ptr [__imp__ImmGetOpenStatus@4]` (FF 25 CC FC 43 01). The IAT slot 0x0143FCCC is
// IMM32.dll!ImmGetOpenStatus in the retail import directory. The import library is the Platform SDK imm32.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `IMM32.dll` that defines _ImmGetOpenStatus@4 is
// a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _ImmGetOpenStatus@4 rather than __imp__ImmGetOpenStatus@4.
// The build links imm32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb554c -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
