// Behaviour test for 006a3220: STLport __uninitialized_fill_n over (long, CIntelligentPointer<CThing>) pairs (model copy).
#include <cstdio>
#include <new>
class CThing { public: int id; };
static int g_copies = 0;
struct CIntelligentPointer {
    CThing* item;
    CIntelligentPointer(const CIntelligentPointer& o) : item(o.item) { ++g_copies; }
    CIntelligentPointer(CThing* t) : item(t) { }
};
struct SThingEntry {
    long first;
    CIntelligentPointer second;
    SThingEntry(long k, CThing* t) : first(k), second(t) { }
    SThingEntry(const SThingEntry& o) : first(o.first), second(o.second) { }
};
struct __false_type { };
SThingEntry* uninitialized_fill_n(SThingEntry* first, unsigned int n, const SThingEntry& x, const __false_type&)
{
    SThingEntry* cur = first;
    for (; n > 0; --n, ++cur)
        new ((void*)&*cur) SThingEntry(x);
    return cur;
}
int main()
{
    CThing t; t.id = 7;
    SThingEntry x(42, &t);
    unsigned char raw[5 * sizeof(SThingEntry)];
    SThingEntry* buf = (SThingEntry*)(void*)raw;
    int fail = 0;
    SThingEntry* end = uninitialized_fill_n(buf, 4, x, __false_type());
    if (end != buf + 4 || g_copies != 4) fail = 1;
    for (int i = 0; i < 4; ++i) if (buf[i].first != 42 || buf[i].second.item != &t) fail = 1;
    if (uninitialized_fill_n(buf, 0, x, __false_type()) != buf || g_copies != 4) fail = 1;
    if (fail) { std::printf("006a3220_TEST FAIL\n"); return 1; }
    std::printf("006a3220_TEST PASS\n");
    return 0;
}