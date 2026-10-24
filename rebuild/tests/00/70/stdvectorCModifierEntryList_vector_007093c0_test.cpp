// Behaviour test for 0x007093C0 vector<vector<CModifierEntry>>::vector(size_type n) (STLport sized constructor).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct CModifierEntry { unsigned char _body[8]; };
struct CModifierEntryList {
    CModifierEntry* _M_start; CModifierEntry* _M_finish; CModifierEntry* _M_end_of_storage;
    CModifierEntryList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CModifierEntryList(const CModifierEntryList& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_copies; }
};
struct CModifierListVector {
    CModifierEntryList* _M_start; CModifierEntryList* _M_finish; CModifierEntryList* _M_end_of_storage;
    explicit CModifierListVector(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (CModifierEntryList*)std::malloc(n * sizeof(CModifierEntryList)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        CModifierEntryList value;
        CModifierEntryList* cur = _M_start;
        for (unsigned int k = n; k > 0; --k, ++cur)
            new (cur) CModifierEntryList(value);
        _M_finish = cur;
    }
};

int main()
{
    CModifierListVector v(3);
    bool ok = g_copies == 3 && v._M_finish == v._M_start + 3 && v._M_end_of_storage == v._M_start + 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && v._M_start[i]._M_start == 0 && v._M_start[i]._M_finish == 0 && v._M_start[i]._M_end_of_storage == 0;
    CModifierListVector e(0);
    ok = ok && g_copies == 3 && e._M_start == 0 && e._M_finish == 0 && e._M_end_of_storage == 0;
    std::free(v._M_start);
    if (!ok) { std::printf("007093c0_TEST FAIL\n"); return 1; }
    std::printf("007093c0_TEST PASS\n");
    return 0;
}