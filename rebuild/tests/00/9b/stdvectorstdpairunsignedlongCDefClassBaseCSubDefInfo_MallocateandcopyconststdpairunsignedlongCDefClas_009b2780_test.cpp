// behaviour test for 0x009B2780 vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >::_M_allocate_and_copy, linked
// against the built member: n != 0 allocates n * 12 bytes and copy-constructs [first, last) into the front; n == 0
// allocates nothing and returns null (empty range)
#include <cstdio>
#include <cstdlib>
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
protected:                                         // protected, as in the source (STLport 4.6)
    template <class ForwardIter> T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);
public:
};
}
typedef std::pair<unsigned long, CDefClassBase::CSubDefInfo> E;
template <class V> struct CopyAccess : public V { using V::_M_allocate_and_copy; };   // the test reaches the protected member
int main()
{
    if (sizeof(E) != 12) { std::printf("009b2780_TEST FAIL size\n"); return 1; }
    E src[3];
    for (int i = 0; i < 3; ++i) { src[i].first = 0xA0u + i; src[i].second.DefIndex = i * 7; src[i].second.OriginalParentDefIndex = -i; }
    std::vector<E> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    E* p = static_cast<CopyAccess<std::vector<E> >&>(v)._M_allocate_and_copy<const E*>(5, src, src + 3);
    if (p == 0) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (p[i].first != 0xA0u + i || p[i].second.DefIndex != i * 7 || p[i].second.OriginalParentDefIndex != -i) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    std::free(p);
    E* q = static_cast<CopyAccess<std::vector<E> >&>(v)._M_allocate_and_copy<const E*>(0, src, src);
    if (q != 0) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    std::printf("009b2780_TEST PASS\n");
    return 0;
}
