// type_info::type_info (copy constructor): VC7.1 CRT library code, not Fable source (grade toolchain_library).
// Toolchain-library: msvcrt.lib(build\intel\dll_obj\ti_inst.obj) ??0type_info@@AAE@ABV0@@Z -- 11 bytes verified against Fable.exe 0x00BFE960
//
// Retail 0x00BFE960..0x00BFE96A is the whole 11-byte .text section (COMDAT, section 2) of ti_inst.obj in the VS
// .NET 2003 Vc7/lib/msvcrt.lib that retail links (it imports MSVCR71.dll). The object is C++ (@comp.id 0x0060
// build 3077, a retail Rich header entry). 1 of its 1 relocations are proven against retail:
// +4:DIR32:??_7type_info@@6B@=0x012A8370[ok]. This is type_info's private copy constructor (`mov eax,ecx ; mov
// dword ptr [eax],offset ??_7type_info@@6B@ ; ret 4`). RTTI selects this copy: retail .text holds 149 copies of
// the masked 11 bytes (every class whose copy constructor only stores its vftable), and only 0x00BFE960 stores
// 0x012A8370. The dword before that vftable (0x012A836C) points to the Complete Object Locator 0x0135EB68, whose
// TypeDescriptor 0x0139C890 is named `.?AVtype_info@@`, and the vftable's slot 0 is 0x00BFE96B, ti_inst.obj's
// ??_Etype_info@@UAEPAXI@Z (the next row). So the relocation +4:DIR32:??_7type_info@@6B@ is proven and the other
// 148 copies are other classes. No retail code calls 0x00BFE960. Byte-identical alternates (not linked):
// ddk3790/libc.lib(obj\i386\typinfo.obj), ddk3790/libcd.lib(obj\i386\typinfo.obj),
// ddk3790/libcmt.lib(obj\i386\typinfo.obj), ddk3790/libcmtd.lib(obj\i386\typinfo.obj),
// ddk3790/libcp.lib(build\intel\zst_obj\iostream.obj), ddk3790/libcp.lib(build\intel\zst_obj\wiostrea.obj) (+8).
// The address was outside the function manifest until the 2026-10-30 boundary pass added it (agent_source
// boundary); vendor bytes are linked, not reconstructed (owner rule 2026-10-30).
// The build links msvcrt.lib for these bytes; nothing here is compiled and there is no body to write.
// Proof: python3 tools/decomp_pipeline/libsig/lib_verify.py 00bfe960 -> MATCH;
// oracle row in rebuild/oracles/library_signatures.tsv; docs/pipeline/LIBRARY_SIGNATURES.md.
