// Behaviour test for 0x00C90930 libvorbis vorbis_synthesis_read (copy of the model + logic).
#include <cstdio>
struct vorbis_dsp_state { int analysisp; void* vi; float** pcm; float** pcmret; int pcm_storage; int pcm_current; int pcm_returned; };
static int synthesis_read(vorbis_dsp_state* v, int n)
{
    if (n && v->pcm_returned + n > v->pcm_current) return -131;
    v->pcm_returned += n;
    return 0;
}
int main()
{
    vorbis_dsp_state v = { 0 };
    v.pcm_current = 10; v.pcm_returned = 3;
    int ok = 1;
    if (synthesis_read(&v, 0) != 0 || v.pcm_returned != 3) ok = 0;
    if (synthesis_read(&v, 3) != 0 || v.pcm_returned != 6) ok = 0;
    if (synthesis_read(&v, 5) != -131 || v.pcm_returned != 6) ok = 0;   /* OV_EINVAL, unchanged */
    if (synthesis_read(&v, 4) != 0 || v.pcm_returned != 10) ok = 0;
    if (!ok) { std::printf("00c90930_TEST FAIL\n"); return 1; }
    std::printf("00c90930_TEST PASS\n");
    return 0;
}