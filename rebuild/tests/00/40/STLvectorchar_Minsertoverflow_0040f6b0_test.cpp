#include <cstdio>
#include <cstdlib>
#include <cstring>

// Model of 0x0040F6B0 STLport vector<char>::_M_insert_overflow(position, x, __true_type, fill_len, atend): allocate
// old_size + max(old_size, fill_len), copy the head, memset fill_len copies of x, copy the tail unless atend, free
// the old block and set start/finish/end_of_storage.
static int g_frees = 0;
static void CountedFree(void* p) { ++g_frees; std::free(p); }
struct __true_type { };
template <class _Tp> inline const _Tp& Max(const _Tp& a, const _Tp& b) { return a < b ? b : a; }
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)std::memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
inline char* FillN(char* first, unsigned int n, const char& value)
{
    char tmp = value;
    std::memset(first, (unsigned char)tmp, n);
    return first + n;
}
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)std::malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p) { if (p) CountedFree(p); }
    void _M_clear() { deallocate(_M_start); }
    void _M_set(_Tp* s, _Tp* f, _Tp* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + Max(old_size, fill_len);
        _Tp* new_start = allocate(len);
        _Tp* new_finish = (_Tp*)CopyTrivial(_M_start, position, new_start);
        new_finish = FillN(new_finish, fill_len, x);
        if (!atend)
            new_finish = (_Tp*)CopyTrivial(position, _M_finish, new_finish);
        _M_clear();
        _M_set(new_start, new_finish, new_start + len);
    }
};

static int Fail()
{
    std::printf("0040f6b0_TEST FAIL\n");
    return 1;
}

int main()
{
    vector<char> v;
    v._M_start = (char*)std::malloc(6);
    std::memcpy(v._M_start, "abcdef", 6);
    v._M_finish = v._M_end_of_storage = v._M_start + 6;
    v._M_insert_overflow(v._M_start + 2, 'Z', __true_type(), 3, false);       // middle insert: len 6 + max(6,3)
    if (v.size() != 9 || v._M_end_of_storage - v._M_start != 12 || std::memcmp(v._M_start, "abZZZcdef", 9) != 0 || g_frees != 1)
        return Fail();
    v._M_insert_overflow(v._M_finish, 'q', __true_type(), 20, true);          // append past the end: len 9 + 20
    if (v.size() != 29 || v._M_end_of_storage - v._M_start != 29 || std::memcmp(v._M_start, "abZZZcdef", 9) != 0 ||
        v._M_start[9] != 'q' || v._M_start[28] != 'q' || g_frees != 2)
        return Fail();
    vector<char> e;
    e._M_start = e._M_finish = e._M_end_of_storage = 0;
    e._M_insert_overflow(e._M_finish, 'x', __true_type(), 4, true);           // empty vector: nothing to free
    if (e.size() != 4 || e._M_end_of_storage - e._M_start != 4 || std::memcmp(e._M_start, "xxxx", 4) != 0 || g_frees != 2)
        return Fail();
    std::free(v._M_start);
    std::free(e._M_start);
    std::printf("0040f6b0_TEST PASS\n");
    return 0;
}