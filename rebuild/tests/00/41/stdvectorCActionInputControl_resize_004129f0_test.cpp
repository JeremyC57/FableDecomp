#include <cstdio>
class CActionInputControl {
public:
    long Fields[7];                                      // GameAction .. ControlDirection (0x1c bytes)
};
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class T> class allocator {};
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*);   // 0x0040E2D0 for CActionInputControl
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
        iterator i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(iterator pos, unsigned int n, const T& x);   // 0x00411DB0
    void insert(iterator pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<CActionInputControl>::resize(unsigned int, const CActionInputControl&);
}

namespace std {
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = (Distance)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
static int g_fills; static unsigned int g_n; static const CActionInputControl* g_x; static CActionInputControl* g_pos;
template <> void vector<CActionInputControl>::_M_fill_insert(iterator pos, unsigned int n, const CActionInputControl& x)
{ ++g_fills; g_pos = pos; g_n = n; g_x = &x; for (unsigned int i = 0; i < n; ++i) *_M_finish++ = x; }
}
int main()
{
    static CActionInputControl store[10];
    std::vector<CActionInputControl> v; v._M_start = store; v._M_finish = store + 5; v._M_end_of_storage = store + 10;
    for (int i = 0; i < 5; ++i) store[i].Fields[0] = i;
    CActionInputControl x; for (int j = 0; j < 7; ++j) x.Fields[j] = 9;
    v.resize(3, x);
    bool ok = v._M_finish == store + 3 && std::g_fills == 0;
    v.resize(6, x);
    ok = ok && std::g_fills == 1 && std::g_n == 3 && std::g_pos == store + 3 && std::g_x == &x && v._M_finish == store + 6 && store[5].Fields[6] == 9;
    v.resize(6, x);
    ok = ok && std::g_fills == 2 && std::g_n == 0;
    std::printf(ok ? "004129f0_TEST PASS\n" : "004129f0_TEST FAIL\n");
    return ok ? 0 : 1;
}