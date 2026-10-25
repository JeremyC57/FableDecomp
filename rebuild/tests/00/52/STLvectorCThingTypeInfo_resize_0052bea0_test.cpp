// Behaviour test for 0x0052BEA0: vector<CThingTypeInfo>::resize(n, x)
#include <cstdio>
#include <new>

static int g_destroyed = 0;

class CThingTypeInfo {                                       // test stand-in for the element: one tag word
public:
    long Tag;
    CThingTypeInfo() : Tag(0) { }
    CThingTypeInfo(long tag) : Tag(tag) { }
    ~CThingTypeInfo() { ++g_destroyed; }
};

class TestVector {                                  // fixed-capacity copy of the STLport vector shape
public:
    typedef CThingTypeInfo* iterator;
    typedef unsigned int size_type;
    unsigned char storage[16 * sizeof(CThingTypeInfo)];
    CThingTypeInfo* _M_start;
    CThingTypeInfo* _M_finish;
    int eraseCalls, insertCalls;
    TestVector() : eraseCalls(0), insertCalls(0) { _M_start = _M_finish = (CThingTypeInfo*)storage; }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    iterator erase(iterator first, iterator last)
    {
        ++eraseCalls;
        iterator dst = first;
        for (iterator src = last; src != _M_finish; ++src, ++dst) *dst = *src;     // copy down
        for (iterator p = dst; p != _M_finish; ++p) p->~CThingTypeInfo();                     // destroy the tail
        _M_finish = dst;
        return first;
    }
    void _M_fill_insert(iterator pos, size_type n, const CThingTypeInfo& x)
    {
        ++insertCalls;
        for (size_type i = 0; i < n; ++i) { new ((void*)_M_finish) CThingTypeInfo(x); ++_M_finish; }   // pos == end() here
        (void)pos;
    }
    void insert(iterator pos, size_type n, const CThingTypeInfo& x) { _M_fill_insert(pos, n, x); }
    void resize(size_type new_size, const CThingTypeInfo& x)     // the reconstructed body
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};

int main()
{
    TestVector v;
    CThingTypeInfo a(7);
    v.resize(5, a);                                 // grow from empty: five copies of a
    if (v.size() != 5 || v.insertCalls != 1 || v.eraseCalls != 0 || v._M_start[0].Tag != 7 || v._M_start[4].Tag != 7) {
        std::printf("0052bea0_TEST FAIL\n");
        return 1;
    }
    CThingTypeInfo b(9);
    v.resize(8, b);                                 // grow by three copies of b
    if (v.size() != 8 || v.insertCalls != 2 || v._M_start[4].Tag != 7 || v._M_start[5].Tag != 9 || v._M_start[7].Tag != 9) {
        std::printf("0052bea0_TEST FAIL\n");
        return 1;
    }
    int before = g_destroyed;
    v.resize(3, b);                                 // shrink: erase [begin + 3, end)
    if (v.size() != 3 || v.eraseCalls != 1 || g_destroyed - before != 5 || v._M_start[2].Tag != 7) {
        std::printf("0052bea0_TEST FAIL\n");
        return 1;
    }
    v.resize(3, b);                                 // same size: an empty fill insert
    if (v.size() != 3 || v.insertCalls != 3 || v.eraseCalls != 1) {
        std::printf("0052bea0_TEST FAIL\n");
        return 1;
    }
    v.resize(0, b);
    std::printf("0052bea0_TEST PASS\n");
    return 0;
}