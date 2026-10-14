#include <cstdio>
static int g_baseCopies = 0;
struct CDefClassBase { virtual void v00() {} unsigned char _body[0x24]; CDefClassBase() {} CDefClassBase(const CDefClassBase& o) { ++g_baseCopies; _body[0] = o._body[0]; } };
struct CPrizeScoreDef : CDefClassBase { virtual void v00() {} long Score; long Mult; CPrizeScoreDef() : Score(0), Mult(0) {} CPrizeScoreDef(const CPrizeScoreDef& o) : CDefClassBase(o), Score(o.Score), Mult(o.Mult) {} };

int main()
{
    CPrizeScoreDef a; a._body[0] = 0x5a; a.Score = 1234; a.Mult = -3;
    CPrizeScoreDef b(a);
    if (g_baseCopies != 1 || b._body[0] != 0x5a || b.Score != 1234 || b.Mult != -3) { std::printf("004edc9b_TEST FAIL\n"); return 1; }
    std::printf("004edc9b_TEST PASS\n");
    return 0;
}
