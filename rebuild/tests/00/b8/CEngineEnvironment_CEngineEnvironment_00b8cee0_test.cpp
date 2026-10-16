#include <cstdio>
struct V { float X, Y, Z; V() : X(9), Y(9), Z(9) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Env {
    V Hist[64]; V Interp[64]; V Cur, Next, Prev; float s0, s1, s2; long frame; bool a, b;
    Env() : Cur(1, 0, 0), Next(1, 0, 0), Prev(1, 0, 0), s0(0), s1(0), s2(0), frame(0), a(false), b(false)
    { for (long i = 0; i < 64; ++i) { Hist[i] = V(0, 0, 0); Interp[i] = V(0, 0, 0); } }
};
int main()
{
    Env* e = new Env;
    bool ok = e->Cur.X == 1 && e->Next.X == 1 && e->Prev.X == 1 && e->Prev.Y == 0 && e->frame == 0 && !e->a && !e->b;
    for (int i = 0; i < 64; ++i) if (e->Hist[i].X != 0 || e->Interp[i].Z != 0) ok = false;
    delete e;
    if (!ok) { std::printf("00b8cee0_TEST FAIL\n"); return 1; }
    std::printf("00b8cee0_TEST PASS\n");
    return 0;
}