// libvorbis::res1_class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int* nonzero, int ch) -- Xiph res0.c: pack the nonzero channels to the front of `in` and classify their partitions for residue type 1 (NULL when every channel is silent)
// Leaf: @res1_class@20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA1250, 84 bytes, __fastcall (vb ecx, vl edx, in/nonzero/ch on the
//   stack), ret 0Ch. Callee: _01class 0x00CA0680 (static in res0.c; LTCG left it out of line with all four arguments on
//   the stack and a callee pop, `ret 10h` -- declared `__stdcall`, which is that convention at the call site, as the
//   landed codebook.c rows declare decode_packed_entry_number 0x00C96320).
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: slot 5 (class) of residue1_exportbundle at 0x013ABDE4 (pack 0x00C9FEA0, unpack 0x00C9FF80, look 0x00CA00E0,
//   free_info 0x00C9FDD0, free_look 0x00C9FDF0 -- the landed res0_* rows --, class = this, forward 0x00CA1060, inverse
//   0x00CA12B0, whose body is the same compaction loop passing _01inverse the decodepart 0x00C965C0). The catalog label
//   (gap-scan) is a placeholder.
// Shape: vb is homed in a stack slot across the loop (push ecx) and reloaded for the call.
typedef struct vorbis_block vorbis_block;
typedef void vorbis_look_residue;

extern "C" long** __stdcall _01class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int ch);   // 0x00CA0680

extern "C" long** __fastcall res1_class(vorbis_block* vb, vorbis_look_residue* vl,
                                        float** in, int* nonzero, int ch)
{
    int i, used = 0;
    for (i = 0; i < ch; i++)
        if (nonzero[i])
            in[used++] = in[i];
    if (used)
        return (_01class(vb, vl, in, used));
    else
        return (0);
}