// __security_check_cookie: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\secchk.obj) @__security_check_cookie@4 -- 14 bytes verified against Fable.exe 0x00BFE9F9
//
// Retail 0x00BFE9F9..0x00BFEA06 is the 14-byte COMDAT .text section of @__security_check_cookie@4 in
// secchk.obj (C, @comp.id 0x005F build 3077) of the VS .NET 2003 Vc7/lib/msvcrt.lib that retail links; the
// same object's report_failure section sits right before it at 0x00BFE9C8 (49 bytes). Both relocations are
// proven against retail: +2 DIR32 ___security_cookie -> 0x0139C8A8, which holds seccook.obj's initial value
// 0xBB40E64E, and +A REL32 _report_failure -> 0x00BFE9C8, whose bytes are secchk.obj's report_failure section.
// The lane's WinDDK 3790.1830 bufferoverflowu.lib secchk.obj is a different 22-byte body (it adds a
// `test ecx,0FFFF0000h` check), so the era library is the VS .NET 2003 one, not the DDK's.
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe9f9 -> MATCH (2/2 relocations proven);
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
