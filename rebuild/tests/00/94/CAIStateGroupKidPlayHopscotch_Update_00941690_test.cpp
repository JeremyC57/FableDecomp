#include <cstdio>
#include <cmath>
// Behaviour model of CAIStateGroup_KidPlayHopscotch::Update's decision logic.
enum { NUL=0, TERM=1, FIN=2, FAIL=3, PROG=4 };
struct V { float X, Y, Z; };
struct Sim {
    bool alive; int moveSeq[4]; int mi; int lookSeq[4]; int li; int perf[2]; int pi; V pos; V start; V fwd;
    int moves, looks, performs, nothing, terminates;
};
static int Update(Sim& s)
{
    int ret;
    do {
        if (!s.alive) { s.terminates++; return 0; }
        s.moves++; ret = s.moveSeq[s.mi++];
    } while (ret == PROG);
    if (ret != FIN || !s.alive) { s.terminates++; return 1; }
    do {
        if (!s.alive) { s.terminates++; return 2; }
        s.looks++; ret = s.lookSeq[s.li++];
    } while (ret == PROG);
    if (ret != FIN || !s.alive) { s.terminates++; return 3; }
    s.performs++; ret = s.perf[s.pi++];
    if (ret != FIN) { s.terminates++; return 4; }
    V d = { s.pos.X - s.start.X, s.pos.Y - s.start.Y, s.pos.Z - s.start.Z };
    float r = 1.0f / std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
    float dot = d.X * r * s.fwd.X + d.Y * r * s.fwd.Y + d.Z * r * s.fwd.Z;
    if (dot > 0.9f) {
        s.performs++; ret = s.perf[s.pi++];
        if (ret != FIN) { s.terminates++; return 5; }
    }
    s.nothing++; s.terminates++; return 6;
}
int main()
{
    Sim a = { true, {PROG, PROG, FIN, 0}, 0, {PROG, FIN, 0, 0}, 0, {FIN, FIN}, 0, {0, 5, 0}, {0, 0, 0}, {0, 1, 0}, 0,0,0,0,0 };
    int r = Update(a);
    if (r != 6 || a.moves != 3 || a.looks != 2 || a.performs != 2 || a.nothing != 1 || a.terminates != 1) { std::printf("00941690_TEST FAIL\n"); return 1; }
    Sim b = { true, {FIN}, 0, {FIN}, 0, {FIN, FIN}, 0, {5, 0, 0}, {0, 0, 0}, {0, 1, 0}, 0,0,0,0,0 };
    r = Update(b);
    if (r != 6 || b.performs != 1 || b.nothing != 1) { std::printf("00941690_TEST FAIL\n"); return 1; }
    Sim c = { true, {FAIL}, 0, {FIN}, 0, {FIN, FIN}, 0, {0, 5, 0}, {0, 0, 0}, {0, 1, 0}, 0,0,0,0,0 };
    r = Update(c);
    if (r != 1 || c.looks != 0 || c.terminates != 1) { std::printf("00941690_TEST FAIL\n"); return 1; }
    Sim d = { false, {FIN}, 0, {FIN}, 0, {FIN, FIN}, 0, {0, 5, 0}, {0, 0, 0}, {0, 1, 0}, 0,0,0,0,0 };
    r = Update(d);
    if (r != 0 || d.moves != 0 || d.terminates != 1) { std::printf("00941690_TEST FAIL\n"); return 1; }
    Sim e = { true, {FIN}, 0, {FIN}, 0, {FAIL, FIN}, 0, {0, 5, 0}, {0, 0, 0}, {0, 1, 0}, 0,0,0,0,0 };
    r = Update(e);
    if (r != 4 || e.nothing != 0) { std::printf("00941690_TEST FAIL\n"); return 1; }
    std::printf("00941690_TEST PASS\n");
    return 0;
}