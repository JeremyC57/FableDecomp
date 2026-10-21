#include <cstdio>
class CWideString {
public:
    void* PData;
};
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    iterator erase(iterator first, iterator last);       // 0x00412450 for CWideString
    void _M_fill_insert(iterator pos, unsigned int n, const T& x);   // 0x004126E0 for CWideString
    void insert(iterator pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<CWideString>::resize(unsigned int, const CWideString&);
}

static int g_erase = 0, g_fill = 0; static CWideString* g_a = 0; static CWideString* g_b = 0; static unsigned int g_n = 0; static const CWideString* g_x = 0;
namespace std {
template <class T, class A> typename vector<T, A>::iterator vector<T, A>::erase(iterator first, iterator last) { ++g_erase; g_a = first; g_b = last; _M_finish = first; return first; }
template <class T, class A> void vector<T, A>::_M_fill_insert(iterator pos, unsigned int n, const T& x) { ++g_fill; g_a = pos; g_n = n; g_x = &x; }
}
int main()
{
    static CWideString buf[10]; CWideString x;
    std::vector<CWideString> v; v._M_start = buf; v._M_finish = buf + 5; v._M_end_of_storage = buf + 10;
    v.resize(3, x);
    bool ok = g_erase == 1 && g_a == buf + 3 && g_b == buf + 5 && g_fill == 0;
    v.resize(8, x);
    ok = ok && g_fill == 1 && g_a == buf + 3 && g_n == 5 && g_x == &x;
    v.resize(3, x);
    ok = ok && g_fill == 2 && g_n == 0 && g_erase == 1;
    if (ok) std::printf("00412b20_TEST PASS\n");
    return 0;
}