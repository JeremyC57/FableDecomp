// Behaviour test for 0x007704a0 STLport vector::_M_insert_overflow (reallocating insert)
#include <cstdio>
#include <cstdlib>
#include <new>

static const char* g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l" };
struct __false_type { __false_type() { } };
struct CCharString { const char* PStringData; CCharString(const char* s) : PStringData(s) { } CCharString(const CCharString& o) : PStringData(o.PStringData) { } };
struct TElem { CCharString first, second; TElem(int k) : first(g_names[k]), second(g_names[k + 1]) { } TElem(const TElem& o) : first(o.first), second(o.second) { } };
static bool Is(const TElem& e, int k) { return e.first.PStringData == g_names[k] && e.second.PStringData == g_names[k + 1]; }

static TElem* UninitializedCopy(TElem* first, TElem* last, TElem* result, const __false_type&)
{
    for (; first != last; ++first, ++result) new (result) TElem(*first);
    return result;
}
static TElem* UninitializedFillN(TElem* first, unsigned int n, const TElem& x, const __false_type&)
{
    TElem* cur = first;
    for (; n > 0; --n, ++cur) new (cur) TElem(x);
    return cur;
}
struct TVector {
    TElem* _M_start;
    TElem* _M_finish;
    TElem* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { std::free(_M_start); }   /* 0x007701C0 stand-in */
    void _M_insert_overflow(TElem* position, const TElem& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        TElem* new_start = len != 0 ? (TElem*)std::malloc(len * sizeof(TElem)) : 0;
        TElem* new_finish = UninitializedCopy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) { new (new_finish) TElem(x); ++new_finish; }
        else new_finish = UninitializedFillN(new_finish, fill_len, x, __false_type());
        if (!atend) new_finish = UninitializedCopy(position, _M_finish, new_finish, __false_type());
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    TVector v = { 0, 0, 0 };
    v._M_insert_overflow(v._M_finish, TElem(0), __false_type(), 1, true);      // [0], cap 1
    v._M_insert_overflow(v._M_finish, TElem(2), __false_type(), 1, true);      // [0 2], cap 2
    if (v.size() != 2 || v._M_end_of_storage - v._M_start != 2 || !Is(v._M_start[0], 0) || !Is(v._M_start[1], 2)) { std::printf("007704a0_TEST FAIL\n"); return 1; }
    v._M_insert_overflow(v._M_start + 1, TElem(4), __false_type(), 3, false);   // [0 4 4 4 2], cap 2 + 3
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5) { std::printf("007704a0_TEST FAIL\n"); return 1; }
    if (!Is(v._M_start[0], 0) || !Is(v._M_start[1], 4) || !Is(v._M_start[3], 4) || !Is(v._M_start[4], 2)) { std::printf("007704a0_TEST FAIL\n"); return 1; }
    v._M_insert_overflow(v._M_start, TElem(6), __false_type(), 1, false);       // [6 0 4 4 4 2], cap 10
    if (v.size() != 6 || v._M_end_of_storage - v._M_start != 10 || !Is(v._M_start[0], 6) || !Is(v._M_start[5], 2)) { std::printf("007704a0_TEST FAIL\n"); return 1; }
    std::printf("007704a0_TEST PASS\n");
    return 0;
}