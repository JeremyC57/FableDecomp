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

// Behaviour test for 0x009936E0 vector<pair<long, CSoundBankRecord>>::push_back: with room the entry is copied in place
// (record count bumped) and finish advances one 36-byte step; when full the overflow path gets (finish, x, 1, true).
int main()
{
    int fails = 0;
    Entry* raw = (Entry*)malloc(sizeof(Entry) * 2);
    Vec v; v._M_start = v._M_finish = raw; v._M_end_of_storage = raw + 2;
    Entry a(3, Rec(30, &g_info)), b(4, Rec(40, &g_info));
    v.push_back(a); v.push_back(b);
    if (v._M_finish != raw + 2 || raw[0].first != 3 || raw[1].second.tag != 40 || g_info.RefCount != 4 || g_overflows) ++fails;
    v.push_back(a);
    if (g_overflows != 1 || g_fill != 1 || !g_atEnd || g_overflowKey != 3 || g_info.RefCount != 4) ++fails;
    raw[0].~Entry(); raw[1].~Entry(); free(raw);
    std::printf(fails ? "009936e0_TEST FAIL\n" : "009936e0_TEST PASS\n");
    return fails ? 1 : 0;
}