// Behaviour test for 0x008EC070 std::vector<NCreatureMode::CModeHoverBase::CHoverState>::_M_insert_overflow(position, x, __false_type, fill_len, atend)
// The model mirrors the source: STLport 4.6 _M_insert_overflow over an element whose copy constructor / destructor
// are counted (stand-ins for the retail out-of-line string copy / destructor or element copy constructor).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;

struct Elem {
    long m_key;
    long m_words[5];
    Elem(long k) : m_key(k) { for (int i = 0; i < 5; ++i) m_words[i] = k * 10 + i; ++g_live; }
    Elem(const Elem& o) : m_key(o.m_key) { for (int i = 0; i < 5; ++i) m_words[i] = o.m_words[i]; ++g_live; }
    ~Elem() { --g_live; }
};

namespace tv {
struct __false_type { __false_type() { } };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
inline Elem* ucopy(Elem* f, Elem* l, Elem* r) { for (; f != l; ++f, ++r) new (r) Elem(*f); return r; }
inline Elem* ufill(Elem* f, unsigned int n, const Elem& x) { for (; n > 0; --n, ++f) new (f) Elem(x); return f; }

struct vec {
    Elem* _M_start;
    Elem* _M_finish;
    Elem* _M_end_of_storage;
    vec() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vec() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        for (Elem* p = _M_start; p != _M_finish; ++p) p->~Elem();
        if (_M_start) std::free(_M_start);
    }
    void _M_insert_overflow(Elem* __position, const Elem& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        const unsigned int __old_size = size();
        const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
        Elem* __new_start = __len != 0 ? (Elem*)std::malloc(__len * sizeof(Elem)) : 0;
        Elem* __new_finish = ucopy(_M_start, __position, __new_start);
        if (__fill_len == 1) { new (__new_finish) Elem(__x); ++__new_finish; }
        else __new_finish = ufill(__new_finish, __fill_len, __x);
        if (!__atend) __new_finish = ucopy(__position, _M_finish, __new_finish);
        _M_clear();
        _M_start = __new_start; _M_finish = __new_finish; _M_end_of_storage = __new_start + __len;
    }
    void push_back(const Elem& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) Elem(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
}

int main()
{
    int fail = 0;
    {
        tv::vec v;
        for (long i = 0; i < 5; ++i) v.push_back(Elem(i));             // 0 -> 1 -> 2 -> 4 -> 8 capacity
        if (v.size() != 5 || (v._M_end_of_storage - v._M_start) != 8 || g_live != 5) fail |= 1;
        for (long i = 0; i < 5; ++i) if (v._M_start[i].m_key != i || v._M_start[i].m_words[5 - 1] != i * 10 + 5 - 1) fail |= 2;
        // insert 3 copies of 99 before element 2 (not at the end): capacity grows to size + max(size, 3)
        v._M_insert_overflow(v._M_start + 2, Elem(99), tv::__false_type(), 3, false);
        long want[8] = { 0, 1, 99, 99, 99, 2, 3, 4 };
        if (v.size() != 8 || (v._M_end_of_storage - v._M_start) != 10 || g_live != 8) fail |= 4;
        for (int i = 0; i < 8; ++i) if (v._M_start[i].m_key != want[i]) fail |= 8;
        // fill_len larger than the size: len = size + fill_len
        v._M_insert_overflow(v._M_finish, Elem(7), tv::__false_type(), 12, true);
        if (v.size() != 20 || (v._M_end_of_storage - v._M_start) != 20 || v._M_start[19].m_key != 7 || g_live != 20) fail |= 16;
    }
    if (g_live != 0) fail |= 32;
    if (fail) { std::printf("008ec070_TEST FAIL\n"); return 1; }
    std::printf("008ec070_TEST PASS\n");
    return 0;
}