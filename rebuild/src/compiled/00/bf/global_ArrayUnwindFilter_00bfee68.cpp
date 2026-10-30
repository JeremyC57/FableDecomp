// ArrayUnwindFilter: VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\ehvecdtr.obj) ?ArrayUnwindFilter@@YAHPAU_EXCEPTION_POINTERS@@@Z -- 18 bytes verified against Fable.exe 0x00BFEE68
//
// Retail 0x00BFEE68..0x00BFEE79 is the whole 18-byte .text section (COMDAT, section 2) of ehvecdtr.obj in the VS
// .NET 2003 Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C++ (@comp.id 0x0060
// build 3077, a retail Rich header entry). 1 of its 1 relocations are proven against retail:
// +e:REL32:?terminate@@YAXXZ=0x00BFEFAA[ok]. ArrayUnwindFilter is the __except filter of ehvecdtr.cpp (it calls
// terminate for a C++ exception thrown while an array is being unwound). No retail code references 0x00BFEE68:
// __ArrayUnwind (0x00BFEE7A, landed, same object) carries the filter inline in its own __except funclet, which
// calls terminate at 0x00BFEED2. Byte-identical alternates (not linked): ddk3790/libc.lib(obj\i386\ehvecdtr.obj),
// ddk3790/libcd.lib(obj\i386\ehvecdtr.obj), ddk3790/libcmt.lib(obj\i386\ehvecdtr.obj),
// ddk3790/libcmtd.lib(obj\i386\ehvecdtr.obj), ddk3790/msvcrt.lib(obj\i386\ehvecdtr.obj),
// ddk3790/msvcrtd.lib(obj\i386\ehvecdtr.obj) (+2). The address was outside the function manifest until the
// 2026-10-30 boundary pass added it (agent_source boundary); vendor bytes are linked, not reconstructed (owner
// rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfee68 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
