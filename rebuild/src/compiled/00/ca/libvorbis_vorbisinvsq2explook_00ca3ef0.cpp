// libvorbis::vorbis_invsq2explook(int a) -- Xiph lookup.c: 2^(-a/2) from the INVSQ2EXP table (a from -32 to 32)
// Leaf: @vorbis_invsq2explook@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA3EF0, 8 bytes, __fastcall (a in ecx), plain ret, float in st0.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: `fld dword [ecx*4+0x013AC570]`: the table starts at 0x013AC4F0 with 65536.0, 46340.95, 32768.0 ... and
//   holds 1.0 at 0x013AC570 = INVSQ2EXP_LOOKUP[a - INVSQ2EXP_LOOKUP_MIN] with MIN = -32 (the -(-32)*4 = 0x80 offset is
//   folded into the displacement); its caller 0x00CA46A4 is lsp.c's vorbis_lsp_to_curve (lsp.c includes lookup.c under
//   FLOAT_LOOKUP). Label correction: the catalog's "CMemoryAllocator::IsAllocatorFixedSize" is propagated.
#define INVSQ2EXP_LOOKUP_MIN (-32)
extern "C" float INVSQ2EXP_LOOKUP[65];  // 0x013AC4F0 (static in lookup_data.h)

extern "C" float __fastcall vorbis_invsq2explook(int a)
{
    return INVSQ2EXP_LOOKUP[a - INVSQ2EXP_LOOKUP_MIN];
}