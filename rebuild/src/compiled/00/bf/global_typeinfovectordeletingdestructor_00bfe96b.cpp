// type_info::`vector deleting destructor': VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\ti_inst.obj) ??_Etype_info@@UAEPAXI@Z -- 75 bytes verified against Fable.exe 0x00BFE96B
//
// Retail 0x00BFE96B..0x00BFE9B5 is the whole 75-byte .text section (COMDAT, section 11) of ti_inst.obj in the VS
// .NET 2003 Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C++ (@comp.id 0x0060
// build 3077, a retail Rich header entry). 5 of its 5 relocations are proven against retail:
// +f:DIR32:??1type_info@@UAE@XZ=0x00BFEF38[ok] +1c:REL32:??_M@YGXPAXIHP6EX0@Z@Z=0x00BFEED8[ok:lib]
// +27:REL32:??3@YAXPAX@Z=0x00BFE9BC[ok] +34:REL32:??1type_info@@UAE@XZ=0x00BFEF38[ok]
// +3f:REL32:??3@YAXPAX@Z=0x00BFE9BC[ok]. The vector deleting destructor is slot 0 of type_info's vftable
// 0x012A8370 (the only retail reference); it calls ~type_info through the MSVCR71 thunk 0x00BFEF38, the `eh vector
// destructor iterator' 0x00BFEED8 and operator delete 0x00BFE9BC. The address was outside the function manifest
// until the 2026-10-30 boundary pass added it (agent_source boundary); vendor bytes are linked, not reconstructed
// (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe96b -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
