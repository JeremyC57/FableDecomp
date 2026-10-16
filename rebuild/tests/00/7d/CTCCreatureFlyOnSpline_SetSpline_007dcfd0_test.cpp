#include <cstdio>
// Behaviour model of CTCCreatureFlyOnSpline::SetSpline (0x007DCFD0): counted-pointer assignment plus a physics snap.
struct Block { int refs; int destroyed; };
struct Spline { float scale; void GetSplinePosition(float* out, float t) { out[0] = t * scale; out[1] = 0; out[2] = 0; } };
struct Ptr {
    Spline* Data; Block* b;
    Ptr(Spline* s, Block* bl) : Data(s), b(bl) { if (b) ++b->refs; }
    Ptr(const Ptr& o) : Data(o.Data), b(o.b) { if (b) ++b->refs; }
    ~Ptr() { if (b && --b->refs == 0) ++b->destroyed; }
    Ptr& operator=(const Ptr& o) { if (b != o.b) { if (b) { --b->refs; if (b->refs == 0) ++b->destroyed; } Data = o.Data; b = o.b; if (b) ++b->refs; } return *this; }
};
struct Fly {
    Ptr PSpline; float Progress; float physPos;
    Fly(const Ptr& p) : PSpline(p), Progress(0.5f), physPos(0) {}
    void SetSpline(Ptr s) { PSpline = s; float pos[3]; PSpline.Data->GetSplinePosition(pos, Progress); physPos = pos[0]; }
};
int main()
{
    Block b1 = { 0, 0 }, b2 = { 0, 0 };
    Spline s1 = { 2.0f }, s2 = { 10.0f };
    {
        Ptr p1(&s1, &b1);
        Fly f(p1);
        { Ptr p2(&s2, &b2); f.SetSpline(p2); }
        if (f.physPos != 5.0f || b2.refs != 1 || b1.refs != 1 || b1.destroyed) { std::printf("007dcfd0_TEST FAIL\n"); return 1; }
    }
    if (b1.destroyed != 1 || b2.destroyed != 1) { std::printf("007dcfd0_TEST FAIL\n"); return 1; }
    std::printf("007dcfd0_TEST PASS\n");
    return 0;
}