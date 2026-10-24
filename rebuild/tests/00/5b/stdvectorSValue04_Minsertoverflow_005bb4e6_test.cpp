// behaviour test for 005bb4e6: STLport vector<4-byte value>::_M_insert_overflow (grow to old + max(old, fill), copy head, fill, copy tail)
#include <cstdio>
#include <cstdlib>
#include <new>
struct __false_type { __false_type() {} };
struct SValue04 { unsigned int m_value; };
static int g_frees = 0;
static SValue04* UninitCopy(const SValue04* first, const SValue04* last, SValue04* result, const __false_type&)
{
    SValue04* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) SValue04(*first);
    return cur;
}
template <class T> const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct VectorModel {
    SValue04* _M_start;
    SValue04* _M_finish;
    SValue04* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static SValue04* allocate(unsigned int n) { return n != 0 ? (SValue04*)std::malloc(n * sizeof(SValue04)) : 0; }
    void _M_insert_overflow(SValue04* position, const SValue04& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + Max(old_size, fill_len);
        SValue04* new_start = allocate(len);
        SValue04* new_finish = UninitCopy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) { new (new_finish) SValue04(x); ++new_finish; }
        else { for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) SValue04(x); }
        if (!atend)
            new_finish = UninitCopy(position, _M_finish, new_finish, __false_type());
        if (_M_start) { std::free(_M_start); ++g_frees; }
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};
int main()
{
    int bad = 0;
    VectorModel v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    SValue04 x; x.m_value = 7;
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);          /* empty: len = 0 + max(0,1) = 1 */
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || v._M_start[0].m_value != 7 || g_frees != 0) ++bad;
    x.m_value = 8;
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);          /* len = 1 + 1 = 2 */
    if (v.size() != 2 || v._M_end_of_storage - v._M_start != 2 || v._M_start[1].m_value != 8 || g_frees != 1) ++bad;
    x.m_value = 9;
    v._M_insert_overflow(v._M_start + 1, x, __false_type(), 3, false);      /* middle insert of 3: len = 2 + 3 = 5, tail copied */
    unsigned int want[5] = { 7, 9, 9, 9, 8 };
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5 || g_frees != 2) ++bad;
    for (int i = 0; i < 5 && !bad; ++i) if (v._M_start[i].m_value != want[i]) ++bad;
    std::free(v._M_start);
    if (bad) { std::printf("005bb4e6_TEST FAIL\n"); return 1; }
    std::printf("005bb4e6_TEST PASS\n");
    return 0;
}