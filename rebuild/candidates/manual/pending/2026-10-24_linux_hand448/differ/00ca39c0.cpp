// libvorbis::floor0_inverse2(vorbis_block* vb, vorbis_look_floor* i, void* memo, float* out) -- libvorbis floor0.c: render a decoded floor-0 LSP set into the spectral envelope (or zero it when the packet had none)
// Grade: DIFFER(114v114) -- lane gap (register allocation follows the callee convention): the static floor0_map_lazy_init 0x00CA36E0 takes vb in eax and look in edi (LTCG custom register convention: `push ebx ; mov eax,esi ; call`); declared as a C++ function it takes ecx/edx
// Leaf: @floor0_inverse2@16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA39C0, 114 bytes (+14 int3). libvorbis lib/floor0.c; slot 6 (inverse2) of
//   floor0_exportbundle at 0x013AC244 (unpack 0x00CA3600, look 0x00CA3850, free_info 0x00CA3580, free_look 0x00CA35A0,
//   inverse1 0x00CA3890); `__fastcall` (vb ecx, i edx, memo/out on the stack, `ret 8`).
// Callees: floor0_map_lazy_init 0x00CA36E0 (static, custom convention), vorbis_lsp_to_curve 0x00CA4570 (`__fastcall`,
//   curve in ecx, map in edx, six stack arguments), the memset intrinsic (`rep stosd` + `rep stosb`).
// Label: manifest "Geometry_TransformAndScale" is a descriptive placeholder; the real name is the floor0.c static `floor0_inverse2`.
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)

typedef void vorbis_look_floor;
typedef struct {
    float** pcm; long opb[5]; long lW; long W;   // W at +0x1c
} vorbis_block;
typedef struct {
    int order; long rate; long barkmap; int ampbits; int ampdB;   // ampdB at +0x10
    int numbooks; int books[16]; float lessthan; float greaterthan;
} vorbis_info_floor0;
typedef struct {
    int ln; int m; int** linearmap; int n[2]; vorbis_info_floor0* vi; long bits; long frames;
} vorbis_look_floor0;

extern "C" void __fastcall floor0_map_lazy_init(vorbis_block* vb, vorbis_info_floor0* info, vorbis_look_floor0* look);   // 0x00CA36E0
extern "C" void __fastcall vorbis_lsp_to_curve(float* curve, int* map, int n, int ln, float* lsp, int m,
                                               float amp, float ampoffset);   // 0x00CA4570

extern "C" int __fastcall floor0_inverse2(vorbis_block* vb, vorbis_look_floor* i,
                                          void* memo, float* out)
{
    vorbis_look_floor0* look = (vorbis_look_floor0*)i;
    vorbis_info_floor0* info = look->vi;

    floor0_map_lazy_init(vb, info, look);

    if (memo) {
        float* lsp = (float*)memo;
        float amp = lsp[look->m];

        /* take the coefficients back to a spectral envelope curve */
        vorbis_lsp_to_curve(out,
                            look->linearmap[vb->W],
                            look->n[vb->W],
                            look->ln,
                            lsp, look->m, amp, (float)info->ampdB);
        return (1);
    }
    memset(out, 0, sizeof(*out) * look->n[vb->W]);
    return (0);
}
