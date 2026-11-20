// Behaviour test for CRampedParam::ApplyGainToFrame (0x00C3A7D0): links the row object. CRampedParam::Update and CCodaFrame::GetData are
// /alternatename stand-ins that log; __ftol2 is the CRT's. A frame with no valid samples is left alone (no call). Otherwise the gain is
// the parameter's value (+0x00), wrapped once into [-pi, pi) when the angle flag (+0x30) is set, clamped to [0, 1]; Update(this,
// samples * 1000 / rate) runs once, before GetData; then every sample of channels x samples is scaled by (unsigned short)(long)(gain *
// 32767) / 32767 (signed divide, truncating). Nothing past the frame's samples and nothing in the parameter changes.
#include <stdio.h>
#include <string.h>
#include <math.h>
extern "C" void __fastcall RowApply(void* self, int unused, void* frame, unsigned long rate);
#pragma comment(linker, "/alternatename:@RowApply@16=?ApplyGainToFrame@CRampedParam@@QAEXAAVCCodaFrame@@K@Z")
static int g_updates, g_gets, g_order; static void* g_update_this; static float g_dt; static void* g_get_this; static short* g_data;
extern "C" void __fastcall StubUpdate(void* self, int, float dt) { ++g_updates; g_update_this = self; g_dt = dt; g_order = g_gets ? 2 : 1; }
extern "C" short* __fastcall StubGetData(void* self, int) { ++g_gets; g_get_this = self; return g_data; }
#pragma comment(linker, "/alternatename:?Update@CRampedParam@@QAEXM@Z=@StubUpdate@12")
#pragma comment(linker, "/alternatename:?GetData@CCodaFrame@@QAEPAFXZ=@StubGetData@8")

struct Param { float value, target, r[10]; unsigned char angle, pad[3]; };
struct Frame { unsigned long channels, capacity, samples; void* buffer; short* data; };

static bool Run(const char* name, float value, bool angle, unsigned long channels, unsigned long samples, unsigned long rate)
{
    Param p; memset(&p, 0x11, sizeof p); p.value = value; p.angle = angle ? 1 : 0;
    short data[64], orig[64]; for (int i = 0; i < 64; ++i) data[i] = orig[i] = (short)((i * 7919) % 65536 - 32768 + (i & 1 ? 1000 : -1000));
    Frame f = { channels, 99, samples, (void*)0x1234, (short*)0x5678 };
    Param p0 = p; Frame f0 = f;
    g_updates = g_gets = g_order = 0; g_update_this = g_get_this = 0; g_dt = -1; g_data = data;
    RowApply(&p, 0, &f, rate);
    bool ok = !memcmp(&p0, &p, sizeof p) && !memcmp(&f0, &f, sizeof f);
    if (!samples) {
        ok = ok && g_updates == 0 && g_gets == 0 && !memcmp(orig, data, sizeof data);
    } else {
        float gain = value;
        if (angle) { if (gain >= 3.14159265f) gain -= 6.28318531f; else if (gain < -3.14159265f) gain += 6.28318531f; }
        if (gain < 0.0f) gain = 0.0f; else if (gain > 1.0f) gain = 1.0f;
        double want_dt = (double)samples * 1000.0 / (double)rate;
        long scale = (long)(gain * 32767.0f);
        ok = ok && g_updates == 1 && g_update_this == &p && g_gets == 1 && g_get_this == &f && g_order == 1
             && fabs(g_dt - want_dt) <= want_dt * 1e-6;
        for (unsigned long i = 0; i < 64; ++i) {
            short want = orig[i];
            if (i < channels * samples) want = (short)(orig[i] * (unsigned short)scale / 32767);
            if (data[i] != want) { ok = false; printf("   sample %lu: %d want %d\n", i, data[i], want); break; }
        }
    }
    printf("%-34s dt %8.4f %s\n", name, g_dt, ok ? "ok" : "FAIL");
    return ok;
}

int main()
{
    bool ok = true;
    ok &= Run("no valid samples", 0.5f, false, 2, 0, 44100);
    ok &= Run("half gain, stereo", 0.5f, false, 2, 16, 44100);
    ok &= Run("negative clamps to 0", -0.25f, false, 2, 8, 22050);
    ok &= Run("above 1 clamps to 1", 1.5f, false, 1, 20, 48000);
    ok &= Run("exactly 1", 1.0f, false, 3, 7, 11025);
    ok &= Run("exactly 0", 0.0f, false, 1, 5, 44100);
    ok &= Run("0.3, mono", 0.3f, false, 1, 31, 32000);
    ok &= Run("0.1 (just above 0)", 0.1f, false, 2, 10, 44100);
    ok &= Run("angle 4.0 wraps negative -> 0", 4.0f, true, 2, 4, 44100);
    ok &= Run("angle -4.0 wraps positive -> 1", -4.0f, true, 2, 4, 44100);
    ok &= Run("angle 0.75 unchanged", 0.75f, true, 4, 6, 8000);
    ok &= Run("no angle 4.0 -> 1", 4.0f, false, 1, 9, 44100);
    ok &= Run("441 samples at 44100 = 10 ms", 0.9f, false, 0, 441, 44100);
    printf(ok ? "00c3a7d0_TEST PASS\n" : "00c3a7d0_TEST FAIL\n");
    return ok ? 0 : 1;
}