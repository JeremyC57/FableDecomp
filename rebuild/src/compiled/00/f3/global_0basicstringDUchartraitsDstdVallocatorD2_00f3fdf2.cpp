// ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcprt.lib(MSVCP71.dll) ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z -- 6 bytes verified against Fable.exe 0x00F3FDF2
//
// Retail 0x00F3FDF2 is `jmp dword ptr
// [__imp_??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z]` (FF 25 98 FF 43 01). The IAT
// slot 0x0143FF98 is MSVCP71.dll!??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z in the
// retail import directory. The VS .NET 2003 Vc7/lib/msvcprt.lib member `MSVCP71.dll` that defines
// ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z is a short import record (CODE, NAME):
// the linker emits this thunk for it whenever a caller references
// ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z rather than
// __imp_??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z. The address was outside the
// function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcprt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fdf2 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
