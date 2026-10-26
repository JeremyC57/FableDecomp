// std::vector::erase(iterator __first, iterator __last) -- (vector<CCardPositionsDef >) STLport range erase: assign the tail [last, finish) down over first, destroy the leftover elements, pull finish back, return first
// Flags: /Gr
// Leaf: ?erase@?$vector@VCCardPositionsDef@@V?$allocator@VCCardPositionsDef@@@std@@@std@@QAEPAVCCardPositionsDef@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9D7B, 60 bytes (size-optimised TU with a frame pointer), thiscall, `ret 8`.
// Identity (proven): std::vector<CCardPositionsDef >::erase(iterator, iterator); evidence: its only caller is std::vector<CCardPositionsDef >::resize 0x004F9D32 (n < size(): erase(begin() + n, end()), the size by `idiv 0x48`), called from CPersistContext::TransferVectorOfSubComponents<CCardPositionsDef> 0x004F9C07 of CTavernGameCardBaseDef::Transfer 0x004F9A90 (slot 18 of vtable 0x0124258C, RTTI .?AVCTavernGameCardBaseDef@@) on CTavernGameCardBaseDef::CardOrders at this+0x6C (Ego_r.pdb +0x6C, vector<CCardPositionsDef>; site 0x004F9B4E); the element is CCardPositionsDef (vtable 0x012439DC)
// Label: the manifest's "Vector_Insert_Element" is a behavioural label; this row is vector<CCardPositionsDef >::erase(first, last) (family J068: five 60-byte bodies, each its own element).
// Callees: __copy_ptrs(first, last, result, const __false_type&) 0x004F9DB7 (out of line, __fastcall in this /Gr TU, `ret 8`: the
//   implicit CCardPositionsDef::operator= inlined every 0x48 bytes after CParentDefClassBase::operator= 0x00431ED0), __destroy_aux(first, last, const __false_type&)
//   0x004DA61A (__fastcall, `ret 4`: the virtual destructor, slot 0 with flag 0, every 0x48 bytes; the bindings' ??$__destroy_aux@VCCardPositionsDef@@@std@@ spelling).
// Shape: STLport 4.6 vector.h `erase` verbatim. The _TrivialAss() tag is a constructor-less empty __false_type, value-initialised
//   (`stosb`) in its own frame byte for the unseen __copy_ptrs; the _Trivial_destructor() tag of __destroy_aux, whose body this
//   TU compiles first, gets no store and is homed in the top byte of the `first` slot (`lea eax,[ebp+0Bh]`).
// Element: CCardPositionsDef, 0x48 bytes in retail (vtable 0x012439DC slot 20 returns 0x48, base CParentDefClassBase), Ego_r.pdb members
//   rebased to retail (retail = Ego_r - 4 after the shorter CParentDefClassBase, each STLport container 12 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
struct __false_type { };
template <class _Tp> struct __type_traits {                // a class element: nothing is trivial
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
};
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _InputIter, class _OutputIter>
_OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&);   // 0x004F9DB7
template <class _Tp>
void __destroy_aux(_Tp* __first, _Tp* __last, const __false_type&);   // 0x004DA61A (defined below, ahead of erase)
template <class _Tp>
__forceinline void __destroy(_Tp* __first, _Tp* __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp>
__forceinline void _Destroy(_Tp* __first, _Tp* __last)
{
    __destroy(__first, __last, (_Tp*)0);
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class C2DVector;
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (natural packing)
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC, retail 0x39 (packed)
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
class CCardPositionsDef : public CParentDefClassBase {   // vtable 0x012439DC, retail 0x48 bytes
public:
    std::vector<C2DVector > Offset;                    // +0x3C (Ego_r +0x40)
};
namespace std {
template <class _Tp>
void __destroy_aux(_Tp* __first, _Tp* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->~_Tp();
}
template void __destroy_aux<CCardPositionsDef >(CCardPositionsDef*, CCardPositionsDef*, const __false_type&);
template vector<CCardPositionsDef >::iterator vector<CCardPositionsDef >::erase(iterator, iterator);
}