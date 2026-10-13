#include <cstdio>

struct CClass { int x; };
static CClass g_obj;
CClass* __cdecl MakeIt() { return &g_obj; }
struct Entry { unsigned char pad[0xc]; CClass* (__cdecl* Allocate)(); unsigned char p2[4]; };
struct F { void* vt; Entry* First;
    long GetClassNumberFromName(void* n) { return *(long*)n; }
    CClass* AllocateClass(void* n) { long t = GetClassNumberFromName(n); Entry* e = &First[t]; if (e->Allocate) return e->Allocate(); return 0; } };
int main()
{
    Entry tab[2]; tab[0].Allocate = 0; tab[1].Allocate = MakeIt; F f; f.First = tab; long n0 = 0, n1 = 1;
    if (f.AllocateClass(&n0) != 0 || f.AllocateClass(&n1) != &g_obj) { std::printf("00a371c0_TEST FAIL\n"); return 1; }
    std::printf("00a371c0_TEST PASS\n");
    return 0;
}
