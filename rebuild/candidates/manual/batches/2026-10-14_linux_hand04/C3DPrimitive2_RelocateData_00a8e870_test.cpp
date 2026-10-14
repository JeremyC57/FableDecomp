#include <cstdio>
struct P {
    virtual void v00() {}
    unsigned char pad[0x40]; char* sb; unsigned long n; char* ab;
    virtual void Relocate(long, void* o, void* nb, long) {
        if (sb) sb = sb - (char*)o + (char*)nb;
        if (ab) ab = ab - (char*)o + (char*)nb;
    }
};
int main()
{
    char oldb[64], newb[64]; P p; p.sb = oldb + 8; p.ab = 0;
    p.Relocate(1, oldb, newb, 64);
    if (p.sb != newb + 8 || p.ab != 0) { std::printf("00a8e870_TEST FAIL\n"); return 1; }
    p.sb = 0; p.ab = oldb + 16; p.Relocate(1, oldb, newb, 64);
    if (p.sb != 0 || p.ab != newb + 16) { std::printf("00a8e870_TEST FAIL\n"); return 1; }
    std::printf("00a8e870_TEST PASS\n");
    return 0;
}
