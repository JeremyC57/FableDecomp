// Link test for 0x00A82650: the source object's __copy over list<C2DLineF> assigns each one-word list in order through
// list<C2DLineF>::operator= (0x00517720, supplied here) and returns the advanced result.
#include <cstdio>
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Point1; C2DVector Point2; };
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    void* _M_node;
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);
};
struct random_access_iterator_tag { };
static int g_assigns = 0;
template <> list<C2DLineF>& list<C2DLineF>::operator=(const list<C2DLineF>& __x) { ++g_assigns; _M_node = __x._M_node; return *this; }
}
typedef std::list<C2DLineF> CLineList;
CLineList* __fastcall Copy_SListHandle04_00517720_00a82650(const CLineList* first, const CLineList* last, CLineList* result, const std::random_access_iterator_tag& category, int* distance);
int main()
{
    static int nodes[3];
    CLineList src[3], dst[4];
    for (int i = 0; i < 3; ++i) src[i]._M_node = &nodes[i];
    dst[3]._M_node = 0;
    std::random_access_iterator_tag tag;
    CLineList* r = Copy_SListHandle04_00517720_00a82650(src, src + 3, dst, tag, 0);
    int ok = r == dst + 3 && std::g_assigns == 3 && dst[0]._M_node == &nodes[0] && dst[2]._M_node == &nodes[2] && dst[3]._M_node == 0;
    ok = ok && Copy_SListHandle04_00517720_00a82650(src, src, dst + 1, tag, 0) == dst + 1 && std::g_assigns == 3;
    if (!ok) { std::printf("00a82650_TEST FAIL\n"); return 1; }
    std::printf("00a82650_TEST PASS\n");
    return 0;
}
