// ImmSetOpenStatus import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: imm32.lib(IMM32.dll) _ImmSetOpenStatus@8 -- 6 bytes verified against Fable.exe 0x00CB5576
//
// Retail 0x00CB5576 is `jmp dword ptr [__imp__ImmSetOpenStatus@8]` (FF 25 E8 FC 43 01). The IAT slot 0x0143FCE8 is
// IMM32.dll!ImmSetOpenStatus in the retail import directory. The import library is the Platform SDK imm32.lib
// (named from the WinDDK 3790.1830 lib/wnet/i386 copy); its member `IMM32.dll` that defines _ImmSetOpenStatus@8 is
// a short import record (CODE, NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references
// _ImmSetOpenStatus@8 rather than __imp__ImmSetOpenStatus@8. The address was outside the function manifest until
// the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links imm32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5576 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
