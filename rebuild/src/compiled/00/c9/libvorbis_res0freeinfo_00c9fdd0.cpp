// libvorbis::res0_free_info(vorbis_info_residue* i) -- wipe and free a residue 0/1/2 setup block; NULL is ignored
// Leaf: @res0_free_info@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9FDD0, 30 bytes, i in ecx, plain `ret`. libvorbis 1.0.1 lib/res0.c (exported;
//   in the residue0/1/2 export tables beside res0_free_look 0x00C9FDF0, which follows it).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the free is the MSVCR71
//   import [0x01440154] through _ogg_free, as in the landed agent447 rows (mdct_clear 0x00C9C4C0, res0_free_look 0x00C9FDF0).
// Label: catalog `std::_Cons_val<std::allocator<C3DGeneratorObject>,...>` is propagated; its source DEFINED a data global
//   `g_initObject` to stand in for the free import and modelled the struct as 0x1c5 opaque dwords.
// Shape: the shipped source; `rep stosd` x0x1c5 is the memset of sizeof(vorbis_info_residue0).
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef void vorbis_info_residue;
typedef struct vorbis_info_residue0 {              // libvorbis 1.0.1 backends.h, 0x714 bytes
    long  begin;
    long  end;
    int   grouping;
    int   partitions;
    int   groupbook;
    int   secondstages[64];
    int   booklist[256];
    float classmetric1[64];
    float classmetric2[64];
} vorbis_info_residue0;

extern "C" void __fastcall res0_free_info(vorbis_info_residue* i)
{
    vorbis_info_residue0* info = (vorbis_info_residue0*)i;
    if (info) {
        memset(info, 0, sizeof(*info));
        _ogg_free(info);
    }
}
