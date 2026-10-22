#include <cstdio>
#include <cmath>
// Behaviour model of CKeyboardDX::UpdateKeyEvents (0x00AB6240): buffered DirectInput records become pressed / released events;
// a release takes its start time from the matching held maintained event; unknown DX keys map to key 0; a lost device is
// re-acquired once.
struct Rec { unsigned int ofs, data, stamp; };
struct Ev { int pressed; int key; double start, time; };
struct Held { int type; int key; float start; };
struct Kbd {
    unsigned short map[0x7a];
    Held held[4]; int heldCount;
    Rec recs[8]; int recCount; int lostOnce; int acquires;
    Ev out[8]; int outCount;
};
static int Convert(const Kbd& k, unsigned int dx) { for (int i = 0; i < 0x7a; ++i) if (k.map[i] == dx) return i; return 0; }
static int Read(Kbd& k) { if (k.lostOnce) { k.lostOnce = 0; return -1; } return 0; }
static bool Update(Kbd& k)
{
    k.outCount = 0;
    int r = Read(k);
    if (r == -1) { ++k.acquires; r = Read(k); }
    if (r >= 0) {
        for (int i = 0; i < k.recCount; ++i) {
            int key = Convert(k, k.recs[i].ofs);
            float now = (float)k.recs[i].stamp * 0.001f;
            Ev e; e.key = key; e.time = now;
            if (k.recs[i].data & 0x80) { e.pressed = 1; e.start = now; }
            else {
                e.pressed = 0; e.start = now;
                for (int h = 0; h < k.heldCount; ++h)
                    if (k.held[h].type == 2 && k.held[h].key == key) { e.start = k.held[h].start; break; }
            }
            k.out[k.outCount++] = e;
        }
    }
    return true;
}
int main()
{
    static Kbd k;
    for (int i = 0; i < 0x7a; ++i) k.map[i] = (unsigned short)(i + 0x100);
    k.map[5] = 0x1e; k.map[9] = 0x30;
    k.heldCount = 2; k.held[0].type = 1; k.held[0].key = 9; k.held[0].start = 7.0f; k.held[1].type = 2; k.held[1].key = 9; k.held[1].start = 1.5f;
    k.recCount = 3;
    k.recs[0].ofs = 0x1e; k.recs[0].data = 0x80; k.recs[0].stamp = 2000;
    k.recs[1].ofs = 0x30; k.recs[1].data = 0x00; k.recs[1].stamp = 3000;
    k.recs[2].ofs = 0x99; k.recs[2].data = 0x00; k.recs[2].stamp = 4000;
    k.lostOnce = 1;
    int ok = Update(k) && k.acquires == 1 && k.outCount == 3;
    ok = ok && k.out[0].pressed == 1 && k.out[0].key == 5 && std::fabs(k.out[0].time - 2.0) < 1e-4;
    ok = ok && k.out[1].pressed == 0 && k.out[1].key == 9 && std::fabs(k.out[1].start - 1.5) < 1e-6 && std::fabs(k.out[1].time - 3.0) < 1e-4;
    ok = ok && k.out[2].pressed == 0 && k.out[2].key == 0 && std::fabs(k.out[2].start - 4.0) < 1e-4;
    if (!ok) { std::printf("00ab6240_TEST FAIL\n"); return 1; }
    std::printf("00ab6240_TEST PASS\n");
    return 0;
}
