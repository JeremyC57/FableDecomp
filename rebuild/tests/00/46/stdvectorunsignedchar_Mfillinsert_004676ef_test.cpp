// Behaviour test for 0x004676EF vector<unsigned char>::_M_fill_insert (STLport POD model): memmove tail, memset gap, overflow hand-off.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct __true_type { };
static int g_overflow_calls = 0;

static unsigned char* __uninitialized_copy(const unsigned char* first, const unsigned char* last, unsigned char* result, const __true_type&)
{
    if (last == first) return result;
    std::memmove(result, first, last - first);
    return result + (last - first);
}
static void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;
    return num > 0 ? std::memmove((char*)result - num, first, num) : result;
}

struct ByteVector {
    unsigned char* _M_start; unsigned char* _M_finish; unsigned char* _M_end_of_storage;
    void _M_insert_overflow(unsigned char* position, const unsigned char& x, const __true_type&, unsigned int n, bool atEnd = false)
    {
        ++g_overflow_calls;
        unsigned int old = _M_finish - _M_start;
        unsigned int len = old + (old < n ? n : old);
        unsigned char* s = (unsigned char*)std::malloc(len ? len : 1);
        unsigned char* f = __uninitialized_copy(_M_start, position, s, __true_type());
        for (unsigned int i = 0; i < n; ++i) *f++ = x;
        if (!atEnd) f = __uninitialized_copy(position, _M_finish, f, __true_type());
        std::free(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
    void _M_fill_insert(unsigned char* position, unsigned int n, const unsigned char& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                unsigned char x_copy = x;
                const unsigned int after = _M_finish - position;
                unsigned char* old_finish = _M_finish;
                if (after > n) {
                    __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __true_type());
                    _M_finish += n;
                    __copy_trivial_backward(position, old_finish - n, old_finish);
                    for (unsigned char* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    for (unsigned int k = n - after; k > 0; --k) _M_finish[k - 1] = x_copy;
                    _M_finish += n - after;
                    __uninitialized_copy(position, old_finish, _M_finish, __true_type());
                    _M_finish += after;
                    for (unsigned char* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                _M_insert_overflow(position, x, __true_type(), n);
        }
    }
};

static int Run(int size, int cap, int pos, int n)
{
    ByteVector v;
    v._M_start = (unsigned char*)std::malloc(cap ? cap : 1);
    for (int i = 0; i < size; ++i) v._M_start[i] = (unsigned char)('a' + i);
    v._M_finish = v._M_start + size; v._M_end_of_storage = v._M_start + cap;
    unsigned char x = 'Z';
    v._M_fill_insert(v._M_start + pos, n, x);
    int ok = (int)(v._M_finish - v._M_start) == size + n;
    for (int i = 0; ok && i < size + n; ++i) {
        unsigned char e = i < pos ? (unsigned char)('a' + i) : (i < pos + n ? 'Z' : (unsigned char)('a' + i - n));
        ok = v._M_start[i] == e;
    }
    std::free(v._M_start);
    return ok;
}

int main()
{
    int ok = 1;
    ok &= Run(8, 20, 2, 3);    // tail longer than n
    ok &= Run(8, 20, 6, 9);    // tail shorter than n
    ok &= Run(8, 20, 8, 5);    // append
    ok &= Run(8, 20, 3, 0);    // n == 0
    int before = g_overflow_calls;
    ok &= Run(8, 9, 1, 4);     // overflow
    if (g_overflow_calls != before + 1) ok = 0;
    if (!ok) { std::printf("004676ef_TEST FAIL\n"); return 1; }
    std::printf("004676ef_TEST PASS\n");
    return 0;
}