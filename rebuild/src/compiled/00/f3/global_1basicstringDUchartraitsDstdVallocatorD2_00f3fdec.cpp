// ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: msvcprt.lib(MSVCP71.dll) ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ -- 6 bytes verified against Fable.exe 0x00F3FDEC
//
// Retail 0x00F3FDEC is `jmp dword ptr
// [__imp_??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ]` (FF 25 9C FF 43 01). The IAT
// slot 0x0143FF9C is MSVCP71.dll!??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ in the
// retail import directory. The VS .NET 2003 Vc7/lib/msvcprt.lib member `MSVCP71.dll` that defines
// ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ is a short import record (CODE, NAME):
// the linker emits this thunk for it whenever a caller references
// ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ rather than
// __imp_??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ. The address was outside the
// function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links msvcprt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fdec -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
