// std::vector<CNavQuadTree::CNavigationLayer>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth insert of 40-byte navigation layers: allocate size + max(size, fill_len), copy the head, x once (inline layer copy) or fill_len times, the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCNavigationLayer@CNavQuadTree@@V?$allocator@VCNavigationLayer@CNavQuadTree@@@std@@@std@@IAEXPAVCNavigationLayer@CNavQuadTree@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85B90, 289 bytes, thiscall `ret 14h` (five arguments).
// Identity (proven): the landed push_back 0x00A85E40 of CNavQuadTree::NavigationLayers (vector<CNavQuadTree::CNavigationLayer>,
//   Ego_r.pdb) and this vector's _M_fill_insert 0x00A85CC0 call it; the element copy is CNavigationLayer's implicit copy
//   constructor (member order of the landed CNavigationLayer::operator= 0x00A85700). The landed push_back binds it with a
//   public member; STLport 4.6 declares _M_insert_overflow protected (NOTES.md, Review at landing).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14; __uninitialized_copy 0x00A85420 (head and tail), __uninitialized_fill_n
//   0x00A85560 (this chunk), __destroy_aux 0x00A85930 (the inline _M_clear); for the single x the three member vector
//   copy constructors 0x00A80B80 (vector<CANavQuadTreeNode*>), 0x00A83170 (vector<list<C2DLineF> >), 0x00A83A30
//   (vector<pair<unsigned __int64, list<C2DLineF> > >) and the Dirty byte.
// Shape: STLport 4.6 vector.c verbatim (no EH), the 0x00A75010 model with _M_clear inline; the copy helpers are same-TU
//   noinline stand-ins that never read their tags, so the tags are homed on the atend slot as in retail.
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
    vector(const vector<_Tp, _Alloc>& __x);                // out of line
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
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
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{   // stand-in for 0x00A85930
    for (; __first != __last; ++__first)
        __first->SwitchableLineLists.Dirty = false;
}
#pragma auto_inline(on)
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp, class _Alloc>
inline void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    if (this->_M_start != 0) free(this->_M_start);
}
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = __len != 0 ? (pointer)malloc(__len * sizeof(_Tp)) : 0;
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}
typedef CNavQuadTree::CNavigationLayer TNavigationLayer;
template void std::vector<TNavigationLayer>::_M_insert_overflow(TNavigationLayer*, const TNavigationLayer&, const std::__false_type&, unsigned int, bool);