// behaviour test for 0x006A2BD0 pair<CIntelligentPointer<CThing>, ...> copy constructor (model: the pointer re-registers the item, the second is copied)
#include <cstdio>
static int g_sets = 0;
struct P { void* item; P() : item(0) { } P(const P& o) : item(0) { item = o.item; ++g_sets; } };
struct Pair { P first; unsigned char second; Pair(const Pair& o) : first(o.first), second(o.second) { } Pair() : second(0) { } };
int main()
{
    int obj = 0;
    Pair a; a.first.item = &obj; a.second = 7;
    Pair b(a);
    int ok = b.first.item == &obj && b.second == 7 && g_sets == 1;
    if (!ok) { std::printf("006a2bd0_TEST FAIL\n"); return 1; }
    std::printf("006a2bd0_TEST PASS\n");
    return 0;
}