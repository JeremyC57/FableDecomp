// __SEH_prolog: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\sehprolg.obj) __SEH_prolog -- 59 bytes verified against Fable.exe 0x0040138C
//
// Retail 0x0040138C..0x004013C6 is the first 59 bytes of the single .text section of sehprolg.obj in the
// VS .NET 2003 Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is MASM output
// (@comp.id 0x000F build 3077, the retail Rich header's twelve 0x000F/3077 objects); the linker copies its
// 76-byte section whole, so __SEH_epilog follows at 0x004013C7. The one relocation (+1 DIR32
// __except_handler3) resolves to 0x004013D8, the linker's thunk for MSVCR71.dll!_except_handler3.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 0040138c -> MATCH (relocation proven);
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
