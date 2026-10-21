#include <cstdio>
#include <string.h>
namespace std {
template <class T> class allocator {};
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    iterator erase(iterator first, iterator last)
    {
        iterator i = (iterator)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(iterator pos, unsigned int n, T x);   // 0x00441020
    void insert(iterator pos, unsigned int n, T x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, T x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<unsigned int>::resize(unsigned int, unsigned int);
}


static unsigned int g_pos_off, g_n; static unsigned int g_x = 12345;
template <> void std::vector<unsigned int>::_M_fill_insert(iterator pos, unsigned int n, unsigned int x)
{ g_pos_off = unsigned int(pos - _M_start); g_n = n; g_x = x; for (unsigned int i = 0; i < n; ++i) *_M_finish++ = x; }
int main()
{
    static unsigned int buf[16] = { 1u, 2u, 3u, 4u };
    std::vector<unsigned int> v; v._M_start = buf; v._M_finish = buf + 4; v._M_end_of_storage = buf + 16;
    v.resize(2, 9u);
    bool ok = v._M_finish == buf + 2 && g_x == 12345;
    v.resize(5, 7u);
    ok = ok && g_pos_off == 2 && g_n == 3 && g_x == 7 && v._M_finish == buf + 5 && buf[4] == 7 && buf[1] == 2;
    v.resize(5, 8u); ok = ok && g_n == 0;
    if (ok) std::printf("00442770_TEST PASS\n");
    return 0;
}