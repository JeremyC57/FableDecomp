// Behaviour test for 00857dc0: body copied from src/00857dc0.cpp (CTCCameraPointScriptedSpline::RemoveAnimIndices), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_memmove = 0;
static void* __cdecl my_memmove(void* dst, const void* src, unsigned int n) { ++g_memmove; return memmove(dst, src, n); }
#define memmove my_memmove
inline void* operator new(unsigned int, void* p) { return p; }
struct __true_type { __true_type() {} };
static int g_overflow = 0;
struct CLongVector {
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    void _M_insert_overflow(long* pos, const long& x, const __true_type&, unsigned int fill_len, bool atend)
    { ++g_overflow; CHECK(pos == _M_finish && fill_len == 1 && atend); *_M_finish = x; ++_M_finish; ++_M_end_of_storage; }
    static long* __copy_trivial(const long* first, const long* last, long* result)
    {
        if (last == first)
            return result;
        unsigned int n = (const char*)last - (const char*)first;
        return (long*)((char*)memmove(result, first, n) + n);
    }
    long* erase(long* first, long* last)
    {
        long* i = __copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
struct CAnimIndexOwner_00857dc0 {
    unsigned char _pad00[0xf0];
    CLongVector   AnimIndices;
    __forceinline void AddAnimIndex(long index) { AnimIndices.push_back(index); }
    void RemoveAnimIndices(const CLongVector& indices);
};
void CAnimIndexOwner_00857dc0::RemoveAnimIndices(const CLongVector& indices)
{
    AnimIndices.clear();
    for (const long* it = indices._M_start; it != indices._M_finish; ++it)
        AddAnimIndex(*it);
}
int main()
{
    long storage[6] = { 100, 200, 300, 0, 0, 0 };
    CAnimIndexOwner_00857dc0 owner; memset(&owner, 0, sizeof owner);
    owner.AnimIndices._M_start = storage; owner.AnimIndices._M_finish = storage + 3; owner.AnimIndices._M_end_of_storage = storage + 4;
    long input[5] = { 7, 8, 9, 10, 11 };
    CLongVector indices; indices._M_start = input; indices._M_finish = input + 5; indices._M_end_of_storage = input + 5;
    owner.RemoveAnimIndices(indices);                       // cleared (no memmove when the tail is empty), 4 fit, the fifth grows
    CHECK(g_memmove == 0 && g_overflow == 1 && owner.AnimIndices._M_finish - storage == 5);
    CHECK(storage[0] == 7 && storage[3] == 10 && storage[4] == 11);
    indices._M_finish = input;                              // empty input: just cleared
    owner.RemoveAnimIndices(indices);
    CHECK(owner.AnimIndices._M_finish == storage && g_overflow == 1);
    if (fails == 0) printf("00857dc0_TEST PASS\n"); else printf("00857dc0_TEST FAIL (%d)\n", fails);
    return fails;
}