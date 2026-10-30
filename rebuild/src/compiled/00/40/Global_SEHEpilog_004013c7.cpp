// __SEH_epilog: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\sehprolg.obj) __SEH_epilog -- 17 bytes verified against Fable.exe 0x004013C7
//
// Retail 0x004013C7..0x004013D7 is the tail (offset 0x3B, 17 bytes) of the 76-byte .text section of
// sehprolg.obj in the VS .NET 2003 Vc7/lib/msvcrt.lib that retail links; __SEH_prolog is the same section
// at 0x0040138C. The epilog itself has no relocation; the section's one fixup (+1 DIR32 __except_handler3,
// in the prolog) resolves to 0x004013D8, the linker's thunk for MSVCR71.dll!_except_handler3.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 004013c7 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
