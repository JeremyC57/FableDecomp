// behaviour test for 0x004766C2: inserting n copies of a record with user copy/assign keeps order and object counts
#include <cstdio>
#include <vector>
static int g_live = 0;
struct R { int v; R(int x = 0) : v(x) { ++g_live; } R(const R& o) : v(o.v) { ++g_live; } R& operator=(const R& o) { v = o.v; return *this; } virtual ~R() { --g_live; } };
int main()
{
    {
        std::vector<R> v; v.reserve(12); for (int i = 0; i < 4; ++i) v.push_back(R(i));
        v.insert(v.begin() + 1, 2, R(7)); v.insert(v.begin() + 5, 3, R(8));
        int ok = v.size() == 9 && v[0].v == 0 && v[1].v == 7 && v[3].v == 1 && v[5].v == 8 && v[8].v == 3;
        v.insert(v.end(), 6, R(5)); ok = ok && v.size() == 15 && v[14].v == 5;
        if (!ok) { std::printf("004766c2_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("004766c2_TEST FAIL\n"); return 1; }
    std::printf("004766c2_TEST PASS\n");
    return 0;
}