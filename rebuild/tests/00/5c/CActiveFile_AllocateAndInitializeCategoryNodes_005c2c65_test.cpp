// Behaviour test for 005c2c65: body copied from src/005c2c65.cpp (vector<CCategory>::_M_insert_overflow), callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_malloc = 0; static unsigned int g_mallocSize = 0; static int g_copies = 0;
static void* my_malloc(unsigned int size) { ++g_malloc; g_mallocSize = size; return std::malloc(size); }
#define malloc my_malloc
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
struct CCategory {
    unsigned char _body[0x2c];
    CCategory() { memset(_body, 0, sizeof _body); }
    CCategory(const CCategory& other) { memcpy(_body, other._body, sizeof _body); ++g_copies; }
};
static int g_ucopy = 0, g_fill = 0, g_clear = 0; static unsigned long g_fillCount = 0;
static CCategory* __fastcall UninitializedCopy(const CCategory* first, const CCategory* last, CCategory* result, const __false_type&)
{ ++g_ucopy; for (; first != last; ++first, ++result) new (result) CCategory(*first); return result; }
static CCategory* __fastcall UninitializedFillN(CCategory* first, unsigned long count, const CCategory& value, const __false_type&)
{ ++g_fill; g_fillCount = count; for (unsigned long i = 0; i < count; ++i) new (first + i) CCategory(value); return first + count; }
template<class T> __forceinline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct CCategoryVector {
    CCategory* _M_start;
    CCategory* _M_finish;
    CCategory* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    static __forceinline CCategory* allocate(unsigned long n) { return n != 0 ? (CCategory*)malloc(n * sizeof(CCategory)) : 0; }
    void _M_clear() { ++g_clear; }
    __forceinline void _M_set(CCategory* start, CCategory* finish, CCategory* endOfStorage)
    {
        _M_start = start;
        _M_finish = finish;
        _M_end_of_storage = endOfStorage;
    }
    void _M_insert_overflow(CCategory* position, const CCategory& x, const __false_type& isPOD, unsigned long fillLength, bool atEnd);
};
void CCategoryVector::_M_insert_overflow(CCategory* position, const CCategory& x, const __false_type&,
                                         unsigned long fillLength, bool atEnd)
{
    const unsigned long oldSize = size();
    const unsigned long length = oldSize + Max(oldSize, fillLength);
    CCategory* newStart = allocate(length);
    CCategory* newFinish = UninitializedCopy(_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        new(newFinish) CCategory(x);
        ++newFinish;
    } else {
        newFinish = UninitializedFillN(newFinish, fillLength, x, __false_type());
    }
    if (!atEnd)
        newFinish = UninitializedCopy(position, _M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
int main()
{
    CCategory old[3]; old[0]._body[0] = 1; old[1]._body[0] = 2; old[2]._body[0] = 3;
    CCategory x; x._body[0] = 9;
    // push_back style: 3 elements, insert 1 at end -> capacity 6
    CCategoryVector v; v._M_start = old; v._M_finish = old + 3; v._M_end_of_storage = old + 3;
    v._M_insert_overflow(old + 3, x, __false_type(), 1, true);
    CHECK(g_malloc == 1 && g_mallocSize == 6 * 0x2c && g_ucopy == 1 && g_fill == 0 && g_clear == 1);
    CHECK(v._M_finish - v._M_start == 4 && v._M_end_of_storage - v._M_start == 6);
    CHECK(v._M_start[0]._body[0] == 1 && v._M_start[2]._body[0] == 3 && v._M_start[3]._body[0] == 9 && g_copies == 4);
    // insert 5 copies in the middle (position 1) of a 3-element vector, not at end -> length 3 + max(3,5) = 8, tail copied
    CCategoryVector w; w._M_start = old; w._M_finish = old + 3; w._M_end_of_storage = old + 3;
    w._M_insert_overflow(old + 1, x, __false_type(), 5, false);
    CHECK(g_malloc == 2 && g_mallocSize == 8 * 0x2c && g_fill == 1 && g_fillCount == 5 && g_ucopy == 3 && g_clear == 2);
    CHECK(w._M_finish - w._M_start == 8 && w._M_start[0]._body[0] == 1 && w._M_start[1]._body[0] == 9 && w._M_start[5]._body[0] == 9 && w._M_start[6]._body[0] == 2 && w._M_start[7]._body[0] == 3);
    // empty vector, fill length 0 -> no allocation (allocate(0) returns null)
    CCategoryVector e; e._M_start = 0; e._M_finish = 0; e._M_end_of_storage = 0;
    e._M_insert_overflow(0, x, __false_type(), 0, true);
    CHECK(g_malloc == 2 && e._M_start == 0 && e._M_finish == 0 && g_fill == 2 && g_fillCount == 0);
    std::free(v._M_start); std::free(w._M_start);
    if (fails == 0) printf("005c2c65_TEST PASS\n"); else printf("005c2c65_TEST FAIL (%d)\n", fails);
    return fails;
}