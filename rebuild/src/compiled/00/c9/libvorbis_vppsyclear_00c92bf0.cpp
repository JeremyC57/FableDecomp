// libvorbis::_vp_psy_clear(vorbis_look_psy* p) -- free a psychoacoustic look's ATH/octave/bark tables, the 17x8 tone curves and the 3 noise offset curves, then wipe it
// Leaf: _vp_psy_clear
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C92BF0, 174 bytes. libvorbis 1.0.1 lib/psy.c, compiled as C with /Gr:
//   `__fastcall`, p in ecx, plain `ret`.
// Callees: free (MSVCR71 import [0x01440154], hoisted into ebp) through _ogg_free.
// Layout: libvorbis 1.0.1 vorbis_look_psy (0x30 bytes: n, vi, tonecurves, noiseoffset, ath, octave, bark,
//   firstoc, shiftoc, eighth_octave_lines, total_octave_lines, rate). P_BANDS 17, P_LEVELS 8, P_NOISECURVES 3.
// Shape: the shipped source; the loops count byte offsets (`cmp esi,20h`, `cmp ebx,44h`, `cmp esi,0Ch`) and
//   re-read p->tonecurves / p->noiseoffset after every free; memset(p,0,0x30) is `rep stosd` x12.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

#define P_BANDS 17
#define P_LEVELS 8
#define P_NOISECURVES 3

typedef struct vorbis_look_psy {
    int     n;
    void*   vi;
    float*** tonecurves;
    float** noiseoffset;
    float*  ath;
    long*   octave;
    long*   bark;
    long    firstoc;
    long    shiftoc;
    int     eighth_octave_lines;
    int     total_octave_lines;
    long    rate;
} vorbis_look_psy;

extern "C" void __fastcall _vp_psy_clear(vorbis_look_psy* p)
{
    int i, j;
    if (p) {
        if (p->ath) _ogg_free(p->ath);
        if (p->octave) _ogg_free(p->octave);
        if (p->bark) _ogg_free(p->bark);
        if (p->tonecurves) {
            for (i = 0; i < P_BANDS; i++) {
                for (j = 0; j < P_LEVELS; j++) {
                    _ogg_free(p->tonecurves[i][j]);
                }
                _ogg_free(p->tonecurves[i]);
            }
            _ogg_free(p->tonecurves);
        }
        if (p->noiseoffset) {
            for (i = 0; i < P_NOISECURVES; i++) {
                _ogg_free(p->noiseoffset[i]);
            }
            _ogg_free(p->noiseoffset);
        }
        memset(p, 0, sizeof(*p));
    }
}