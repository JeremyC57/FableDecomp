// `eh vector constructor iterator': VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\ehvecctr.obj) ??_L@YGXPAXIHP6EX0@Z1@Z -- 98 bytes verified against Fable.exe 0x00F3FB96
//
// Retail 0x00F3FB96..0x00F3FBF7 is the whole 98-byte .text section of ehvecctr.obj in the VS .NET 2003
// Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C++ (@comp.id 0x0060 build 3077, a
// retail Rich header entry). 4 of its 4 relocations are proven against retail: +3:DIR32:$T327=0x01301CF8[ok:lib]
// +8:REL32:__SEH_prolog=0x0040138C[ok:lib] +43:REL32:__SEH_epilog=0x004013C7[ok:lib]
// +5d:REL32:?__ArrayUnwind@@YGXPAXIHP6EX0@Z@Z=0x00BFEE7A[ok:lib]. The 98-byte section holds the iterator and its
// 24-byte __finally funclet (0x00F3FBE0..0x00F3FBF7, which calls __ArrayUnwind); the manifest row at 0x00F3FBE0
// named CFrameData::ReleaseTexture is that funclet, not a function. VC7.1 emits a call to ??_L for `new T[n]` of a
// class with a constructor and destructor. It was parked in the pending pool as an unauthored DIFFER (hand-written
// MASM or CRT source); vendor bytes are linked, not reconstructed (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00f3fb96 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
