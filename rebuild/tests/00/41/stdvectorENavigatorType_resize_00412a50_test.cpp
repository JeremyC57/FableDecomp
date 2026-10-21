#include <cstdio>
enum ENavigatorType { NAVIGATOR_TYPE_NONE };
namespace std {
template <class T> class allocator {};
template <class InputIter, class OutputIter>
inline OutputIter __copy(InputIter first, InputIter last, OutputIter result)
{
    for (long n = last - first; n > 0; --n)
    {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
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
        iterator i = __copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(iterator pos, unsigned int n, T x);   // 0x00411F10
    void insert(iterator pos, unsigned int n, T x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, T x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<ENavigatorType>::resize(unsigned int, ENavigatorType);
}


static unsigned int g_pos_off, g_n; static int g_x = -1;
template <> void std::vector<ENavigatorType>::_M_fill_insert(iterator pos, unsigned int n, ENavigatorType x)
{ g_pos_off = unsigned int(pos - _M_start); g_n = n; g_x = x; for (unsigned int i = 0; i < n; ++i) *_M_finish++ = x; }
int main()
{
    static ENavigatorType buf[16] = { (ENavigatorType)1, (ENavigatorType)2, (ENavigatorType)3, (ENavigatorType)4 };
    std::vector<ENavigatorType> v; v._M_start = buf; v._M_finish = buf + 4; v._M_end_of_storage = buf + 16;
    v.resize(2, (ENavigatorType)9);
    bool ok = v._M_finish == buf + 2 && g_x == -1;
    v.resize(5, (ENavigatorType)7);
    ok = ok && g_pos_off == 2 && g_n == 3 && g_x == 7 && v._M_finish == buf + 5 && buf[4] == 7 && buf[1] == 2;
    v.resize(5, (ENavigatorType)8); ok = ok && g_n == 0;
    if (ok) std::printf("00412a50_TEST PASS\n");
    return 0;
}