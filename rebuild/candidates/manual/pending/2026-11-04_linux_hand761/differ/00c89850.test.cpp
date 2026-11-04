// Behaviour test for CReverbFilterTap::ProcessCrossfade (0x00C89850): with `remaining` of 512 samples left in the fade, the
// old parameter set is weighted remaining/512 and the new one 1 - remaining/512 at the block start, both moving linearly
// over the block; the output level ramps between the weighted output gains; when the block reaches the fade end the
// target parameters are adopted.
#include <stdio.h>
#include <math.h>

struct Params { float historyGain[2]; float outputGain; int id; };
struct Ramp { float start; float step; };

static Ramp HistoryRamp(float gain, float startWeight, float endWeight, unsigned int count)
{
    Ramp r;
    r.start = gain * startWeight;
    r.step = (endWeight - startWeight) * gain / (float)count;
    return r;
}

static void Crossfade(Params* current, const Params* target, unsigned int count, unsigned int remaining,
                      Ramp oldTaps[2], Ramp newTaps[2], Ramp* level)
{
    float fade = (float)remaining;
    float startOld = fade * (1.0f / 512);
    float startNew = 1.0f - startOld;
    float endOld = (fade - (float)count) * (1.0f / 512);
    float endNew = 1.0f - endOld;
    for (unsigned int i = 0; i < 2; ++i) {
        oldTaps[i] = HistoryRamp(current->historyGain[i], startOld, endOld, count);
        newTaps[i] = HistoryRamp(target->historyGain[i], startNew, endNew, count);
    }
    float levelStart = startNew * target->outputGain + startOld * current->outputGain;
    float levelEnd = endNew * target->outputGain + endOld * current->outputGain;
    level->start = levelStart;
    level->step = (levelEnd - levelStart) / (float)count;
    if (count >= remaining)
        *current = *target;
}

static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }

int main()
{
    Params cur = { { 1.0f, 0.5f }, 2.0f, 1 };
    Params tgt = { { 0.0f, 1.0f }, 4.0f, 2 };
    Ramp o[2], n[2], lv;
    Crossfade(&cur, &tgt, 128, 512, o, n, &lv);                  // first block of a full fade
    bool ok = Near(o[0].start, 1.0f) && Near(o[0].step, -0.25f / 128) && Near(n[1].start, 0.0f) && Near(n[1].step, 0.25f / 128)
        && Near(lv.start, 2.0f) && Near(lv.start + lv.step * 128, 0.75f * 2.0f + 0.25f * 4.0f) && cur.id == 1;
    Crossfade(&cur, &tgt, 128, 128, o, n, &lv);                  // last block: ends fully on the target, adopts it
    ok = ok && Near(lv.start + lv.step * 128, 4.0f) && cur.id == 2 && Near(cur.outputGain, 4.0f);
    printf(ok ? "00c89850_TEST PASS\n" : "00c89850_TEST FAIL\n");
    return ok ? 0 : 1;
}
