// check_managed_app: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\crtexew.obj) _check_managed_app -- 86 bytes verified against Fable.exe 0x00401011
//
// Retail 0x00401011..0x00401066 is the whole 86-byte .text section (COMDAT, section 2) of crtexew.obj in the VS
// .NET 2003 Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C (@comp.id 0x005F build
// 3077, a retail Rich header entry). 1 of its 1 relocations are proven against retail:
// +4:DIR32:__imp__GetModuleHandleA@4=0x0143FE94[ok]. check_managed_app is the static helper of crtexe.c that reads
// the image's own PE header (GetModuleHandleA(NULL), the CLR runtime-header data directory) to tell a managed
// image from a native one. No retail code or data references 0x00401011: _WinMainCRTStartup (0x00401067, same
// object) carries its own inlined copy of the check, and the COMDAT stayed in the image unreferenced.
// Byte-identical alternates (not linked): vs2003rtm/libc.lib(build\intel\st_obj\crt0.obj),
// vs2003rtm/libc.lib(build\intel\st_obj\wcrt0.obj), vs2003rtm/libc.lib(build\intel\st_obj\wincrt0.obj),
// vs2003rtm/libc.lib(build\intel\st_obj\wwincrt0.obj), vs2003rtm/libcmt.lib(build\intel\mt_obj\crt0.obj),
// vs2003rtm/libcmt.lib(build\intel\mt_obj\wcrt0.obj) (+5). The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary); vendor bytes are linked, not reconstructed (owner
// rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00401011 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
