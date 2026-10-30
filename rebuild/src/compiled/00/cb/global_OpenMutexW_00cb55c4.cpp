// OpenMutexW import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: kernel32.lib(KERNEL32.dll) _OpenMutexW@12 -- 6 bytes verified against Fable.exe 0x00CB55C4
//
// Retail 0x00CB55C4 is `jmp dword ptr [__imp__OpenMutexW@12]` (FF 25 24 FE 43 01). The IAT slot 0x0143FE24 is
// KERNEL32.dll!OpenMutexW in the retail import directory. The VS .NET 2003 Vc7/lib/kernel32.lib member
// `KERNEL32.dll` that defines _OpenMutexW@12 is a short import record (CODE, NAME_UNDECORATE): the linker emits
// this thunk for it whenever a caller references _OpenMutexW@12 rather than __imp__OpenMutexW@12. The address was
// outside the function manifest until the 2026-10-30 boundary pass added it (agent_source boundary).
// The build links kernel32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb55c4 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
