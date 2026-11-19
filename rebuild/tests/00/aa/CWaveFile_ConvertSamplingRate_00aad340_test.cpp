// Behaviour test for 0x00AAD340 CWaveFile::ConvertSamplingRate (links the REAL row object).
// CWaveFile is raw native-layout storage, every offset read by the row: Channels +0x00 (0x00AAD447), SampleLength +0x04 (0x00AAD3E5),
// SamplingFrequency +0x08 (0x00AAD34A), AudioData {start +0x0C, finish +0x10, end +0x14} (0x00AAD4B2, 0x00AAD564).
// The row's out-of-line callee vector<short>(n) 0x00AADD80 is provided here (malloc + zero fill, STLport); vector::operator= and the
// inlined destructor come from the row object.
// Reference model (independent): step = float(SF) / float(rate); step_int = floor(step); step_frac = floor(frac(step) * 65536);
// n = floor(float(double(SL) * rate / SF)); per output sample: pos+1 clamped to SL-1, weights 65536-frac / frac, per channel
// (s[pos*ch+c] * w1 + s[next*ch+c] * w2) >> 16 (signed); frac wraps only when it EXCEEDS 65536.
// Cases sit on every boundary: frac exactly 65536 (no wrap), next exactly SL-1 (no clamp) and SL (clamp), integer and
// non-integer steps with fractions above .5 (floor, not round), negative samples (arithmetic shift), sentinels after SL.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

namespace std {
template<class T> class allocator;
template<class T, class A> class vector { public: T* s; T* f; T* e; vector(unsigned int n); };
template<> vector<short, allocator<short> >::vector(unsigned int n)
{
    s = n ? (short*)malloc(n * sizeof(short)) : 0;
    if (n && !s) { printf("00aad340_TEST FAIL (out of memory)\n"); exit(1); }
    for (unsigned int i = 0; i < n; ++i) s[i] = 0;
    f = s + n; e = s + n;
}
}
class CWaveFile { public: void ConvertSamplingRate(unsigned long rate); };

struct Raw { unsigned long Channels, SampleLength, SamplingFrequency; short* start; short* finish; short* end; };

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

static long Nearest(double x)                 // fistp under RC = nearest, ties to even
{
    double f = floor(x), d = x - f;
    if (d > 0.5 || (d == 0.5 && fmod(f, 2.0) != 0.0)) f += 1.0;
    return (long)f;
}
static long Floor(float v)                    // GFFloatToLongFloor: fistp(v - 0.5), +1 when float(r + 1) has v's bits
{
    long r = Nearest((double)v - 0.5);
    float next = (float)((double)r + 1.0);
    if (memcmp(&next, &v, sizeof v) == 0) ++r;
    return r;
}
static unsigned long g_seed;
static short Gen(unsigned long i, unsigned long live)   // input sample i: live samples, then sentinels a wrong index would read
{
    return (short)(i < live ? (int)((i * 7919u + g_seed) % 20001u) - 10000 : 30000 - (int)(i % 1000u));
}

static void Case(unsigned long ch, unsigned long sl, unsigned long sf, unsigned long rate, unsigned long extra, int k)
{
    float step = (float)sf / (float)rate;
    unsigned long step_int = (unsigned long)Floor(step);
    unsigned long step_frac = (unsigned long)Floor((step - (float)step_int) * 65536.0f);
    unsigned long n = (unsigned long)Floor((float)((double)sl * (double)rate / (double)sf));
    // the stored array covers every index the row may read (pos can run past SampleLength) and is big enough that
    // AudioData = new_data copies in place; the reference is rebuilt from Gen() afterwards
    unsigned long stored = (ch * sl > ch * n ? ch * sl : ch * n) + ch * (step_int + 2) + extra;
    short* in = (short*)malloc(stored * sizeof(short));
    if (!in) { check(false, "malloc", k); return; }
    for (unsigned long i = 0; i < stored; ++i) in[i] = Gen(i, ch * sl);
    Raw w; w.Channels = ch; w.SampleLength = sl; w.SamplingFrequency = sf; w.start = in; w.finish = in + stored; w.end = in + stored;
    ((CWaveFile*)(void*)&w)->ConvertSamplingRate(rate);
    check(w.Channels == ch, "Channels untouched", k);
    check(w.SampleLength == n, "SampleLength = new length", k);
    check(w.SamplingFrequency == rate, "SamplingFrequency = rate", k);
    check((unsigned long)(w.finish - w.start) == ch * n, "AudioData size", k);
    unsigned long pos = 0, frac = 0, o = 0, bad = 0;
    for (unsigned long i = 0; i < n && (unsigned long)(w.finish - w.start) == ch * n; ++i) {
        unsigned long next = pos + 1;
        if (next > sl - 1) next = sl - 1;
        long w1 = 65536 - (long)frac, w2 = (long)frac;
        for (unsigned long c = 0; c < ch; ++c, ++o) {
            long a = Gen(pos * ch + c, ch * sl), b = Gen(next * ch + c, ch * sl);
            long v = a * w1 + b * w2;
            short r = (short)(v >= 0 ? v / 65536 : -((-v + 65535) / 65536));   // floor division = arithmetic >> 16
            if (w.start[o] != r && bad++ < 3) printf("  case %d sample %lu: %d v %d\n", k, o, w.start[o], r);
        }
        pos += step_int; frac += step_frac;
        if (frac > 65536) { frac -= 65536; ++pos; }
    }
    check(bad == 0, "samples", k);
    if (w.start) free(w.start);
}

int main()
{
    Case(1, 12, 5, 4, 12, 1);        // step 1.25: frac reaches exactly 65536 (no wrap), then wraps above it
    Case(1, 10, 5, 4, 14, 2);        // same, last sample's next clamps to SL-1 with sentinels behind
    Case(2, 9, 7, 4, 22, 3);         // step 1.75 (floor 1, frac 0xC000), stereo interleave; n = 5.14 -> 5
    Case(1, 11, 4, 7, 15, 4);        // upsample, step 0.571..: step_int 0, n = 19.25 -> 19
    Case(3, 5, 2, 1, 18, 5);         // integer step 2: frac 0, three channels
    Case(1, 7, 3, 5, 10, 6);         // n = 11.67 -> 11 (floor, not round)
    Case(2, 1, 44100, 22050, 4, 7);  // one sample: n = 0, empty result
    Case(1, 4, 1, 1, 8, 8);          // same rate: copy, next clamps at the last sample
    Case(2, 13, 22050, 44100, 3, 9);
    // step 0.75 over SL = 3m (m = 2^22): the new length 4m = 2^24 and GFFloatToLongFloor(2^24) returns 2^24 + 1 (float(2^24 + 1)
    // has 2^24's bits), so i = 4m is still an iteration; there the fraction is exactly 65536 with pos = 3m - 1 = SL - 1: only
    // `> 65536` keeps pos there (a wrap would read the sentinel s[SL]). No smaller input separates `>` from `>=`.
    Case(1, 3 * 4194304, 3, 4, 0, 10);
    // step 65537/65536: step_frac = 1, so the fraction sits at exactly 65536 (i = 65536, no wrap) and then 65537 (wraps)
    // (seed 2608: samples 65537..65539 rise then fall, so a missed wrap at 65537 changes the floor of sample 65537)
    g_seed = 2608;
    Case(1, 70000, 65537, 65536, 0, 11);
    // (audit) step exactly 2^24 (SF 2^24, rate 1): GFFloatToLongFloor(2^24) = 2^24 + 1, so the second output sample reads
    // s[2^24 + 1]; a truncating step_int would read s[2^24] (SL = 2^25 + 5 makes n = 2, ~100 MB of input)
    g_seed = 0;
    Case(1, 33554437, 16777216, 1, 0, 12);
    // (audit) SL * rate = 4097 * 4097 is exact in double but not in float: the double new length is 4097 (a float product would give 4096)
    Case(1, 4097, 4097, 4097, 4, 13);
    // (audit) rate 2^24 + 1 is not a float: the double product 3 * 16777217 / 2 = 25165825.5 rounds to the float 25165826 (n =
    // 25165826); with rate rounded to a float first the length would be 25165825 (GFFloatToLongFloor's +1 on 25165824)
    Case(1, 3, 2, 16777217, 0, 14);
    printf(failures ? "00aad340_TEST FAIL\n" : "00aad340_TEST PASS\n");
    return failures ? 1 : 0;
}