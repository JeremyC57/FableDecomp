// Behaviour test for CVtable_012b5d40::MixAdd16Ramp (0x00C5C090): sixteen blocks of 16-bit samples are added into the long accumulator,
// each scaled by its lane's ramping gain (high word of sample*gain, doubled), the gains stepping after every block and saturating.
#include <stdio.h>
static void Kernel(const short* src, unsigned long count, long* dst, short initial, short step)
{
    int gains[4]; for (int i = 0; i < 4; ++i) gains[i] = (short)(initial + i * step);
    for (unsigned long block = 0; block < count * 2; ++block) {
        for (int lane = 0; lane < 4; ++lane) { int product = (int)src[block * 4 + lane] * gains[lane]; int high = product / 65536; if (product < 0 && product % 65536) --high; dst[block * 4 + lane] += high * 2; }
        for (int lane = 0; lane < 4; ++lane) { int next = gains[lane] + (short)(step * 4); gains[lane] = next > 32767 ? 32767 : (next < -32768 ? -32768 : next); }
    }
}
struct CVtable_012b5d40 {
    virtual ~CVtable_012b5d40() { }
    virtual void MixAdd16Ramp(const short* src, long* dst, short initial, short step);
};
void CVtable_012b5d40::MixAdd16Ramp(const short* src, long* dst, short initial, short step)
{
    Kernel(src, 16, dst, initial, step);
}
static int floorHigh(int product) { int high = product / 65536; if (product < 0 && product % 65536) --high; return high; }
int main(void)
{
    static short src[128]; static long dst[128]; static long want[128];
    int i;
    for (i = 0; i < 128; ++i) { src[i] = (short)(i * 257 - 16000); dst[i] = i; }
    short initial = 0x2000, step = 0x10;
    for (i = 0; i < 128; ++i) {
        int lane = i % 4, block = i / 4;
        int gain = (short)(initial + lane * step) + block * (short)(step * 4);
        if (gain > 32767) gain = 32767;
        want[i] = i + floorHigh((int)src[i] * gain) * 2;
    }
    CVtable_012b5d40 buffer; buffer.MixAdd16Ramp(src, dst, initial, step);
    for (i = 0; i < 128; ++i) if (dst[i] != want[i]) { printf("00c5c090_TEST FAIL at %d (%ld, want %ld)\n", i, dst[i], want[i]); return 1; }
    printf("00c5c090_TEST PASS\n");
    return 0;
}