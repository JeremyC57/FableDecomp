// report_failure: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\secchk.obj) _report_failure -- 49 bytes verified against Fable.exe 0x00BFE9C8
//
// Retail 0x00BFE9C8..0x00BFE9F8 is the whole 49-byte .text section of secchk.obj in the VS .NET 2003
// Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C (@comp.id 0x005F build 3077, a
// retail Rich header entry). 4 of its 4 relocations are proven against retail: +3:DIR32:$T19896=0x012A8378[ok:lib]
// +8:REL32:__SEH_prolog=0x0040138C[ok:lib] +15:REL32:___security_error_handler=0x00BFEFA4[ok]
// +2c:DIR32:__imp__ExitProcess@4=0x0143FD7C[ok]. report_failure is the static helper of secchk.c that
// @__security_check_cookie@4 (0x00BFE9F9, landed, same object) calls when the /GS cookie check fails. It was
// parked in the pending pool as an unauthored DIFFER (hand-written MASM or CRT source); vendor bytes are linked,
// not reconstructed (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe9c8 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
