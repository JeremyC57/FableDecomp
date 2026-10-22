// Behaviour test for CMouseDX::GetExclusiveModeDeviceMouseEvent (0x00AB4910): buffered DirectInput records -> mouse events.
#include <cstdio>

struct Rec { unsigned int dwOfs, dwData, dwTimeStamp, dwSequence, uAppData; };
struct Ev { double Time; int Type; float Movement; };

struct Dev {
    Rec src[4]; unsigned int n; int lostOnce; int acquires;
    int GetDeviceData(Rec* out, unsigned int* inOut)
    {
        if (lostOnce) { lostOnce = 0; return (int)0x8007001E; }
        unsigned int i;
        for (i = 0; i < n && i < *inOut; ++i) out[i] = src[i];
        *inOut = i;
        return 0;
    }
};

struct Mouse {
    Dev* DMouse; bool Acquired; bool active; float MouseStep;
    Rec data[256]; int index, count;
    void SetHasInputFocus(bool f) { Acquired = f; }
    bool Get(Ev* e)
    {
        if (index >= count) {
            if (active) SetHasInputFocus(true); else SetHasInputFocus(false);
            if (Acquired == true && DMouse) {
                unsigned int c = 256;
                int hr = DMouse->GetDeviceData(data, &c);
                if (hr == (int)0x8007001E) { DMouse->acquires++; hr = DMouse->GetDeviceData(data, &c); }
                if (hr >= 0) { index = 0; count = (int)c; }
            }
        }
        if (index < count) {
            Rec* d = &data[index++];
            switch (d->dwOfs) {
            case 0: e->Type = 7; e->Movement = (float)(int)((int)d->dwData * MouseStep); break;
            case 4: e->Type = 8; e->Movement = (float)(int)((int)d->dwData * MouseStep); break;
            case 8: e->Type = 10; e->Movement = (float)(int)d->dwData; break;
            case 12: e->Type = ((unsigned char)d->dwData & 0x80) ? 1 : 4; break;
            case 13: e->Type = ((unsigned char)d->dwData & 0x80) ? 2 : 5; break;
            case 14: e->Type = ((unsigned char)d->dwData & 0x80) ? 3 : 6; break;
            case 19: e->Type = ((unsigned char)d->dwData & 0x80) ? 19 : 24; break;
            default: return false;
            }
            e->Time = d->dwTimeStamp * 0.001f;
            return true;
        }
        return false;
    }
};

static Mouse g_m;

int main()
{
    Dev dev;
    dev.n = 4; dev.lostOnce = 1; dev.acquires = 0;
    dev.src[0].dwOfs = 0;  dev.src[0].dwData = (unsigned int)-3; dev.src[0].dwTimeStamp = 2000;
    dev.src[1].dwOfs = 12; dev.src[1].dwData = 0x80;             dev.src[1].dwTimeStamp = 3000;
    dev.src[2].dwOfs = 19; dev.src[2].dwData = 0x00;             dev.src[2].dwTimeStamp = 4000;
    dev.src[3].dwOfs = 40; dev.src[3].dwData = 0;                dev.src[3].dwTimeStamp = 5000;
    g_m.DMouse = &dev; g_m.active = true; g_m.MouseStep = 2.0f; g_m.index = 0; g_m.count = 0;
    Ev e;
    bool ok = true;
    if (!g_m.Get(&e) || e.Type != 7 || e.Movement != -6.0f || e.Time < 1.999 || e.Time > 2.001 || dev.acquires != 1) ok = false;
    if (!g_m.Get(&e) || e.Type != 1) ok = false;
    if (!g_m.Get(&e) || e.Type != 24) ok = false;
    if (g_m.Get(&e)) ok = false;                 // unknown offset
    if (!g_m.Get(&e) || e.Type != 7) ok = false; // buffer drained -> refilled from the device
    g_m.active = false; g_m.index = g_m.count;
    if (g_m.Get(&e) || g_m.Acquired) ok = false; // not acquired -> nothing
    if (!ok) { std::printf("00ab4910_TEST FAIL\n"); return 1; }
    std::printf("00ab4910_TEST PASS\n");
    return 0;
}
