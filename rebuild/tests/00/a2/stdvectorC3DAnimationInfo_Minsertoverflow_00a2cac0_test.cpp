// Behaviour test for 0x00A2CAC0: STLport vector<C3DAnimationInfo>::_M_insert_overflow -- grow to size + max(size, n), copy the head,
// construct x once (n == 1) or fill n copies, copy the tail unless atend, destroy and free the old block.
#include <cstdio>
#include <stdlib.h>
#include <new>

static int g_live;                                          // constructed - destroyed elements
class C3DAnimationInfo {                                                 // a non-trivial element (user copy, counted lifetime)
public:
    int Id;
    int Payload[9];
    explicit C3DAnimationInfo(int id) : Id(id) { ++g_live; for (int i = 0; i < 9; ++i) Payload[i] = id * 10 + i; }
    C3DAnimationInfo(const C3DAnimationInfo& o) : Id(o.Id) { ++g_live; for (int i = 0; i < 9; ++i) Payload[i] = o.Payload[i]; }
    ~C3DAnimationInfo() { --g_live; }
};

struct Vec {
    C3DAnimationInfo* _M_start; C3DAnimationInfo* _M_finish; C3DAnimationInfo* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static C3DAnimationInfo* Copy(C3DAnimationInfo* first, C3DAnimationInfo* last, C3DAnimationInfo* result)
    {
        for (; first != last; ++first, ++result) new (result) C3DAnimationInfo(*first);
        return result;
    }
    static C3DAnimationInfo* Fill(C3DAnimationInfo* first, unsigned int n, const C3DAnimationInfo& x)
    {
        for (; n > 0; --n, ++first) new (first) C3DAnimationInfo(x);
        return first;
    }
    void _M_insert_overflow(C3DAnimationInfo* position, const C3DAnimationInfo& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        C3DAnimationInfo* new_start = len != 0 ? (C3DAnimationInfo*)malloc(len * sizeof(C3DAnimationInfo)) : 0;
        C3DAnimationInfo* new_finish = Copy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) C3DAnimationInfo(x);
            ++new_finish;
        } else
            new_finish = Fill(new_finish, fill_len, x);
        if (!atend)
            new_finish = Copy(position, _M_finish, new_finish);
        for (C3DAnimationInfo* p = _M_start; p != _M_finish; ++p) p->~C3DAnimationInfo();
        if (_M_start) free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    Vec v = { 0, 0, 0 };
    { C3DAnimationInfo a(1); v._M_insert_overflow(v._M_finish, a, 1, true); }                 // push_back on an empty vector
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || v._M_start[0].Id != 1 || g_live != 1) { std::printf("00a2cac0_TEST FAIL\n"); return 1; }
    { C3DAnimationInfo b(2); v._M_insert_overflow(v._M_finish, b, 1, true); }
    { C3DAnimationInfo c(3); v._M_insert_overflow(v._M_start + 1, c, 3, false); }            // fill 3 in the middle, keep the tail
    const int want[] = { 1, 3, 3, 3, 2 };
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5 || g_live != 5) { std::printf("00a2cac0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i].Id != want[i] || v._M_start[i].Payload[9 - 1] != want[i] * 10 + 9 - 1) { std::printf("00a2cac0_TEST FAIL\n"); return 1; }
    for (C3DAnimationInfo* p = v._M_start; p != v._M_finish; ++p) p->~C3DAnimationInfo();
    free(v._M_start);
    if (g_live != 0) { std::printf("00a2cac0_TEST FAIL\n"); return 1; }
    std::printf("00a2cac0_TEST PASS\n");
    return 0;
}