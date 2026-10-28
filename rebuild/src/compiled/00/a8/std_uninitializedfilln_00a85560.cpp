// std::__uninitialized_fill_n(CNavQuadTree::CNavigationLayer* first, unsigned int n, const CNavQuadTree::CNavigationLayer& x, const __false_type&) -- placement copy-construct n copies of one navigation layer (node-pointer array, line lists, switchable line-list map) into raw storage, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCNavigationLayer@CNavQuadTree@@IV12@@std@@YIPAVCNavigationLayer@CNavQuadTree@@PAV12@IABV12@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85560, 308 bytes, /Gr `__fastcall` (first ecx, n edx; x and the tag reference on
//   the stack), `ret 8`.
// Identity (proven): its callers are vector<CNavQuadTree::CNavigationLayer>'s _M_insert_overflow 0x00A85B90 and
//   _M_fill_insert 0x00A85CC0 (both bound to that vector by landed callers), and the 0x28-byte element copy is
//   CNavigationLayer's implicit copy constructor -- the member order of the landed CNavigationLayer::operator= 0x00A85700:
//   Nodes (CArray<CANavQuadTreeNode*>) +0x0, LineLists (vector<list<C2DLineF> >) +0xC, SwitchableLineLists
//   (CVectorMap<unsigned __int64, list<C2DLineF>, ...>: Data +0x18, empty Compare +0x24, Dirty +0x25).
// Callees: malloc 0x00BFEA0E (each vector's _Vector_base(n)), memmove 0x00BFEAE6 (the pointer array's __copy_trivial),
//   the landed list<C2DLineF> __uninitialized_copy 0x00A82730 and pair<unsigned __int64, list<C2DLineF> >
//   __uninitialized_copy 0x00A83340 (__fastcall, tag on the stack).
// Shape: STLport 4.6 `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` (unsigned n: `jbe`), placement-new null test,
//   the three STLport vector copy constructors inline (zeroed base, allocate(x.size()), then the POD memmove or the
//   out-of-line element copy).
// Layout: engine/CNavQuadTree.h has no nested CNavigationLayer; it is included under a layout name and CNavQuadTree is
//   re-derived from it to carry the nested class (the donor pattern).
#define CNavQuadTree CNavQuadTreeLayout
#include "engine/CNavQuadTree.h"
#undef CNavQuadTree
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Start, End; };
class CANavQuadTreeNode;
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type is_POD_type; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type is_POD_type; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                         // 4-byte STLport list (sentinel pointer)
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{   // stand-in for 0x00A82730 / 0x00A83340 (only the target row is graded)
    for (; __first != __last; ++__first, ++__result)
        *(void**)__result = *(void* const*)__first;
    return __result;
}
#pragma auto_inline(on)
template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, _IsPODType());
    }
};
}
template <class T> class CArray : public std::vector<T> { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {    // retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                         // +0x00
    C                        Compare;                      // +0x0C (empty)
    bool                     Dirty;                        // +0x0D
};
class CNavQuadTree : public CNavQuadTreeLayout {
public:
    class CNavigationLayer {                               // 0x28 bytes, implicit copy constructor
    public:
        CArray<CANavQuadTreeNode*>          Nodes;         // +0x00
        std::vector<std::list<C2DLineF> >   LineLists;     // +0x0C
        CVectorMap<unsigned __int64, std::list<C2DLineF>,
                   CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > > SwitchableLineLists;   // +0x18
    };
};
namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CNavQuadTree::CNavigationLayer* __fastcall __uninitialized_fill_n(CNavQuadTree::CNavigationLayer*, unsigned int, const CNavQuadTree::CNavigationLayer&, const __false_type&);
}