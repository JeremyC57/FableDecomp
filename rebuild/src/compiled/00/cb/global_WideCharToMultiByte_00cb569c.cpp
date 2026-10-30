// WideCharToMultiByte import thunk: linker-synthesised from an import-library record, not Fable source (grade toolchain_library).
// Toolchain-library: kernel32.lib(KERNEL32.dll) _WideCharToMultiByte@32 -- 6 bytes verified against Fable.exe 0x00CB569C
//
// Retail 0x00CB569C is `jmp dword ptr [__imp__WideCharToMultiByte@32]` (FF 25 98 FD 43 01). The IAT slot
// 0x0143FD98 is KERNEL32.dll!WideCharToMultiByte in the retail import directory. The VS .NET 2003
// Vc7/lib/kernel32.lib member `KERNEL32.dll` that defines _WideCharToMultiByte@32 is a short import record (CODE,
// NAME_UNDECORATE): the linker emits this thunk for it whenever a caller references _WideCharToMultiByte@32 rather
// than __imp__WideCharToMultiByte@32. The address was outside the function manifest until the 2026-10-30 boundary
// pass added it (agent_source boundary).
// The build links kernel32.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00cb569c -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
