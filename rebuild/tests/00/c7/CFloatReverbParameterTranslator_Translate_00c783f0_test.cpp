// Behaviour test for CEffectPart260::Configure (0x00C783F0): every dirty flag runs its stage, stages add the follow-up
// flags (bit 0 -> 0x9106, bit 1 -> bit 2, bit 2 -> 0x9000, 0x20000 -> 0x100, 0x40000 -> 0x8000), the derived values are
// written and the flag word is cleared; a zero flag word does nothing.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void mark(const char* s) { std::strcat(g_log, s); std::strcat(g_log, " "); }
struct Params { unsigned long Flags; long Level; float PeakFrequency, ShelfFrequency, PeakGain, ShelfGain, RateScale; };
struct Owner { float ShelfGain, PeakGain, RateStep; };
static unsigned long g_rate = 22050;

static void Configure(Owner* owner, Params* p)
{
    if (p->Flags == 0) return;
    if (p->Flags & 1) { mark("150"); p->Flags |= 0x9106; }
    if (p->Flags & 2) { unsigned long f = p->Flags | 4; if (f == 0) mark("dead"); else p->Flags = f; }
    if (p->Flags & 4) { mark("0c0"); p->Flags |= 0x9000; }
    if (p->Flags & 0x48) { owner->PeakGain = 1.0f; mark("peak"); }
    if (p->Flags & 0x90) mark("shelf");
    if (p->Flags & 0x20000) { mark("fc0"); p->Flags |= 0x100; }
    if (p->Flags & 0x40000) p->Flags |= 0x8000;
    if (p->Flags & 0x718) mark("800");
    if (p->Flags & 0x80000) owner->RateStep = (float)g_rate * p->RateScale;
    if (p->Flags & 0x180000) mark("040");
    if (p->Flags & 0x10000) mark("c80");
    if (p->Flags & 0x12000) { mark("b30"); mark("c80"); }
    if (p->Flags & 0x800) mark("mix");
    if (p->Flags & 0x1800) mark("ae0");
    if (p->Flags & 0x4000) mark("3b0");
    if (p->Flags & 0xc000) mark("c00");
    if (p->Flags & 0x20) owner->ShelfGain = p->Level <= -10000 ? 0.0f : 0.5f;
    p->Flags = 0;
}

int main()
{
    Owner o = { 0.0f, 0.0f, 0.0f };
    Params p; std::memset(&p, 0, sizeof(p));
    bool ok = true;
    g_log[0] = 0; Configure(&o, &p); ok = ok && g_log[0] == 0;
    p.Flags = 2; g_log[0] = 0; Configure(&o, &p);
    ok = ok && std::strcmp(g_log, "0c0 ae0 c00 ") == 0 && p.Flags == 0;
    p.Flags = 1; g_log[0] = 0; Configure(&o, &p);
    ok = ok && std::strcmp(g_log, "150 0c0 800 ae0 c00 ") == 0;
    p.Flags = 0x80000 | 0x20 | 0x40; p.RateScale = 2.0f; p.Level = -20000; o.ShelfGain = 3.0f; g_log[0] = 0; Configure(&o, &p);
    ok = ok && o.RateStep == 44100.0f && o.ShelfGain == 0.0f && o.PeakGain == 1.0f && std::strcmp(g_log, "peak 040 ") == 0;
    p.Flags = 0x40000; g_log[0] = 0; Configure(&o, &p);
    ok = ok && std::strcmp(g_log, "c00 ") == 0;
    std::printf(ok ? "00c783f0_TEST PASS\n" : "00c783f0_TEST FAIL\n");
    return 0;
}