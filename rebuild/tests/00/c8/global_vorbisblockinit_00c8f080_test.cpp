// Behaviour test for 00c8f080: vorbis_block_init -- clears the block, binds the DSP state, analysis states get the internal block
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_packer = 0;
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct vorbis_dsp_state { int analysisp; };
struct vorbis_block_internal { float** pcmdelay; float ampmax; int blocktype; unsigned int markers[15]; };
struct vorbis_block { float** pcm; oggpack_buffer opb; long lW, W, nW; int pcmend, mode, eofflag; __int64 granulepos, sequence;
    vorbis_dsp_state* vd; void* localstore; long localtop, localalloc, totaluse; void* reap; long glue_bits, time_bits, floor_bits, res_bits; void* internal; };
static void oggpack_writeinit(oggpack_buffer* b) { ++g_packer; std::memset(b, 0, sizeof(*b)); }
static int vorbis_block_init(vorbis_dsp_state* v, vorbis_block* vb)
{
    std::memset(vb, 0, sizeof(*vb)); vb->vd = v; vb->localalloc = 0; vb->localstore = 0;
    if (v->analysisp) { vorbis_block_internal* vbi = (vorbis_block_internal*)(vb->internal = std::calloc(1, sizeof(vorbis_block_internal))); oggpack_writeinit(&vb->opb); vbi->ampmax = -9999; }
    return (0);
}
int main()
{
    vorbis_dsp_state synth = {0}, analysis = {1}; vorbis_block vb;
    std::memset(&vb, 0x55, sizeof vb);
    int ok = vorbis_block_init(&synth, &vb) == 0 && vb.vd == &synth && vb.internal == 0 && vb.lW == 0 && g_packer == 0;
    ok = ok && vorbis_block_init(&analysis, &vb) == 0 && vb.internal != 0 && g_packer == 1 && ((vorbis_block_internal*)vb.internal)->ampmax == -9999.0f;
    ok = ok && sizeof(vorbis_block) == 0x70 && sizeof(vorbis_block_internal) == 0x48;
    std::free(vb.internal);
    std::printf(ok ? "00c8f080_TEST PASS\n" : "00c8f080_TEST FAIL\n");
    return ok ? 0 : 1;
}
