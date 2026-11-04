// Behaviour test for 0x00C71F80 CAudioSource::ComputePitchScale: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
static float Pitch(long long start, long long period, long long pos, float depth, unsigned long& flags)
{
    unsigned long state = flags & 0x180; float scale = 1.0f;
    if (state == 0 && start > pos) { flags = state | (flags & ~0x180u); return scale; }
    if (state == 0) state = 0x80;
    long long phase = (period / 4 - start + pos) % period; float wave;
    if (phase < period / 2) wave = phase * 4.0f / period - 1.0f; else { phase -= period / 2; wave = 1.0f - phase * 4.0f / period; }
    scale = (float)std::pow(2.0, (double)(wave * depth));
    flags = state | (flags & ~0x180u); return scale;
}
int main() {
    unsigned long f = 0; float s = Pitch(0, 100, 0, 1.0f, f);
    int ok = f == 0x80 && std::fabs(s - 1.0f) < 1e-6f;
    s = Pitch(0, 100, 25, 1.0f, f); ok &= std::fabs(s - 2.0f) < 1e-5f;
    unsigned long g = 0; Pitch(50, 100, 0, 1.0f, g); ok &= g == 0;
    std::printf(ok ? "00c71f80_TEST PASS\n" : "00c71f80_TEST FAIL\n");
    return ok ? 0 : 1;
}
