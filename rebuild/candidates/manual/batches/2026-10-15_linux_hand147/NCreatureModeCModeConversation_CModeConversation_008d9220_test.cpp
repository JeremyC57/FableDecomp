#include <cstdio>
struct Talk { long att; long GetAttitude() { return att; } };
struct Thing { bool hasTalk; Talk talk; bool GetTalk(Talk** p) { if (hasTalk) { *p = &talk; return true; } return false; } };
static int g_set = 0; static long g_lastGroup = -1;
static long GroupName(long a) { return a * 100; }
struct Filter { long group; Filter(long g) : group(g) {} void SetGroup(long g) { group = g; ++g_set; g_lastGroup = g; } };
struct D { Thing* parent; long cur, init; Filter f; bool reset;
    D(Thing* t) : parent(t), cur(1), init(1), f(GroupName(1)), reset(true) {
        Talk* talk; if (parent->GetTalk(&talk)) { long a = talk->GetAttitude(); init = a; if (a != cur) { cur = a; f.SetGroup(GroupName(a)); } } } };
int main()
{
    Thing a = { false, { 5 } }; D da(&a);
    Thing b = { true, { 1 } }; D db(&b);
    Thing c = { true, { 4 } }; D dc(&c);
    if (da.cur != 1 || da.init != 1 || da.f.group != 100 || !da.reset || db.cur != 1 || g_set != 1
        || dc.cur != 4 || dc.init != 4 || dc.f.group != 400) { std::printf("008d9220_TEST FAIL\n"); return 1; }
    std::printf("008d9220_TEST PASS\n");
    return 0;
}
