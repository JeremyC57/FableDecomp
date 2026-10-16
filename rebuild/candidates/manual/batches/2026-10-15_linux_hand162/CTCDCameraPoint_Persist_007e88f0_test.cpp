#include <cstdio>
#include <cstring>
// Behaviour model of CTCDCameraPoint::Persist (0x007E88F0): a loading context fills the members, modes 1/3 skip.
struct Ctx {
    int Mode; int calls;
    void Transfer(const char* name, bool& v, const bool& def) { ++calls; v = std::strcmp(name, "CutOutOf") == 0 ? true : def; }
    void Transfer(const char* name, long& v, const long& def) { ++calls; v = std::strcmp(name, "PointType") == 0 ? 2 : def; }
};
struct Point {
    bool CutInto, CutOutOf, Test; long Type;
    void Persist(Ctx* c)
    {
        if (c->Mode != 1 && c->Mode != 3) {
            c->Transfer("CutInto", CutInto, false); c->Transfer("CutOutOf", CutOutOf, false);
            c->Transfer("TestAngleBeforeActivation", Test, false);
            long t; c->Transfer("PointType", t, 0); Type = t;
        }
    }
};
int main()
{
    Ctx c = { 0, 0 }; Point p = { true, false, true, 0 };
    p.Persist(&c);
    if (c.calls != 4 || p.CutInto || !p.CutOutOf || p.Test || p.Type != 2) { std::printf("007e88f0_TEST FAIL\n"); return 1; }
    c.Mode = 3; c.calls = 0; p.Persist(&c);
    if (c.calls != 0) { std::printf("007e88f0_TEST FAIL\n"); return 1; }
    std::printf("007e88f0_TEST PASS\n");
    return 0;
}
