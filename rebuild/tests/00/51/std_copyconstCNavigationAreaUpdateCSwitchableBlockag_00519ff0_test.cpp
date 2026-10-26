// Behaviour test for 0x00519FF0 std::__copy<const CNavigationAreaUpdate::CSwitchableBlockage*, CSwitchableBlockage*, int> (UniqueID, list<C2DLineF>; pair-shaped model): each of the last - first
// records gets its key copied and its list assigned through list::operator= (source / destination recorded, in order),
// the return is result + n, and an empty range returns result untouched.
#include <cstdio>
#ifdef _MSC_VER
#define K64(x) x##ui64
#else
#define __fastcall
#define __int64 long long
#define K64(x) x##ULL
#endif
struct random_access_iterator_tag {};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
static const void* g_from[8]; static const void* g_to[8]; static int g_n;
template <class T> class list {
public:
    _List_node_base* _M_node;
    list<T>& operator=(const list<T>& x) { g_from[g_n] = &x; g_to[g_n] = this; ++g_n; _M_node = x._M_node; return *this; }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
struct C2DLineF { float a, b, c, d; };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
typedef pair<unsigned __int64, list<C2DLineF> > R;

int main()
{
    static _List_node_base nodes[3];
    R src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].first = K64(0x100000000) * (i + 1) + 7; src[i].second._M_node = &nodes[i]; }
    for (int i = 0; i < 4; ++i) { dst[i].first = 0; dst[i].second._M_node = 0; }
    random_access_iterator_tag tag;
    int ok = __copy(src, src, dst, tag, (int*)0) == dst && g_n == 0;
    R* end = __copy(src, src + 3, dst, tag, (int*)0);
    ok &= end == dst + 3 && g_n == 3;
    for (int i = 0; i < 3; ++i)
        ok &= dst[i].first == src[i].first && dst[i].second._M_node == &nodes[i] && g_from[i] == &src[i].second
              && g_to[i] == &dst[i].second;
    ok &= dst[3].first == 0 && dst[3].second._M_node == 0;
    if (!ok) { std::printf("00519ff0_TEST FAIL\n"); return 1; }
    std::printf("00519ff0_TEST PASS\n");
    return 0;
}