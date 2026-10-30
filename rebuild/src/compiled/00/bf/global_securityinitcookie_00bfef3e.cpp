// __security_init_cookie: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\seccinit.obj) ___security_init_cookie -- 102 bytes verified against Fable.exe 0x00BFEF3E
//
// Retail 0x00BFEF3E..0x00BFEFA3 is the whole 102-byte .text section (COMDAT, section 2) of seccinit.obj in the VS
// .NET 2003 Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C (@comp.id 0x005F build
// 3077, a retail Rich header entry). 8 of its 8 relocations are proven against retail:
// +7:DIR32:___security_cookie=0x0139C8A8[ok:lib] +1d:DIR32:__imp__GetSystemTimeAsFileTime@4=0x0143FE9C[ok]
// +29:DIR32:__imp__GetCurrentProcessId@0=0x0143FEA0[ok] +31:DIR32:__imp__GetCurrentThreadId@0=0x0143FE98[ok]
// +39:DIR32:__imp__GetTickCount@0=0x0143FDFC[ok] +45:DIR32:__imp__QueryPerformanceCounter@4=0x0143FE00[ok]
// +53:DIR32:___security_cookie=0x0139C8A8[ok:lib] +5b:DIR32:___security_cookie=0x0139C8A8[ok:lib].
// __security_init_cookie seeds the /GS cookie ___security_cookie (0x0139C8A8) from the system time, process and
// thread ids, tick count and performance counter. Its only retail reference is the pointer at 0x01374104 inside
// the C++ initializer table __xc_a..__xc_z (0x01374000..0x01374C38) that _initterm walks at start-up.
// Byte-identical alternates (not linked): vs2003rtm/libc.lib(build\intel\st_obj\seccinit.obj),
// vs2003rtm/libcmt.lib(build\intel\mt_obj\seccinit.obj). The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary); vendor bytes are linked, not reconstructed (owner
// rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfef3e -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
