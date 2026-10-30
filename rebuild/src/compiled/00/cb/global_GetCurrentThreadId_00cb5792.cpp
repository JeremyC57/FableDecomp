// GetCurrentThreadId import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: kernel32.lib(KERNEL32.dll) _GetCurrentThreadId@0 -- 6 bytes verified against Fable.exe 0x00CB5792
//
// Retail 0x00CB5792 is `jmp dword ptr [__imp__GetCurrentThreadId@0]` (FF 25 98 FE 43 01). The IAT slot 0x0143FE98
// is KERNEL32.dll!GetCurrentThreadId in the retail import directory. The VS .NET 2003 Vc7/lib/kernel32.lib member
// `KERNEL32.dll` that defines _GetCurrentThreadId@0 is a short import record (CODE, NAME_UNDECORATE): the linker
// emits this thunk for it whenever a caller references _GetCurrentThreadId@0 rather than
// __imp__GetCurrentThreadId@0. The address was outside the function manifest until the 2026-10-30 boundary pass
// added it (agent_source boundary).
// The build links kernel32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb5792 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
