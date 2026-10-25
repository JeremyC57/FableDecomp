// 00c69c60 behaviour test: CReverbProcessor::ProcessInput -- an input that just switched off fades the last
// min(count, pre-delay) samples of the delay line (ramp 1 -> 0, wrapping backwards over the start) and resets both
// gains; the block is written through the input gain (with the `active` mode) and the in-place line gain (mode 1), as a
// cross-fade with the blend position while this stage's element is being blended, split into the tail and head runs
// when it wraps; an input that just switched on fades the new block in (ramp 0 -> 1); the write position advances and wraps.
#include <cstdio>
#include <cmath>

struct Call { int gain; bool blend; long src; long dst; bool mode; unsigned long count; unsigned long blendPos; };

static Call g_calls[16];
static int g_numCalls = 0;
static int g_resets = 0;

struct Line { float data[16]; unsigned long length; unsigned long pos; };

static void ScaleRamp(float* buffer, float gain, float step, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) { buffer[i] *= gain; gain += step; }
}

struct Reverb {
    Line line; const float* inputBase; bool blending; long blendElement; long inputElement; unsigned long blendPosition;
    unsigned long preDelay; bool wasActive;

    void Record(int gain, bool blend, const float* src, const float* dst, bool mode, unsigned long count, unsigned long bp)
    {
        Call c;
        c.gain = gain; c.blend = blend; c.mode = mode; c.count = count; c.blendPos = bp;
        c.src = (src >= line.data && src < line.data + 16) ? 100 + (long)(src - line.data) : (long)(src - inputBase);
        c.dst = (long)(dst - line.data);
        g_calls[g_numCalls++] = c;
    }
    void Process(const float* input, bool active, unsigned long count)
    {
        if (!active && wasActive) {
            unsigned long fade = count;
            if (fade > preDelay) fade = preDelay;
            if (fade != 0) {
                unsigned long pos = line.pos;
                unsigned long start = pos >= fade ? pos - fade : line.length - fade + pos;
                unsigned long first = fade;
                if (start + fade > line.length) first = line.length - start;
                float step = -1.0f / (float)fade;
                ScaleRamp(&line.data[start], 1.0f, step, first);
                if (first < fade) ScaleRamp(line.data, 1.0f - (float)first / (float)fade, step, fade - first);
            }
            g_resets += 2;
        }
        bool blend = blending && blendElement == inputElement;
        unsigned long first = count;
        if (line.pos + count > line.length) first = line.length - line.pos;
        Record(0, blend, input, &line.data[line.pos], active, first, blendPosition);
        Record(1, blend, &line.data[line.pos], &line.data[line.pos], true, first, blendPosition);
        if (first < count) {
            Record(0, blend, input + first, line.data, active, count - first, blendPosition - first);
            Record(1, blend, line.data, line.data, true, count - first, blendPosition - first);
        }
        if (active && !wasActive) {
            float step = 1.0f / (float)count;
            ScaleRamp(&line.data[line.pos], 0.0f, step, first);
            if (first < count) ScaleRamp(line.data, (float)first / (float)count, step, count - first);
        }
        line.pos += count;
        if (line.pos >= line.length) line.pos -= line.length;
    }
};

static Reverb Make(const float* input, unsigned long pos, bool wasActive)
{
    Reverb r;
    for (int i = 0; i < 16; ++i) r.line.data[i] = 1.0f;
    r.line.length = 16; r.line.pos = pos; r.inputBase = input; r.blending = false; r.blendElement = 3; r.inputElement = 3;
    r.blendPosition = 100; r.preDelay = 4; r.wasActive = wasActive;
    g_numCalls = 0; g_resets = 0;
    return r;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static bool Is(const Call& c, int gain, bool blend, long src, long dst, bool mode, unsigned long count, unsigned long bp)
{
    return c.gain == gain && c.blend == blend && c.src == src && c.dst == dst && c.mode == mode && c.count == count &&
           (!blend || c.blendPos == bp);
}

int main()
{
    float input[16] = { 0 };
    int fails = 0;
    {   // steady active block, no wrap: input gain then line gain in place, position advances
        Reverb r = Make(input, 2, true);
        r.Process(input, true, 8);
        if (g_numCalls != 2 || !Is(g_calls[0], 0, false, 0, 2, true, 8, 0) || !Is(g_calls[1], 1, false, 102, 2, true, 8, 0)) ++fails;
        if (r.line.pos != 10 || g_resets != 0 || !Near(r.line.data[2], 1.0f)) ++fails;
    }
    {   // wrapping block with an active blend on this stage: four cross-fades, the head run's blend position moved back
        Reverb r = Make(input, 12, true);
        r.blending = true;
        r.Process(input, true, 8);
        if (g_numCalls != 4) ++fails;
        else if (!Is(g_calls[0], 0, true, 0, 12, true, 4, 100) || !Is(g_calls[1], 1, true, 112, 12, true, 4, 100) ||
                 !Is(g_calls[2], 0, true, 4, 0, true, 4, 96) || !Is(g_calls[3], 1, true, 100, 0, true, 4, 96)) ++fails;
        if (r.line.pos != 4) ++fails;
    }
    {   // blend on another element: plain gains
        Reverb r = Make(input, 0, true);
        r.blending = true; r.blendElement = 1;
        r.Process(input, true, 4);
        if (g_numCalls != 2 || g_calls[0].blend || g_calls[1].blend) ++fails;
    }
    {   // switched off: the last 4 samples before the write position (wrapping) ramp 1, .75, .5, .25; gains reset; mode 0
        Reverb r = Make(input, 2, true);
        r.Process(input, false, 8);
        if (!Near(r.line.data[14], 1.0f) || !Near(r.line.data[15], 0.75f) || !Near(r.line.data[0], 0.5f) || !Near(r.line.data[1], 0.25f)) ++fails;
        if (!Near(r.line.data[13], 1.0f) || !Near(r.line.data[2], 1.0f) || g_resets != 2) ++fails;
        if (g_numCalls != 2 || g_calls[0].mode || !g_calls[1].mode) ++fails;
    }
    {   // switched on, wrapping: the new block ramps 0, .25, .5, .75 across the end of the line
        Reverb r = Make(input, 14, false);
        r.Process(input, true, 4);
        if (!Near(r.line.data[14], 0.0f) || !Near(r.line.data[15], 0.25f) || !Near(r.line.data[0], 0.5f) || !Near(r.line.data[1], 0.75f)) ++fails;
        if (!Near(r.line.data[2], 1.0f) || r.line.pos != 2 || g_resets != 0) ++fails;
    }
    {   // staying off: no ramps, the gains still run with mode 0
        Reverb r = Make(input, 5, false);
        r.Process(input, false, 4);
        if (!Near(r.line.data[5], 1.0f) || !Near(r.line.data[4], 1.0f) || g_resets != 0 || g_numCalls != 2 || g_calls[0].mode) ++fails;
        if (r.line.pos != 9) ++fails;
    }
    if (fails) { std::printf("00c69c60_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("00c69c60_TEST PASS\n");
    return 0;
}