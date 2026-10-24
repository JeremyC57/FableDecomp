// Behaviour test for 0x009E7680 CArray::DeallocateMemory: erase all, zero the three words, free the block.
#include <cstdio>
#include <cstdlib>

struct SEntry { int key; int a; int b; int c; };
static int gDestroys = 0, gFrees = 0;

static SEntry* __copy(SEntry* first, SEntry* last, SEntry* result) { for (; first != last; ++first, ++result) *result = *first; return result; }
static void __destroy_aux(SEntry* first, SEntry* last) { for (; first != last; ++first) ++gDestroys; }

template <class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

struct CArray {
    SEntry* _M_start; SEntry* _M_finish; SEntry* _M_end_of_storage;
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    void erase(SEntry* f, SEntry* l)
    {
        SEntry* i = __copy(l, _M_finish, f);
        __destroy_aux(i, _M_finish);
        _M_finish = i;
    }
    void DeallocateMemory()
    {
        if (capacity() > 0) {
            erase(_M_start, _M_finish);
            SEntry* p = Exchange(_M_start, (SEntry*)0);
            SEntry* l = Exchange(_M_finish, (SEntry*)0);
            _M_end_of_storage = 0;
            __destroy_aux(p, l);
            if (p) { ++gFrees; std::free(p); }
        }
    }
};

int main()
{
    CArray a = { 0, 0, 0 };
    a.DeallocateMemory();
    bool ok = gFrees == 0 && gDestroys == 0;
    a._M_start = (SEntry*)std::malloc(4 * sizeof(SEntry));
    a._M_finish = a._M_start + 3;
    a._M_end_of_storage = a._M_start + 4;
    a.DeallocateMemory();
    ok = ok && a._M_start == 0 && a._M_finish == 0 && a._M_end_of_storage == 0 && gFrees == 1 && gDestroys == 3;
    if (!ok) { std::printf("009e7680_TEST FAIL\n"); return 1; }
    std::printf("009e7680_TEST PASS\n");
    return 0;
}