// behaviour test for 0x009B4D80 vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >::reserve, linked against the
// built member: growing moves the entries into a fresh block through _M_allocate_and_copy and frees the old one, an
// empty vector just allocates, a capacity already >= n changes nothing
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
class CDefClassBase {
public:
    class CSubDefInfo { public: long DefIndex; long OriginalParentDefIndex; };
};
namespace std {
template <class T> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    template <class ForwardIter> T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);
    void reserve(unsigned int n);
};
template <class T, class A> template <class ForwardIter>
T* vector<T, A>::_M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
{
    T* result = n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0;
    T* cur = result;
    for (; first != last; ++first, ++cur) { new (cur) T(*first); ++g_copies; }
    return result;
}
}
typedef std::pair<unsigned long, CDefClassBase::CSubDefInfo> E;
template E* std::vector<E>::_M_allocate_and_copy<const E*>(unsigned int, const E*, const E*);
int main()
{
    std::vector<E> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.reserve(2);
    bool ok = v._M_start != 0 && v._M_finish == v._M_start && v._M_end_of_storage == v._M_start + 2 && g_copies == 0;
    for (int i = 0; i < 2; ++i) { v._M_finish->first = 0x100u + i; v._M_finish->second.DefIndex = i; v._M_finish->second.OriginalParentDefIndex = -1 - i; ++v._M_finish; }
    E* old = v._M_start;
    v.reserve(2);
    ok = ok && v._M_start == old && g_copies == 0;
    v.reserve(7);
    ok = ok && v._M_start != 0 && v._M_finish == v._M_start + 2 && v._M_end_of_storage == v._M_start + 7 && g_copies == 2;
    for (int i = 0; i < 2; ++i)
        ok = ok && v._M_start[i].first == 0x100u + i && v._M_start[i].second.DefIndex == i && v._M_start[i].second.OriginalParentDefIndex == -1 - i;
    std::free(v._M_start);
    if (!ok) { std::printf("009b4d80_TEST FAIL\n"); return 1; }
    std::printf("009b4d80_TEST PASS\n");
    return 0;
}
