// Behaviour test for CLimiter::Process 0x00C593C0: 88-sample delay line, pass-through saturation, attack (state 1, 44
// samples) and release (state 2, scaled by 32767 / envelope with round-to-nearest), envelope follower as landed 0x00C592F0.
#include <cstdio>
#include <cmath>

struct Limiter {
    long delay[88]; unsigned long writeIndex; float peak, envelope; long holdCount, state; unsigned long releaseCount;
    void UpdateEnvelope()
    {
        long idx = (long)writeIndex - 44; if (idx < 0) idx += 88;
        long s = delay[idx];
        unsigned long mag = s < 0 ? (unsigned long)(-1 - s) : (unsigned long)s;
        float level = (float)mag;
        if (level > peak || holdCount == 44) { peak = level; holdCount = 0; } else ++holdCount;
        if (peak >= envelope) envelope = (peak - envelope) * 0.0586f + envelope + 100.0f;
        else { envelope *= 0.99994761f; if (envelope <= 32767.0f && state == 2) state = 0; }
    }
    static long Round(float v) { return (long)std::floor(v + 0.5f); }
    static short Sat(long v) { return (short)(v > 32767 ? 32767 : (v < -32768 ? -32768 : v)); }
    void Process(short* out, unsigned long stride, const long* src, long shift)
    {
        for (long count = 128; count != 0; --count) {
            long s = *src++ >> shift;
            delay[writeIndex] = s;
            if (++writeIndex == 88) writeIndex = 0;
            long delayed = delay[writeIndex];
            if (state == 0) {
                *out = Sat(delayed);
                unsigned long mag = s < 0 ? (unsigned long)(-1 - s) : (unsigned long)s;
                if (mag > 32767) { state = 1; releaseCount = 0; peak = 0; envelope = 0; holdCount = 0; }
            } else if (state == 1) {
                UpdateEnvelope(); *out = Sat(delayed);
                if (++releaseCount > 44) state = 2;
            } else {
                UpdateEnvelope();
                if (envelope <= 32767.0f) *out = Sat(delayed);
                else { float gain = 32767.0f / envelope; *out = Sat(Round(gain * delayed)); }
            }
            out += stride;
        }
    }
};
int main()
{
    static Limiter L; for (int i = 0; i < 88; ++i) L.delay[i] = 0;
    L.writeIndex = 0; L.peak = 0; L.envelope = 0; L.holdCount = 0; L.state = 0; L.releaseCount = 0;
    long src[128]; short out[256];
    for (int i = 0; i < 128; ++i) src[i] = (i * 1000) << 2;      // shift 2 -> i*1000; exceeds 32767 at i = 33
    for (int i = 0; i < 256; ++i) out[i] = 7;
    L.Process(out, 2, src, 2);
    bool ok = out[0] == 0 && out[2] == 0 && out[1] == 7 && L.writeIndex == 40 && L.state == 2 && L.releaseCount > 44;
    // the first 88 outputs are the zeroed delay line, output 88 is input 0
    ok = ok && out[2 * 87] == 0;
    if (!ok) { std::printf("00c593c0_TEST FAIL\n"); return 1; }
    long loud[128]; for (int i = 0; i < 128; ++i) loud[i] = 40000;
    L.Process(out, 1, loud, 0);
    for (int i = 0; i < 128; ++i) if (out[i] > 32767 || out[i] < -32768) ok = false;
    if (!ok || L.delay[(L.writeIndex + 87) % 88] != 40000) { std::printf("00c593c0_TEST FAIL\n"); return 1; }
    std::printf("00c593c0_TEST PASS\n");
    return 0;
}