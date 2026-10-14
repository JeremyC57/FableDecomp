#include <cstdio>
static int g_data = 0, g_cloth = 0;
struct BA { int x; };
struct DI { void Get(BA& b) { ++g_data; b.x += 10; } };
struct Cloth { void Get(BA& b) { ++g_cloth; b.x += 1; } };
struct CP { Cloth* o; void* c; };
struct Arr { CP* f; CP* l; CP* e; unsigned long Size() const { return l - f; } CP& operator[](unsigned long i) { return f[i]; } };
struct P {
    virtual void v00() {}
    unsigned char pad[0x1c]; Arr cloth; DI* data;
    void Get(BA& b) { data->Get(b); for (unsigned long i = 0; i < cloth.Size(); ++i) cloth[i].o->Get(b); }
};
int main()
{
    DI d; Cloth c1, c2; CP cps[2] = { { &c1, 0 }, { &c2, 0 } };
    P p; p.data = &d; p.cloth.f = cps; p.cloth.l = cps + 2; p.cloth.e = cps + 2;
    BA b = { 0 }; p.Get(b);
    if (g_data != 1 || g_cloth != 2 || b.x != 12) { std::printf("00a8fcf0_TEST FAIL\n"); return 1; }
    p.cloth.l = cps; p.Get(b); if (g_cloth != 2 || b.x != 22) { std::printf("00a8fcf0_TEST FAIL\n"); return 1; }
    std::printf("00a8fcf0_TEST PASS\n");
    return 0;
}
