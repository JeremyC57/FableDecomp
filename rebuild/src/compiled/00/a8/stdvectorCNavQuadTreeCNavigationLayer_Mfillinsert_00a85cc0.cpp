// std::vector<CNavQuadTree::CNavigationLayer>::_M_fill_insert(iterator pos, size_type n, const value_type& x) -- STLport fill insert of n copies of one navigation layer: in place through a local copy of x when the spare capacity holds n (shift the tail, then fill), else hand over to _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCNavigationLayer@CNavQuadTree@@V?$allocator@VCNavigationLayer@CNavQuadTree@@@std@@@std@@QAEXPAVCNavigationLayer@CNavQuadTree@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85CC0, 342 bytes, thiscall `ret 0Ch`.
// Identity (proven): the landed vector<CNavQuadTree::CNavigationLayer>::resize 0x00A85EC0 calls it; its overflow branch is
//   this vector's _M_insert_overflow 0x00A85B90. The landed resize binds it as a protected member; STLport 4.6 declares
//   _M_fill_insert public (NOTES.md, Review at landing).
// Callees: the three member vector copy constructors 0x00A80B80 / 0x00A83170 / 0x00A83A30 (the local x copy, inline
//   implicit CNavigationLayer copy) and CNavigationLayer's out-of-line implicit destructor 0x00A851E0;
//   __uninitialized_copy 0x00A85420, __copy_backward 0x00A85740 (reached through the inline __copy_backward_ptrs with the
//   random-access tag and the null distance pointer), fill 0x00A85810, __uninitialized_fill_n 0x00A85560,
//   _M_insert_overflow 0x00A85B90.
// Shape: STLport 4.6 vector.c `_M_fill_insert` verbatim (no EH); the tag-taking helpers are same-TU noinline stand-ins
//   that never read their tags, so the tags are homed on argument slots as in retail.
// Layout: engine/CNavQuadTree.h has no nested CNavigationLayer; it is included under a layout name and CNavQuadTree is
//   re-derived from it to carry the nested class (the donor pattern).
#define CNavQuadTree CNavQuadTreeLayout
#include "engine/CNavQuadTree.h"
#undef CNavQuadTree
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Start, End; };
class CANavQuadTreeNode;
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                         // 4-byte STLport list (sentinel pointer)
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    typedef _Tp* iterator;
    vector(const vector<_Tp, _Alloc>& __x);                // out of line
    ~vector();                                             // out of line
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);         // 0x00A85B90
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
        ~CNavigationLayer();                               // 0x00A851E0 (implicit, out of line)
    };
};
namespace std {
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{   // stand-in for 0x00A85420 (only the target row is graded)
    for (; __first != __last; ++__first, ++__result)
        __result->SwitchableLineLists.Dirty = __first->SwitchableLineLists.Dirty;
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{   // stand-in for 0x00A85560
    for (; __n > 0; --__n, ++__first)
        __first->SwitchableLineLists.Dirty = __x.SwitchableLineLists.Dirty;
    return __first;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{   // stand-in for 0x00A85740
    for (_Distance __n = __last - __first; __n > 0; --__n)
        (--__result)->SwitchableLineLists.Dirty = (--__last)->SwitchableLineLists.Dirty;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{   // stand-in for 0x00A85810
    for (; __first != __last; ++__first)
        __first->SwitchableLineLists.Dirty = __val.SwitchableLineLists.Dirty;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                __uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy, __false_type());
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}
typedef CNavQuadTree::CNavigationLayer TNavigationLayer;
template void std::vector<TNavigationLayer>::_M_fill_insert(TNavigationLayer*, unsigned int, const TNavigationLayer&);