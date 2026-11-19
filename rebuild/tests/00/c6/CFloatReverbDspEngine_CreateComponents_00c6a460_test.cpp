// Behaviour test for CReverbProcessor::Allocate (0x00C6A460): the two-pass layout restarts the line index, sizes the
// processor's own block, the four combs (rate/5 + 128 samples rounded up to a multiple of 4, consecutive line indices
// after the first one kept in m_firstLineIndex), the tap bank, diffuser, delay, pre-delay and stereo pair from one
// cursor, and returns the byte total; the memory pass hands out exactly the bytes the sizing pass counted.
#include <cstdio>

struct Config { unsigned int channels; unsigned int rate; };

struct Proc {
    unsigned int lineIndex, firstLineIndex, gainResets;
    unsigned int combIndex[4], combLength[4];
    float* combData[4];
    unsigned int delayIndex, preDelayIndex;
    static void Take(float** mem, unsigned int* bytes, unsigned int n) { if (*mem) *mem += n; *bytes += n * 4; }
    void Allocate(const Config* c, float* memory, unsigned int* pBytes)
    {
        lineIndex = 0;
        unsigned int bytes = 0;
        Take(&memory, &bytes, 128);                       // the processor's own work block
        gainResets += 2;
        firstLineIndex = lineIndex++;
        unsigned int length = ((unsigned int)(long)((float)c->rate * 0.2f) + 128 + 3) & ~3u;
        for (int i = 0; i < 4; ++i) {
            combData[i] = memory; combLength[i] = length; combIndex[i] = lineIndex;
            Take(&memory, &bytes, length);
            lineIndex++;
        }
        lineIndex += 4; Take(&memory, &bytes, 4 * 64); // tap bank: four taps
        lineIndex += 2; Take(&memory, &bytes, 2 * c->rate); // diffuser: two stages
        delayIndex = lineIndex; Take(&memory, &bytes, c->rate / 4);
        preDelayIndex = ++lineIndex; Take(&memory, &bytes, c->rate / 2);
        lineIndex++;
        Take(&memory, &bytes, 1051 + 337);              // stereo pair
        *pBytes = bytes;
    }
};

int main()
{
    int ok = 1;
    Config c; c.channels = 2; c.rate = 48000;
    Proc p; p.gainResets = 0; p.lineIndex = 99;
    unsigned int size = 0;
    p.Allocate(&c, 0, &size);
    ok &= p.combLength[0] == 9728 && p.firstLineIndex == 0 && p.combIndex[0] == 1 && p.combIndex[3] == 4;
    ok &= p.delayIndex == 11 && p.preDelayIndex == 12 && p.lineIndex == 13 && p.gainResets == 2;
    ok &= size == 4 * (128 + 4 * 9728 + 256 + 96000 + 12000 + 24000 + 1388);
    static float block[200000];
    unsigned int size2 = 0;
    p.Allocate(&c, block, &size2);
    ok &= size2 == size && p.combData[0] == block + 128 && p.combData[1] == block + 128 + 9728;
    c.rate = 22050;
    p.Allocate(&c, 0, &size);
    ok &= p.combLength[0] == 4540;
    if (!ok) { std::printf("00c6a460_TEST FAIL\n"); return 1; }
    std::printf("00c6a460_TEST PASS\n");
    return 0;
}