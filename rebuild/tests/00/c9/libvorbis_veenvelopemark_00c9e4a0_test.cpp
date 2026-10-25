// Behaviour test for 0x00C9E4A0 libvorbis _ve_envelope_mark (copy of the model + logic).
#include <cstdio>
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct codec_setup_part { long blocksizes[2]; };
struct envelope_part { int searchstep; int* mark; long current; long curmark; };
struct private_part { envelope_part* ve; };
struct vorbis_dsp_state { vorbis_info* vi; long lW, W, nW, centerW; void* backend_state; };
static int envelope_mark(vorbis_dsp_state* v)
{
    envelope_part* ve = ((private_part*)(v->backend_state))->ve;
    vorbis_info* vi = v->vi;
    codec_setup_part* ci = (codec_setup_part*)vi->codec_setup;
    long centerW = v->centerW;
    long beginW = centerW - ci->blocksizes[v->W] / 4;
    long endW = centerW + ci->blocksizes[v->W] / 4;
    if (v->W) { beginW -= ci->blocksizes[v->lW] / 4; endW += ci->blocksizes[v->nW] / 4; }
    else { beginW -= ci->blocksizes[0] / 4; endW += ci->blocksizes[0] / 4; }
    if (ve->curmark >= beginW && ve->curmark < endW) return 1;
    {
        long first = beginW / ve->searchstep, last = endW / ve->searchstep, i;
        for (i = first; i < last; i++) if (ve->mark[i]) return 1;
    }
    return 0;
}
int main()
{
    int marks[64] = { 0 };
    codec_setup_part ci = { { 256, 2048 } };
    vorbis_info vi = { 0, 2, 44100, 0, 0, 0, 0, &ci };
    envelope_part ve = { 64, marks, 0, -1 };
    private_part b = { &ve };
    vorbis_dsp_state v = { &vi, 0, 0, 0, 1024, &b };      /* short block: span 1024-128 .. 1024+128 */
    int ok = 1;
    if (envelope_mark(&v) != 0) ok = 0;
    ve.curmark = 1000; if (envelope_mark(&v) != 1) ok = 0;   /* curmark inside the span */
    ve.curmark = 1152; if (envelope_mark(&v) != 0) ok = 0;   /* end is exclusive */
    marks[15] = 1;                                            /* 960 / 64 = 15 is inside [896/64, 1152/64) */
    if (envelope_mark(&v) != 1) ok = 0;
    marks[15] = 0; marks[18] = 1;
    if (envelope_mark(&v) != 0) ok = 0;                       /* 18 == last: not scanned */
    v.W = 1; v.lW = 0; v.nW = 1; v.centerW = 2048;            /* long block: 2048-512-64 .. 2048+512+512 */
    if (envelope_mark(&v) != 0) ok = 0;                       /* scan is 1472/64 = 23 .. 3072/64 = 48: mark 18 is outside */
    marks[30] = 1;
    if (envelope_mark(&v) != 1) ok = 0;
    ve.curmark = 3000; marks[30] = 0;
    if (envelope_mark(&v) != 1) ok = 0;                       /* curmark inside the long span */
    if (!ok) { std::printf("00c9e4a0_TEST FAIL\n"); return 1; }
    std::printf("00c9e4a0_TEST PASS\n");
    return 0;
}