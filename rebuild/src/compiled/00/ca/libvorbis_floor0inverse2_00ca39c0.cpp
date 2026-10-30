// libvorbis::floor0_inverse2(vorbis_block* vb, vorbis_look_floor* i, void* memo, float* out) -- libvorbis floor0.c: render a decoded floor-0 LSP set into the spectral envelope (or zero it when the packet had none)
// Leaf: @floor0_inverse2@16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA39C0, 114 bytes. libvorbis I 20030909 lib/floor0.c, slot 6 (inverse2) of
//   floor0_exportbundle 0x013AC244; LTCG-API `__fastcall` (vb ecx, i edx, memo/out on the stack, `ret 8`).
// Callees: floor0_map_lazy_init 0x00CA36E0 (floor0.c static: vb in eax, look in edi, info pushed, `ret 4`),
//   vorbis_lsp_to_curve 0x00CA4570 (`__fastcall`), the memset intrinsic (`rep stosd` + `rep stosb`).
// Shape (retry1): parked as "register allocation follows the LTCG callee convention"; with the real floor0.c static body
//   in the TU inside `extern "C" { static __declspec(noinline) __stdcall }` VC7.1 gives floor0_map_lazy_init retail's
//   private convention (vb eax, look edi, callee-pop info) and the caller matches. The static's body is the libvorbis
//   source (toBARK via the atan intrinsic, floor IAT 0x01440100, malloc IAT 0x01440158).
// Label: manifest "Geometry_TransformAndScale" is a descriptive placeholder; the real name is floor0.c's floor0_inverse2.
#include <string.h>
#include <math.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // IAT 0x01440158
#define _ogg_malloc malloc
#define toBARK(n) (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))
typedef void vorbis_look_floor;
typedef void vorbis_info_floor;
typedef struct { long blocksizes[2]; } codec_setup_info;
typedef struct { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower;
                 long bitrate_window; void* codec_setup; } vorbis_info;
typedef struct { int analysisp; vorbis_info* vi; } vorbis_dsp_state;
typedef struct { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; } oggpack_buffer;
typedef struct {
    float** pcm; oggpack_buffer opb; long lW; long W; long nW; int pcmend; int mode; int eofflag;
    __int64 granulepos; __int64 sequence; vorbis_dsp_state* vd;
} vorbis_block;
typedef struct {
    int order; long rate; long barkmap; int ampbits; int ampdB;
    int numbooks; int books[16]; float lessthan; float greaterthan;
} vorbis_info_floor0;
typedef struct {
    int ln; int m; int** linearmap; int n[2]; vorbis_info_floor0* vi; long bits; long frames;
} vorbis_look_floor0;

extern "C" {
void __fastcall vorbis_lsp_to_curve(float* curve, int* map, int n, int ln, float* lsp, int m,
                                    float amp, float ampoffset);   // 0x00CA4570

static __declspec(noinline) void __stdcall floor0_map_lazy_init(vorbis_block* vb, vorbis_info_floor* infoX,
                                                                vorbis_look_floor0* look)   // 0x00CA36E0
{
    if (!look->linearmap[vb->W]) {
        vorbis_dsp_state* vd = vb->vd;
        vorbis_info* vi = vd->vi;
        codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
        vorbis_info_floor0* info = (vorbis_info_floor0*)infoX;
        int W = vb->W;
        int n = ci->blocksizes[W] / 2, j;

        float scale = look->ln / toBARK(info->rate / 2.f);

        look->linearmap[W] = (int*)_ogg_malloc((n + 1) * sizeof(**look->linearmap));
        for (j = 0; j < n; j++) {
            int val = floor(toBARK((info->rate / 2.f) / n * j) * scale);
            if (val >= look->ln) val = look->ln - 1;
            look->linearmap[W][j] = val;
        }
        look->linearmap[W][j] = -1;
        look->n[W] = n;
    }
}

int __fastcall floor0_inverse2(vorbis_block* vb, vorbis_look_floor* i,
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
}