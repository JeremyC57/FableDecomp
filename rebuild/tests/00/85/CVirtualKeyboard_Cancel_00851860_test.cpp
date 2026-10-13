#include <cstdio>

struct IME { bool active; int fin; bool IsIMEActive() { return active; } void FinaliseString(bool) { ++fin; } };
static IME g_ime;
IME* GFGetIME() { return &g_ime; }
struct K { void* s; bool Canceled; bool Done;
    void Cancel() { if (!Done) { Done = true; Canceled = true; if (GFGetIME()->IsIMEActive()) GFGetIME()->FinaliseString(false); } } };
int main()
{
    g_ime.active = true; g_ime.fin = 0; K k; k.Done = false; k.Canceled = false; k.Cancel();
    if (!k.Done || !k.Canceled || g_ime.fin != 1) { std::printf("00851860_TEST FAIL\n"); return 1; }
    k.Cancel(); if (g_ime.fin != 1) { std::printf("00851860_TEST FAIL\n"); return 1; }
    std::printf("00851860_TEST PASS\n");
    return 0;
}