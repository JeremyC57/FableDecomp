// Behaviour test for 0x00592FFB std::__median<pair<long, CDefPointer<...>>, CContextSensitivePriorityCompare> (model copy)
#include <cstdio>
struct Entry { int id; int priority; };
struct PriorityCompare { bool operator()(const Entry& l, const Entry& r) const { return l.priority > r.priority; } };
static const Entry& median(const Entry& a, const Entry& b, const Entry& c, PriorityCompare comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
int main()
{
    Entry e[3];
    for (int i = 0; i < 3; ++i) e[i].id = i;
    int perms[6][3] = { {1,2,3}, {1,3,2}, {2,1,3}, {2,3,1}, {3,1,2}, {3,2,1} };
    for (int p = 0; p < 6; ++p) {
        for (int i = 0; i < 3; ++i) e[i].priority = perms[p][i];
        const Entry& m = median(e[0], e[1], e[2], PriorityCompare());
        if (m.priority != 2) { std::printf("00592ffb_TEST FAIL\n"); return 1; }
    }
    std::printf("00592ffb_TEST PASS\n");
    return 0;
}