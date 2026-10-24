#include <cstdio>
#include <cstdlib>
#include <new>
struct Info { long RefCount; };
struct Rec {
    Info* info; int tag;
    Rec(int t, Info* i) : info(i), tag(t) { if (info) ++info->RefCount; }
    Rec(const Rec& o) : info(o.info), tag(o.tag) { if (info) ++info->RefCount; }
    ~Rec() { if (info) --info->RefCount; }
};
struct Entry { long first; Rec second; Entry(const long& k, const Rec& r) : first(k), second(r) { } };
struct false_type { false_type() { } };
static int g_overflows = 0; static unsigned int g_fill = 0; static bool g_atEnd = false; static long g_overflowKey = 0;
struct Vec {
    Entry* _M_start; Entry* _M_finish; Entry* _M_end_of_storage;
    void _M_insert_overflow(Entry* pos, const Entry& x, const false_type&, unsigned int fill, bool atEnd)
    { ++g_overflows; g_fill = fill; g_atEnd = atEnd; g_overflowKey = x.first; if (pos != _M_finish) g_fill = 99; }
    void push_back(const Entry& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) Entry(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, false_type(), 1, true);
    }
};
struct CVectorMap {
    Vec Data; char Compare; bool Dirty;
    void Insert(const long& key, const Rec& value) { Data.push_back(Entry(key, value)); Dirty = true; }
};
static Info g_info = { 0 };

// Behaviour test for 0x009937D0 CVectorMap<long, CSoundBankRecord>::Insert: the {key, record} pair is appended (the
// temporary released, so only the stored copy holds a count) and Dirty is set, also on the overflow path.
int main()
{
    int fails = 0;
    Entry* raw = (Entry*)malloc(sizeof(Entry) * 1);
    CVectorMap m; m.Data._M_start = m.Data._M_finish = raw; m.Data._M_end_of_storage = raw + 1; m.Dirty = false;
    Rec r(70, &g_info);
    m.Insert(7, r);
    if (!m.Dirty || m.Data._M_finish != raw + 1 || raw[0].first != 7 || raw[0].second.tag != 70 || g_info.RefCount != 2) ++fails;
    m.Dirty = false;
    m.Insert(8, r);
    if (!m.Dirty || g_overflows != 1 || g_overflowKey != 8 || g_info.RefCount != 2) ++fails;
    raw[0].~Entry(); free(raw);
    std::printf(fails ? "009937d0_TEST FAIL\n" : "009937d0_TEST PASS\n");
    return fails ? 1 : 0;
}