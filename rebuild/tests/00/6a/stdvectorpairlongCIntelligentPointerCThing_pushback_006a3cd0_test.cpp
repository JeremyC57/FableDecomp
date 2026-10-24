// Behaviour test for 006a3cd0: STLport vector::push_back (construct in place or grow through _M_insert_overflow; model copy).
#include <cstdio>
#include <new>
class CThing { public: int id; };
struct CIntelligentPointer {
    CThing* item;
    CIntelligentPointer(CThing* t) : item(t) { }
    CIntelligentPointer(const CIntelligentPointer& o) : item(o.item) { }
};
struct SThingEntry {
    long first;
    CIntelligentPointer second;
    SThingEntry(long k, CThing* t) : first(k), second(t) { }
    SThingEntry(const SThingEntry& o) : first(o.first), second(o.second) { }
};
struct __false_type { };
static unsigned char g_raw[8][sizeof(SThingEntry)];
static int g_overflows = 0;
struct vector {
    SThingEntry* _M_start;
    SThingEntry* _M_finish;
    SThingEntry* _M_end_of_storage;
    void _M_insert_overflow(SThingEntry* position, const SThingEntry& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        // stand-in for 0x006A3A70: grow the capacity by two slots and append at the end
        ++g_overflows;
        if (position != _M_finish || fill_len != 1 || !atend) return;
        _M_end_of_storage += 2;
        new ((void*)_M_finish) SThingEntry(x);
        ++_M_finish;
    }
    void push_back(const SThingEntry& x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new ((void*)this->_M_finish) SThingEntry(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};
int main()
{
    CThing t; t.id = 1;
    vector v; v._M_start = v._M_finish = (SThingEntry*)(void*)g_raw[0]; v._M_end_of_storage = v._M_start + 1;
    int fail = 0;
    v.push_back(SThingEntry(5, &t));
    if (v._M_finish != v._M_start + 1 || g_overflows != 0) fail = 1;
    v.push_back(SThingEntry(6, &t));
    if (v._M_finish != v._M_start + 2 || g_overflows != 1) fail = 1;
    v.push_back(SThingEntry(7, &t));
    if (v._M_finish != v._M_start + 3 || g_overflows != 1) fail = 1;
    for (int i = 0; i < 3; ++i) if (v._M_start[i].first != 5 + i || v._M_start[i].second.item != &t) fail = 1;
    if (fail) { std::printf("006a3cd0_TEST FAIL\n"); return 1; }
    std::printf("006a3cd0_TEST PASS\n");
    return 0;
}