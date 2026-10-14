#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_r = 0; struct CDevice { virtual long q() { return 0; } virtual unsigned long a() { return 0; } virtual unsigned long r() { return 0; } virtual long c(void*) { return 0; } virtual long e(void*, void*, unsigned long) { return 0; } virtual long g(void*, void*) { return 0; } virtual long s(void*, void*) { return 0; } virtual long Acquire() { std::strcat(g_seq, "A"); return g_r; } virtual long Unacquire() { std::strcat(g_seq, "U"); return g_r; } };
struct CMouseDX { unsigned char _pad[0x341c - sizeof(void*)]; CDevice* DMouse; long m_3420; bool Acquired; virtual void SetHasInputFocus(bool focus) { if (DMouse) { if (!focus) { if (Acquired == true) { long r = DMouse->Unacquire(); if (r == 0 || r == 1) Acquired = false; } } else { if (!Acquired) { if (DMouse->Acquire() == 0) Acquired = true; } } } } };

int main() {
    CDevice d; CMouseDX* m = new CMouseDX; m->DMouse = &d; m->Acquired = false; g_r = 0; m->SetHasInputFocus(true); if (!m->Acquired || std::strcmp(g_seq, "A")) { std::printf("00ab47d0_TEST FAIL\n"); return 1; } m->SetHasInputFocus(true); if (std::strcmp(g_seq, "A")) { std::printf("00ab47d0_TEST FAIL\n"); return 1; }
    g_r = 5; m->SetHasInputFocus(false); if (!m->Acquired || std::strcmp(g_seq, "AU")) { std::printf("00ab47d0_TEST FAIL\n"); return 1; } m->Acquired = true; g_r = 1; m->SetHasInputFocus(false); if (m->Acquired) { std::printf("00ab47d0_TEST FAIL\n"); return 1; } m->DMouse = 0; m->Acquired = false; m->SetHasInputFocus(true); if (m->Acquired) { std::printf("00ab47d0_TEST FAIL\n"); return 1; } delete m;
    std::printf("00ab47d0_TEST PASS\n"); return 0;
}
