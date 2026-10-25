// libvorbis::res2_class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int* nonzero, int ch) -- libvorbis res0.c: residue-2 classification; counts the nonzero channels and, if any, classifies the interleaved vector through _2class
// Grade: DIFFER(59v66) -- retail passes vb to the static _2class 0x00CA0890 in eax (`mov eax,ecx` at entry, LTCG custom register convention) with vl in edx; a plain/__fastcall _2class gives ecx/edx and a different prologue
// Leaf: @res2_class@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1310, 66 bytes (+14 int3). libvorbis lib/res0.c; slot 5 (class) of residue2_exportbundle
//   at 0x013ABE18 (res0_pack 0x00C9FEA0, res0_unpack 0x00C9FF80, res0_look 0x00CA00E0, res0_free_info 0x00C9FDD0,
//   res0_free_look 0x00C9FDF0, res2_class, res2_forward 0x00CA1360, res2_inverse 0x00CA15C0); `__fastcall`, `ret 0Ch`.
// Callee: _2class 0x00CA0890 (static; LTCG gave it vb in eax, vl in edx, in/ch on the stack, callee-clean).
// Label: manifest "Geometry_ProcessNonzeroElements" is a descriptive placeholder; the real name is the libvorbis res2_class.
typedef struct vorbis_block vorbis_block;
typedef void vorbis_look_residue;

extern "C" long** __fastcall _2class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int ch);   // 0x00CA0890

extern "C" long** __fastcall res2_class(vorbis_block* vb, vorbis_look_residue* vl,
                                        float** in, int* nonzero, int ch)
{
    int i, used = 0;
    for (i = 0; i < ch; i++)
        if (nonzero[i]) used++;
    if (used)
        return (_2class(vb, vl, in, ch));
    else
        return (0);
}
