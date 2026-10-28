// Behaviour test for 0x00A2C7F0: STLport vector<C3DMeshInfo>::_M_insert_overflow -- grow to size + max(size, n), copy the head,
// construct x once (n == 1) or fill n copies, copy the tail unless atend, destroy and free the old block.
#include <cstdio>
#include <stdlib.h>
#include <new>

static int g_live;                                          // constructed - destroyed elements
class C3DMeshInfo {                                                 // a non-trivial element (user copy, counted lifetime)
public:
    int Id;
    int Payload[23];
    explicit C3DMeshInfo(int id) : Id(id) { ++g_live; for (int i = 0; i < 23; ++i) Payload[i] = id * 10 + i; }
    C3DMeshInfo(const C3DMeshInfo& o) : Id(o.Id) { ++g_live; for (int i = 0; i < 23; ++i) Payload[i] = o.Payload[i]; }
    ~C3DMeshInfo() { --g_live; }
};

struct Vec {
    C3DMeshInfo* _M_start; C3DMeshInfo* _M_finish; C3DMeshInfo* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static C3DMeshInfo* Copy(C3DMeshInfo* first, C3DMeshInfo* last, C3DMeshInfo* result)
    {
        for (; first != last; ++first, ++result) new (result) C3DMeshInfo(*first);
        return result;
    }
    static C3DMeshInfo* Fill(C3DMeshInfo* first, unsigned int n, const C3DMeshInfo& x)
    {
        for (; n > 0; --n, ++first) new (first) C3DMeshInfo(x);
        return first;
    }
    void _M_insert_overflow(C3DMeshInfo* position, const C3DMeshInfo& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        C3DMeshInfo* new_start = len != 0 ? (C3DMeshInfo*)malloc(len * sizeof(C3DMeshInfo)) : 0;
        C3DMeshInfo* new_finish = Copy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) C3DMeshInfo(x);
            ++new_finish;
        } else
            new_finish = Fill(new_finish, fill_len, x);
        if (!atend)
            new_finish = Copy(position, _M_finish, new_finish);
        for (C3DMeshInfo* p = _M_start; p != _M_finish; ++p) p->~C3DMeshInfo();
        if (_M_start) free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    Vec v = { 0, 0, 0 };
    { C3DMeshInfo a(1); v._M_insert_overflow(v._M_finish, a, 1, true); }                 // push_back on an empty vector
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || v._M_start[0].Id != 1 || g_live != 1) { std::printf("00a2c7f0_TEST FAIL\n"); return 1; }
    { C3DMeshInfo b(2); v._M_insert_overflow(v._M_finish, b, 1, true); }
    { C3DMeshInfo c(3); v._M_insert_overflow(v._M_start + 1, c, 3, false); }            // fill 3 in the middle, keep the tail
    const int want[] = { 1, 3, 3, 3, 2 };
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5 || g_live != 5) { std::printf("00a2c7f0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i].Id != want[i] || v._M_start[i].Payload[23 - 1] != want[i] * 10 + 23 - 1) { std::printf("00a2c7f0_TEST FAIL\n"); return 1; }
    for (C3DMeshInfo* p = v._M_start; p != v._M_finish; ++p) p->~C3DMeshInfo();
    free(v._M_start);
    if (g_live != 0) { std::printf("00a2c7f0_TEST FAIL\n"); return 1; }
    std::printf("00a2c7f0_TEST PASS\n");
    return 0;
}