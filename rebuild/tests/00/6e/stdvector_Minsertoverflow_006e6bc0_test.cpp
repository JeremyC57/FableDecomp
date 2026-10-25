// Behaviour test for 0x006E6BC0 vector<CConversationLine>::_M_insert_overflow (model copy of the STLport algorithm).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct Elem {                                          // stands in for CConversationLine (string member)
    long key; long* data;
    Elem(long k) : key(k), data((long*)std::malloc(sizeof(long))) { *data = k * 10; ++g_live; }
    Elem(const Elem& o) : key(o.key), data((long*)std::malloc(sizeof(long))) { *data = *o.data; ++g_live; }
    ~Elem() { std::free(data); --g_live; }
};

template <class T> struct Vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static T* ucopy(const T* f, const T* l, T* r) { for (; f != l; ++f, ++r) new (r) T(*f); return r; }
    void _M_clear()
    {
        for (T* p = _M_start; p != _M_finish; ++p) p->~T();
        if (_M_start) std::free(_M_start);
    }
    void _M_insert_overflow(T* pos, const T& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* new_finish = ucopy(_M_start, pos, new_start);
        if (fill_len == 1) { new (new_finish) T(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) T(x);
        if (!atend) new_finish = ucopy(pos, _M_finish, new_finish);
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    Vec<Elem> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    Elem a(1), b(2), c(3);
    v._M_insert_overflow(v._M_finish, a, 1, true);                  // push_back into empty: capacity 1
    int ok = v.size() == 1 && v._M_end_of_storage - v._M_start == 1 && v._M_start[0].key == 1;
    v._M_insert_overflow(v._M_finish, b, 1, true);                  // grows to 2
    ok = ok && v.size() == 2 && v._M_end_of_storage - v._M_start == 2 && v._M_start[1].key == 2;
    v._M_insert_overflow(v._M_start + 1, c, 3, false);              // insert 3 copies in the middle: len = 2 + 3
    ok = ok && v.size() == 5 && v._M_end_of_storage - v._M_start == 5;
    ok = ok && v._M_start[0].key == 1 && v._M_start[1].key == 3 && v._M_start[3].key == 3 && v._M_start[4].key == 2;
    ok = ok && *v._M_start[4].data == 20 && *v._M_start[2].data == 30;
    ok = ok && g_live == 3 + 5;
    v._M_clear();
    if (!ok || g_live != 3) { std::printf("006e6bc0_TEST FAIL\n"); return 1; }
    std::printf("006e6bc0_TEST PASS\n");
    return 0;
}