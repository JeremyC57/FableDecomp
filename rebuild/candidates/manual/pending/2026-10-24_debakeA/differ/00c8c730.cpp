// Grade: DIFFER(334v326) -- IMPOSSIBLE: libvorbisfile's static _ov_d_seek_lap compiled with link-time code generation: it receives `vf` in eax (`mov esi,eax` at entry, no stack or ecx/edx home) and calls its static callees with LTCG custom register conventions (_ov_initset 0x00C8C1D0 with vf in esi, ov_info 0x00C8BAB0 (ecx=vf, edx=-1), ov_halfrate_p 0x00C8A920, vorbis_info_blocksize 0x00C8DF60 (ecx, edx), _ov_getlap 0x00C8C240, _ov_splice 0x00C8BF90 with eax/ecx/edx + 5 stack words). VC7.1 without /GL cannot emit an eax-argument function, so no single-TU source reproduces the body; the source below is the library code it was built from (ret 0Ch = pos (double) + localseek).
// _ov_d_seek_lap(OggVorbis_File* vf, double pos, int (*localseek)(OggVorbis_File*, double)) -- vorbisfile lapped seek
// Retail 0x00C8C730, 326 bytes (catalog "Batch_InvokeWithStackArgs"; see rebuild/oracles/c8c730-boundary-note.md for the
// 326-byte cut). Identification: `ready_state < OPENED` -> OV_EINVAL (-131, `mov eax,0FFFFFF7Dh`), two alloca'd lap
// buffer tables (_chkstk 0x00BFEA30), the localseek callback called with the double `pos` and vf in ecx, then
// _ov_initprime (0x00C8C200), vorbis_synthesis_lapout (0x00C90950) and the splice; Fable.exe links libVorbis
// ("Xiph.Org libVorbis I 20030909").
// Leaf: _ov_d_seek_lap

#include <malloc.h>

struct vorbis_info { int version; int channels; long rate; };
struct vorbis_dsp_state { int analysisp; vorbis_info* vi; };
struct OggVorbis_File {
    unsigned char _pad[0x58];
    int ready_state;                                       // +0x58
    unsigned char _pad5c[0x1e0 - 0x5c];
    vorbis_dsp_state vd;                                   // +0x1e0
};
enum { OPENED = 2, OV_EINVAL = -131 };

extern int _ov_initset(OggVorbis_File* vf);                // 0x00C8C1D0
extern int _ov_initprime(OggVorbis_File* vf);              // 0x00C8C200
extern vorbis_info* ov_info(OggVorbis_File* vf, int link); // 0x00C8BAB0
extern long ov_halfrate_p(OggVorbis_File* vf);             // 0x00C8A920
extern int vorbis_info_blocksize(vorbis_info* vi, int zo); // 0x00C8DF60
extern float* vorbis_window(vorbis_dsp_state* v, int W);   // 0x00C90C80
extern int vorbis_synthesis_lapout(vorbis_dsp_state* v, float*** pcm);   // 0x00C90950
extern void _ov_getlap(OggVorbis_File* vf, vorbis_info* vi, vorbis_dsp_state* vd, float** lappcm, int lapsize);   // 0x00C8C240
extern void _ov_splice(float** pcm, float** lappcm, int n1, int n2, int ch1, int ch2, float* w1, float* w2);       // 0x00C8BF90

static int _ov_d_seek_lap(OggVorbis_File* vf, double pos, int (*localseek)(OggVorbis_File*, double))
{
    vorbis_info* vi;
    float** lappcm;
    float** pcm;
    float* w1;
    float* w2;
    int n1, n2, ch1, ch2, hs;
    int i, ret;

    if (vf->ready_state < OPENED) return OV_EINVAL;
    ret = _ov_initset(vf);
    if (ret) return ret;
    vi = ov_info(vf, -1);
    hs = ov_halfrate_p(vf);

    ch1 = vi->channels;
    n1 = vorbis_info_blocksize(vi, 0) >> (1 + hs);
    w1 = vorbis_window(&vf->vd, 0);

    lappcm = (float**)alloca(sizeof(*lappcm) * ch1);
    for (i = 0; i < ch1; i++)
        lappcm[i] = (float*)alloca(sizeof(**lappcm) * n1);
    _ov_getlap(vf, vi, &vf->vd, lappcm, n1);

    ret = localseek(vf, pos);
    if (ret) return ret;
    ret = _ov_initprime(vf);
    if (ret) return ret;

    vi = ov_info(vf, -1);
    ch2 = vi->channels;
    n2 = vorbis_info_blocksize(vi, 0) >> (1 + hs);
    w2 = vorbis_window(&vf->vd, 0);

    vorbis_synthesis_lapout(&vf->vd, &pcm);
    _ov_splice(pcm, lappcm, n1, n2, ch1, ch2, w1, w2);
    return 0;
}

int (*g_keep_ov_d_seek_lap)(OggVorbis_File*, double, int (*)(OggVorbis_File*, double)) = _ov_d_seek_lap;
