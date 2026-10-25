// libvorbis::mapping0_free_info(vorbis_info_mapping* i) -- wipe and free a mapping-type-0 setup block; NULL is ignored
// Leaf: @mapping0_free_info@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9EC00, 30 bytes, i in ecx, plain `ret`. libvorbis 1.0.1 lib/mapping0.c, a
//   `static` reached through the mapping0_exportbundle table (so the /Gr convention); given external linkage here so the
//   row's symbol is emitted (agent447 LESSONS: unreferenced statics are dropped under /O2).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the free is the MSVCR71
//   import [0x01440154] through _ogg_free, as in the landed agent447 rows (mdct_clear 0x00C9C4C0, res0_free_look 0x00C9FDF0).
// Label: catalog `std::_Cons_val<std::allocator<C3DGeneratorObject>,...>` is propagated; its source modelled the free
//   import as an undefined function-pointer global and the struct as 0x322 opaque dwords.
// Shape: the shipped source; `rep stosd` x0x322 is the memset of sizeof(vorbis_info_mapping0).
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef void vorbis_info_mapping;
typedef struct {                                   // libvorbis 1.0.1 mapping0.c vorbis_info_mapping0, 0xC88 bytes
    int submaps;
    int chmuxlist[256];
    int floorsubmap[16];
    int residuesubmap[16];
    int coupling_steps;
    int coupling_mag[256];
    int coupling_ang[256];
} vorbis_info_mapping0;

extern "C" void __fastcall mapping0_free_info(vorbis_info_mapping* i)
{
    vorbis_info_mapping0* info = (vorbis_info_mapping0*)i;
    if (info) {
        memset(info, 0, sizeof(*info));
        _ogg_free(info);
    }
}
