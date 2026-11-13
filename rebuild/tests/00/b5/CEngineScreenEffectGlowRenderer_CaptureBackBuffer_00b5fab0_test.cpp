// Behaviour test for 0x00B5FAB0 CEngineScreenEffectGlowRenderer::CaptureBackBuffer (standalone copy of the model's logic).
// Exercises: the four render-target passes and their source textures, the glow / negative-glow shader choice and threshold
// constant, the motion-blur alpha-blend branch, the NoBlurPasses ping-pong with tap offsets growing by BlurOffsetScale, and the
// inline SetRenderState bookmark logic (desired_state / bookmark_mask / dirty: the state is bookmarked once per capture block and
// queued once for RealiseRenderState).
#include <stdio.h>
#include <math.h>

struct StateInfo { unsigned long CurrentState, DesiredState, BookmarkMask; bool DirtyListFlag; };
struct StackElement { StateInfo* State; unsigned long Value; };
struct StateManager {
    StackElement StateStack[64]; StateInfo* StateUpdateList[64];
    long StateStackSize, StateUpdateListSize; unsigned long CurrentBookmarkMask; StateInfo Dummy;
    StateInfo AlphaBlendEnable;
    void AddStateToUpdateList(StateInfo& state) {
        const bool dirty = state.DirtyListFlag;
        if (!dirty) { state.DirtyListFlag = true; StateUpdateList[StateUpdateListSize] = &state; ++StateUpdateListSize; }
    }
    void SetRenderState(unsigned long value, StateInfo& state) {
        const unsigned long desired_state = state.DesiredState;
        if (desired_state != value) {
            const unsigned long bookmark_mask = state.BookmarkMask;
            if ((bookmark_mask & CurrentBookmarkMask) == 0) {
                state.BookmarkMask |= CurrentBookmarkMask;
                StateStack[StateStackSize].State = &state;
                StateStack[StateStackSize].Value = state.DesiredState;
                ++StateStackSize;
            }
            state.DesiredState = value;
            AddStateToUpdateList(state);
        }
    }
    void SetAlphaBlendEnable(bool enable) { SetRenderState(enable ? 1 : 0, AlphaBlendEnable); }
    void StartCaptureBlock() { StateStack[StateStackSize++].State = &Dummy; CurrentBookmarkMask <<= 1; }
};

struct Pass { int target; int source; float c[4][2]; };
static Pass passes[16]; static int npasses;
enum { T256 = 1, T64, T64ALT, T16, BACK };

static int Capture(StateManager& sm, float thresholdR, float motionBlur, long noBlurPasses, float blurOffsetScale,
                   int* glowNeg, float* thresholdOut)
{
    npasses = 0;
    sm.StartCaptureBlock();
    sm.SetAlphaBlendEnable(motionBlur > 0.0f && motionBlur < 1.0f);
    *glowNeg = thresholdR < 1.0f;
    *thresholdOut = thresholdR < 1.0f ? thresholdR : thresholdR - 1.0f;
    Pass p0 = { T256, BACK, { { 0, 0 } } }; passes[npasses++] = p0;
    sm.SetAlphaBlendEnable(false);
    const float t = 1.0f / 128.0f;
    Pass p1 = { T64, T256, { { 0, 0 }, { 0, t }, { t, 0 }, { t, t } } }; passes[npasses++] = p1;
    float offsetX = t, offsetY = t;
    for (long i = 0; i < noBlurPasses; ++i) {
        sm.SetAlphaBlendEnable(false);
        {
            const float origin = -1.0f / 128.0f;
            float left = origin - offsetX, right = offsetX - 1.0f / 128.0f, top = origin - offsetY, bottom = offsetY - 1.0f / 128.0f;
            Pass a = { T64ALT, T64, { { origin, origin }, { left, top }, { right, top }, { origin, bottom } } }; passes[npasses++] = a;
        }
        offsetX = blurOffsetScale * offsetX; offsetY = blurOffsetScale * offsetY;
        sm.SetAlphaBlendEnable(false);
        {
            const float origin = -1.0f / 128.0f;
            float left = origin - offsetX, right = offsetX - 1.0f / 128.0f, top = origin - offsetY, bottom = offsetY - 1.0f / 128.0f;
            Pass b = { T64, T64ALT, { { origin, origin }, { origin, top }, { left, bottom }, { right, bottom } } }; passes[npasses++] = b;
        }
        offsetX = blurOffsetScale * offsetX; offsetY = blurOffsetScale * offsetY;
    }
    const float q = 1.0f / 32.0f;
    Pass p3 = { T16, T64, { { 0, 0 }, { 0, q }, { q, 0 }, { q, q } } }; passes[npasses++] = p3;
    return npasses;
}

static bool near(float a, float b) { return fabs(a - b) < 1e-6f; }

int main()
{
    int fails = 0;
    StateManager sm = StateManager();
    sm.CurrentBookmarkMask = 1;
    int neg = 0; float thr = 0.0f;
    int n = Capture(sm, 1.25f, 0.5f, 2, 1.5f, &neg, &thr);
    if (n != 7) ++fails;                                            // pass 1, pass 2, 2 x (A, B), last
    if (neg != 0 || !near(thr, 0.25f)) ++fails;                     // ThresholdR >= 1: glow shader, R - 1
    if (passes[2].target != T64ALT || passes[2].source != T64 || passes[3].target != T64 || passes[3].source != T64ALT) ++fails;
    if (passes[6].target != T16 || passes[6].source != T64) ++fails;
    // first ping: offset 1/128 -> left = -2/128; second half: offset 1.5/128 -> bottom = 0.5/128
    if (!near(passes[2].c[1][0], -2.0f / 128.0f) || !near(passes[3].c[3][1], 0.5f / 128.0f)) ++fails;
    // second iteration: offset 2.25/128 then 3.375/128
    if (!near(passes[4].c[2][0], 1.25f / 128.0f) || !near(passes[5].c[2][1], 2.375f / 128.0f)) ++fails;
    // bookmark: the blend state changed (0 -> 1 -> 0) inside one capture block: pushed once, queued once
    if (sm.StateStackSize != 2 || sm.StateStack[1].State != &sm.AlphaBlendEnable || sm.StateStack[1].Value != 0) ++fails;
    if (sm.StateUpdateListSize != 1 || sm.AlphaBlendEnable.DesiredState != 0) ++fails;
    // negative-glow arm, no motion blur, no blur passes
    StateManager sm2 = StateManager(); sm2.CurrentBookmarkMask = 1;
    n = Capture(sm2, 0.75f, 0.0f, 0, 1.5f, &neg, &thr);
    if (n != 3 || neg != 1 || !near(thr, 0.75f)) ++fails;
    if (sm2.StateStackSize != 1 || sm2.StateUpdateListSize != 0) ++fails;   // blend stays 0: only the capture marker
    if (fails) { printf("00b5fab0_TEST FAIL\n"); return 1; }
    printf("00b5fab0_TEST PASS\n");
    return 0;
}