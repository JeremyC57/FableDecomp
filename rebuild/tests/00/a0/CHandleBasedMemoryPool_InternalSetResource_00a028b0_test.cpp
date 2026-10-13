#include <cstdio>

struct CHandleRecord { void* Resource; };
struct Pool { unsigned char pad[0x438]; CHandleRecord** Handles; unsigned char rest[0x2c];
    void InternalSetResource(long h, void* r) { Handles[h]->Resource = r; } };
int main()
{
    CHandleRecord r0, r1; CHandleRecord* tab[2] = { &r0, &r1 }; Pool p; p.Handles = tab; int x;
    p.InternalSetResource(1, &x);
    if (r1.Resource != &x) { std::printf("00a028b0_TEST FAIL\n"); return 1; }
    std::printf("00a028b0_TEST PASS\n");
    return 0;
}