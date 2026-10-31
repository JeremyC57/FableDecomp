// Behaviour test for libvorbis _ov_getlap (0x00C8C240): lapping buffers are filled from decoded PCM (decoding packets
// when none is buffered), topped up from the lapout data, or zero-filled (with upstream's 4*lapsize-lapcount byte length).
#include <cstdio>
#include <cstring>

struct Decoder { int avail; int packets; int lapout; float base; int fetches; int reads; };
struct Info { int channels; };
static float g_ch[2][64];
static float* g_pcm[2] = { g_ch[0], g_ch[1] };

static int pcmout(Decoder* d, float*** pcm) { if (d->avail <= 0) return 0; *pcm = g_pcm; return d->avail; }
static void synth_read(Decoder* d, int n) { d->avail -= n; d->reads++; }
static int fetch(Decoder* d) { d->fetches++; if (d->packets <= 0) return -2; d->packets--; d->avail = 5; return 1; }
static int lapout(Decoder* d, float*** pcm) { *pcm = g_pcm; return d->lapout; }

static void getlap(Decoder* d, Info* vi, float** lappcm, int lapsize)
{
    int lapcount = 0, i;
    float** pcm;
    while (lapcount < lapsize) {
        int samples = pcmout(d, &pcm);
        if (samples) {
            if (samples > lapsize - lapcount) samples = lapsize - lapcount;
            for (i = 0; i < vi->channels; i++) std::memcpy(lappcm[i] + lapcount, pcm[i], sizeof(**pcm) * samples);
            lapcount += samples;
            synth_read(d, samples);
        } else {
            int ret = fetch(d);
            if (ret == -2) break;
        }
    }
    if (lapcount < lapsize) {
        int samples = lapout(d, &pcm);
        if (samples == 0) {
            for (i = 0; i < vi->channels; i++) std::memset(lappcm[i] + lapcount, 0, sizeof(**pcm) * lapsize - lapcount);
            lapcount = lapsize;
        } else {
            if (samples > lapsize - lapcount) samples = lapsize - lapcount;
            for (i = 0; i < vi->channels; i++) std::memcpy(lappcm[i] + lapcount, pcm[i], sizeof(**pcm) * samples);
            lapcount += samples;
        }
    }
}

int main()
{
    int i;
    for (i = 0; i < 64; ++i) { g_ch[0][i] = (float)(i + 1); g_ch[1][i] = (float)(100 + i); }
    float a[2][64], *lap[2] = { a[0], a[1] };
    Info vi; vi.channels = 2;
    bool ok = true;
    // enough packets: 12 samples = 5 + 5 + 2
    Decoder d = { 0, 3, 0, 0.0f, 0, 0 };
    std::memset(a, 0x7f, sizeof(a));
    getlap(&d, &vi, lap, 12);
    ok = ok && d.fetches == 3 && d.reads == 3 && a[0][0] == 1.0f && a[0][5] == 1.0f && a[0][10] == 1.0f && a[1][11] == 101.0f && d.avail == 3;
    // EOF after one packet: 5 decoded, then 3 samples from lapout
    Decoder e = { 0, 1, 3, 0.0f, 0, 0 };
    std::memset(a, 0, sizeof(a));
    getlap(&e, &vi, lap, 10);
    ok = ok && e.fetches == 2 && a[0][4] == 5.0f && a[0][5] == 1.0f && a[0][7] == 3.0f && a[0][8] == 0.0f;
    // EOF and no lapout: the tail is zeroed for 4*lapsize-lapcount bytes from lapcount
    Decoder z = { 0, 1, 0, 0.0f, 0, 0 };
    std::memset(a, 0x7f, sizeof(a));
    getlap(&z, &vi, lap, 8);
    ok = ok && a[0][4] == 5.0f && a[0][5] == 0.0f && a[0][7] == 0.0f;
    unsigned char* tail = (unsigned char*)(a[0] + 5);
    ok = ok && tail[4 * 8 - 5 - 1] == 0 && tail[4 * 8 - 5] == 0x7f;
    std::printf(ok ? "00c8c240_TEST PASS\n" : "00c8c240_TEST FAIL\n");
    return 0;
}