// Behaviour test for 0x00AD6B50 _STL::fill(QuadricOptimiserInternals::CUniqueVertex*, ...): every unique-vertex record of the range becomes a copy of the value (exists flag, then the vertex and face index sets through the tree operator=)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
namespace _STL {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Rb_tree& operator=(const _Rb_tree& __x)          // stand-in
    {
        _M_header = __x._M_header; _M_node_count = __x._M_node_count;
        ++gAssigns;
        return *this;
    }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    _Rb_tree<_Key, _Key, _Compare> _M_t;
    set& operator=(const set& __x) { _M_t = __x._M_t; return *this; }
};
}
namespace QuadricOptimiserInternals {
class CIndexList {
public:
    _STL::set<unsigned long> Indices;
};
class CUniqueVertex {
public:
    bool       Exists;
    CIndexList VertexIndices;
    CIndexList FaceList;
};
}
typedef QuadricOptimiserInternals::CUniqueVertex CUniqueVertex;
static int gVertHeader, gFaceHeader;
static CUniqueVertex gArr[4];
namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
int main()
{
    CUniqueVertex v;
    v.Exists = true;
    v.VertexIndices.Indices._M_t._M_header = &gVertHeader; v.VertexIndices.Indices._M_t._M_node_count = 4;
    v.FaceList.Indices._M_t._M_header = &gFaceHeader; v.FaceList.Indices._M_t._M_node_count = 2;
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].Exists = false; gArr[i].VertexIndices.Indices._M_t._M_node_count = 0; gArr[i].FaceList.Indices._M_t._M_node_count = 0; }
    bool ok = true;
    _STL::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].Exists && gArr[i].VertexIndices.Indices._M_t._M_header == (void*)&gVertHeader &&
             gArr[i].VertexIndices.Indices._M_t._M_node_count == 4 && gArr[i].FaceList.Indices._M_t._M_header == (void*)&gFaceHeader &&
             gArr[i].FaceList.Indices._M_t._M_node_count == 2;
    ok = ok && !gArr[3].Exists && gAssigns == 6;
    _STL::fill(gArr, gArr, v);
    ok = ok && gAssigns == 6;
    if (ok) { printf("00ad6b50_TEST PASS\n"); return 0; }
    printf("00ad6b50_TEST FAIL\n");
    return 1;
}