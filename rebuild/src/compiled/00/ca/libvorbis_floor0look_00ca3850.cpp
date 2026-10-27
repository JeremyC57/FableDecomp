// libvorbis::floor0_look(vorbis_dsp_state* vd, vorbis_info_floor* i) -- Xiph floor0.c (post-1.0.1): allocate a floor 0 look over the info: order, bark map size, the info itself and the two lazily built linear maps
// Leaf: @floor0_look@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA3850, 50 bytes, __fastcall (vd ecx, unused; i edx), plain ret.
// Callees: calloc (MSVCR71 import [0x01440058], hoisted into ebx) twice through _ogg_calloc.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: the look slot (+8) of floor0_exportbundle at 0x013AC244 (pack NULL, unpack 0x00CA3600, look = this, free_info
//   0x00CA3580, free_look 0x00CA35A0, inverse1 0x00CA3890, inverse2 0x00CA39C0); the body is the 2003 CVS floor0_look
//   (`look->linearmap=_ogg_calloc(2,sizeof(*look->linearmap))`; libVorbis 1.1 ships it as floor0_look(vd,i)): calloc(1,0x20),
//   m (+4) = info->order (+0), ln (+0) = info->barkmap (+8), vi (+0x14) = info. The catalog label is a placeholder.
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // [0x01440058]
#define _ogg_calloc calloc

typedef void vorbis_info_floor;
typedef void vorbis_look_floor;
typedef struct vorbis_dsp_state vorbis_dsp_state;

typedef struct {
    int   order;
    long  rate;
    long  barkmap;

    int   ampbits;
    int   ampdB;

    int   numbooks; /* <= 16 */
    int   books[16];

    float lessthan;     /* encode-only config setting hacks for libvorbis */
    float greaterthan;  /* encode-only config setting hacks for libvorbis */
} vorbis_info_floor0;

typedef struct {
    int ln;
    int m;
    int** linearmap;
    int n[2];

    vorbis_info_floor0* vi;

    long bits;
    long frames;
} vorbis_look_floor0;

extern "C" vorbis_look_floor* __fastcall floor0_look(vorbis_dsp_state* vd, vorbis_info_floor* i)
{
    vorbis_info_floor0* info = (vorbis_info_floor0*)i;
    vorbis_look_floor0* look = (vorbis_look_floor0*)_ogg_calloc(1, sizeof(*look));
    look->m = info->order;
    look->ln = info->barkmap;
    look->vi = info;

    look->linearmap = (int**)_ogg_calloc(2, sizeof(*look->linearmap));

    return look;
}