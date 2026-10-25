// Behaviour test for 00c78970: BuildKernelRows4 -- zeroed row buffer, reversed scaled 25-tap kernel at (base - phase), copied to every row
#include <cstdio>
#include <cstring>
using std::memset; using std::memcpy;

void BuildKernelRows4(const short* kernel, long phase, short* rows, long gain)
{
    memset(rows, 0, 256);
    short* first = rows + (4 - phase);
    short* out = first;
    const short* in = kernel + 24;
    for (int i = 0; i < 25; i++)
        *out++ = (short)((*in-- * gain) / 32768);
    memcpy(rows + (37 - phase), first, 25 * sizeof(short));
    memcpy(rows + (70 - phase), first, 25 * sizeof(short));
    memcpy(rows + (103 - phase), first, 25 * sizeof(short));
}

int main()
{
    short kernel[25];
    for (int i = 0; i < 25; i++) kernel[i] = (short)(i * 1000 - 12000);
    static short rows[128 + 8];
    for (int phase = 0; phase < 4; phase++) {
        for (int i = 0; i < 128 + 8; i++) rows[i] = 0x5555;
        long gain = 16384 + phase;
        BuildKernelRows4(kernel, phase, rows, gain);
        for (int i = 0; i < 128; i++) {
            int row = -1, k;
            for (int r = 0; r < 4; r++) { k = i - (4 + r * 33 - phase); if (k >= 0 && k < 25) row = r; }
            short want = 0;
            if (row >= 0) { k = i - (4 + row * 33 - phase); want = (short)((kernel[24 - k] * gain) / 32768); }
            if (rows[i] != want) { std::printf("00c78970_TEST FAIL\n"); return 1; }
        }
        if (rows[128] != 0x5555) { std::printf("00c78970_TEST FAIL\n"); return 1; }
    }
    std::printf("00c78970_TEST PASS\n");
    return 0;
}