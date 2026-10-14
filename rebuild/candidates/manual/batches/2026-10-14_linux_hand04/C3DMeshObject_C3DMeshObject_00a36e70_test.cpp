#include <cstdio>
struct V { float X, Y, Z; };
struct R { V a, b; };
static int g_order[2]; static int g_n = 0;
struct CP { void* a; void* b; CP() : a(0), b(0) {} };
struct Base { virtual void v00() {} Base() {} };
struct M : Base {
    virtual void v00() {}
    void* info; CP mesh; float pm[12]; bool inv; float ipm[12]; float scale;
    M(void* i, const V& p, const R& r, float s) : Base(), mesh(), inv(false), scale(s) { SetMesh(i); SetPos(p, r, s); }
    void SetMesh(void* i) { info = i; g_order[g_n++] = 1; }
    void SetPos(const V& p, const R&, float s) { pm[0] = p.X * s; g_order[g_n++] = 2; }
};
int main()
{
    V p = { 2.0f, 0, 0 }; R r = { { 0, 0, 1 }, { 0, 1, 0 } }; int tag = 7;
    M m(&tag, p, r, 3.0f);
    if (m.info != &tag || m.mesh.a || m.mesh.b || m.inv || m.scale != 3.0f || m.pm[0] != 6.0f || g_order[0] != 1 || g_order[1] != 2) {
        std::printf("00a36e70_TEST FAIL\n"); return 1;
    }
    std::printf("00a36e70_TEST PASS\n");
    return 0;
}
