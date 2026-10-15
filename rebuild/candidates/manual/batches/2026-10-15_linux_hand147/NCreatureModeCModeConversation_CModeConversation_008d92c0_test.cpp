#include <cstdio>
struct Talk { long att; long GetAttitude() { return att; } };
struct Thing { bool hasTalk; Talk talk; bool GetTalk(Talk** p) { if (hasTalk) { *p = &talk; return true; } return false; } };
static int g_set = 0; static long g_lastGroup = -1;
static long GroupName(long a) { return a * 100; }
struct Filter { long group; Filter(long g) : group(g) {} void SetGroup(long g) { group = g; ++g_set; g_lastGroup = g; } };
struct D { Thing* parent; long cur, init; Filter f; bool reset;
    D(Thing* t, long at) : parent(t), cur(at), init(at), f(GroupName(at)), reset(true) {
        Talk* talk; if (parent->GetTalk(&talk)) { long a = talk->GetAttitude(); cur = a; if (init != a) f.SetGroup(GroupName(a)); } } };
int main()
{
    Thing a = { false, { 5 } }; D da(&a, 3);
    Thing b = { true, { 3 } }; D db(&b, 3);
    Thing c = { true, { 4 } }; D dc(&c, 2);
    if (da.cur != 3 || da.init != 3 || da.f.group != 300 || !da.reset || db.cur != 3 || g_set != 1
        || dc.cur != 4 || dc.init != 2 || dc.f.group != 400) { std::printf("008d92c0_TEST FAIL\n"); return 1; }
    std::printf("008d92c0_TEST PASS\n");
    return 0;
}
