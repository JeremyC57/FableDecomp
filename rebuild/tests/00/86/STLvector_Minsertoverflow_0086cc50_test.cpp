// Behaviour test for 0x0086CC50: STLport vector::_M_insert_overflow grows to old_size + max(old_size, fill_len), keeps the
// head, places fill_len copies of x at the position, keeps the tail unless appending, and frees the old block.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CSkeletalMorph { int GroupID; int NameTablePos; unsigned int AllowVariationWord; };
struct SPair { int first; CSkeletalMorph second; };

struct SVector
{
    SPair* _M_start;
    SPair* _M_finish;
    SPair* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void insert_overflow(SPair* position, const SPair& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        SPair* new_start = len != 0 ? (SPair*)std::malloc(len * sizeof(SPair)) : 0;
        SPair* new_finish = new_start;
        for (SPair* p = _M_start; p != position; ++p, ++new_finish) new (new_finish) SPair(*p);
        if (fill_len == 1) { new (new_finish) SPair(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) SPair(x);
        if (!atend) for (SPair* q = position; q != _M_finish; ++q, ++new_finish) new (new_finish) SPair(*q);
        if (_M_start) std::free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

static SPair Make(int k) { SPair p; p.first = k; p.second.GroupID = k; p.second.NameTablePos = k * 10; p.second.AllowVariationWord = 1; return p; }

int main()
{
    int fails = 0;
    if (sizeof(SPair) != 16u) ++fails;
    SVector v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.insert_overflow(v._M_finish, Make(1), 1, true);                  // empty: capacity 1
    if (v.size() != 1u || v._M_end_of_storage != v._M_start + 1 || v._M_start[0].first != 1) ++fails;
    v.insert_overflow(v._M_finish, Make(2), 1, true);                  // full: capacity doubles to 2
    if (v.size() != 2u || v._M_end_of_storage != v._M_start + 2 || v._M_start[1].second.NameTablePos != 20) ++fails;
    v.insert_overflow(v._M_start + 1, Make(7), 3, false);              // middle insert of 3: capacity 2 + 3 = 5
    if (v.size() != 5u || v._M_end_of_storage != v._M_start + 5) ++fails;
    int expect[5] = { 1, 7, 7, 7, 2 };
    for (int i = 0; i < 5; ++i) if (v._M_start[i].first != expect[i]) ++fails;
    std::free(v._M_start);
    if (fails) { std::printf("0086cc50_TEST FAIL\n"); return 1; }
    std::printf("0086cc50_TEST PASS\n");
    return 0;
}