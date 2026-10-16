#include <cstdio>
// Behaviour model of CVillageDef::CVillageDef: every vector empty, PatrolTrackName = -1, the sound strings empty.
struct Vec { long* a; long* b; long* c; Vec() : a(0), b(0), c(0) {} };
struct Str { const char* p; Str() : p(0) {} };
struct VillageDef {
    Vec Tasks; long PatrolTrackName; Vec Story, School, Work, Cook, Eat, Tavern, Gaming; Str s[4];
    VillageDef() : PatrolTrackName(-1) {}
};
int main()
{
    VillageDef d;
    const Vec* v[8] = { &d.Tasks, &d.Story, &d.School, &d.Work, &d.Cook, &d.Eat, &d.Tavern, &d.Gaming };
    for (int i = 0; i < 8; ++i) if (v[i]->a || v[i]->b || v[i]->c) { std::printf("004dff04_TEST FAIL\n"); return 1; }
    if (d.PatrolTrackName != -1 || d.s[0].p || d.s[3].p) { std::printf("004dff04_TEST FAIL\n"); return 1; }
    std::printf("004dff04_TEST PASS\n");
    return 0;
}
