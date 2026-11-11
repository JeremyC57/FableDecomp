// Behaviour test for CTCFlammable::SetOnFire @ 0x00780410: every fire point's Resistance (+0xc of 0x2c) is zeroed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CFirePoint { unsigned char _pad[0xc]; long Resistance; unsigned char _p[0x2c - 0xc - 4]; }; struct CTCFlammable { unsigned char _pad[0x10]; CFirePoint* FirePointsStart; CFirePoint* FirePointsFinish; CFirePoint* FirePointsEnd; void SetOnFire() { for (CFirePoint* e = FirePointsStart; e != FirePointsFinish; ++e) e->Resistance = 0; } };

int main() {
    CFirePoint e[3]; for (int i = 0; i < 3; ++i) e[i].Resistance = 7; CTCFlammable* m = new CTCFlammable; m->FirePointsStart = e; m->FirePointsFinish = e + 3; m->SetOnFire(); if (e[0].Resistance || e[1].Resistance || e[2].Resistance) { std::printf("00780410_TEST FAIL\n"); return 1; } e[0].Resistance = 7; m->FirePointsFinish = e; m->SetOnFire(); if (e[0].Resistance != 7) { std::printf("00780410_TEST FAIL\n"); return 1; } delete m;
    std::printf("00780410_TEST PASS\n"); return 0;
}
