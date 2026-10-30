// std::vector::_M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last) -- (vector<CQuickAccessItemButtonGuiDef >) allocate __n elements of 0x50 bytes and copy-construct [__first, __last) into them; return the block
// Leaf: ??$_M_allocate_and_copy@PBVCQuickAccessItemButtonGuiDef@@@?$vector@VCQuickAccessItemButtonGuiDef@@V?$allocator@VCQuickAccessItemButtonGuiDef@@@std@@@std@@IAEPAVCQuickAccessItemButtonGuiDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046F020, 74 bytes (size-optimised TU, entry 16-aligned by chance), thiscall (this unused), `ret 0Ch`.
// Identity (proven): std::vector<CQuickAccessItemButtonGuiDef >::_M_allocate_and_copy<const CQuickAccessItemButtonGuiDef*>; evidence: its only caller is std::vector<CQuickAccessItemButtonGuiDef >::operator= 0x0046ED15 (its reallocation branch), whose callers are CPlayerGuiDef::QuickAccessItemButtons at this+0x728 (vector operator= call 0x0046E1AF in CPlayerGuiDef::operator= 0x0046CE2D, whose Copy thunk 0x0046CE28 is slot 19 of vtable 0x012352DC, RTTI .?AVCPlayerGuiDef@@); CPlayerGuiDef::ContextSensitiveItemButtons at this+0x734 (vector operator= call 0x0046E1C1 in CPlayerGuiDef::operator= 0x0046CE2D, whose Copy thunk 0x0046CE28 is slot 19 of vtable 0x012352DC, RTTI .?AVCPlayerGuiDef@@); the element copy constructor 0x0046EE2D stores vtable 0x012360A4 (RTTI .?AVCQuickAccessItemButtonGuiDef@@, base CParentDefClassBase), and the stride / malloc scale 0x50 equal that vtable's slot-20 size
//   STLport spelling (the Dinkumware PDB name sets have no _M_ helpers). Family J066 (five def-element instances, each its own type).
// Label: the manifest's "CQuickAccessItemButtonGuiDef_CopyConstructArray" is a behavioural label for this element; the leaf is the STLport member template.
// Callees: malloc 0x00BFEA0E (allocator::allocate); CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&) 0x0046EE2D (out of line, thiscall: its body stores
//   vtable 0x012360A4, RTTI .?AVCQuickAccessItemButtonGuiDef@@, after the CParentDefClassBase copy constructor 0x0046EE98).
// Shape: agent951's STLport 4.6 model (0x0045C71C) with a class element: placement new null-tests the slot before the out-of-line
//   copy constructor; the __uninitialized_copy loop keeps one (result - first) offset register.
// Element: CQuickAccessItemButtonGuiDef, 0x50 bytes in retail (vtable 0x012360A4 slot 20 returns 0x50; Ego_r.pdb 0x54 with the 0x39-byte packed
//   CParentDefClassBase and STLport containers at 12 bytes), Ego_r.pdb members rebased to retail.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __false_type _IsPODType;                       // __type_traits of a class element
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (natural packing)
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CResourceList;
class CDefinitionManager;
#pragma pack(push, 1)
// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,
//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0);
//   FABLE_PDCB_TYPED_MEMBERS: InstantiationName / SubDefInfoMap are typed with this row's CDefString / CVectorMap.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class C2DVector {                                        // Ego_r.pdb 8 bytes
public:
    float X;
    float Y;
};
class CQuickAccessItemButtonGuiDef : public CParentDefClassBase {   // vtable 0x012360A4, retail 0x50 bytes
public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CQuickAccessItemButtonGuiDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CQuickAccessItemButtonGuiDef@@UAEKXZ
    CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& other);   // 0x0046EE2D
    long ButtonGraphic;                                // +0x3C (Ego_r +0x40)
    C2DVector Position;                                // +0x40 (Ego_r +0x44)
    C2DVector Offset;                                  // +0x48 (Ego_r +0x4C)
};
namespace std {
typedef CQuickAccessItemButtonGuiDef _Elem;
template _Elem* vector<_Elem>::_M_allocate_and_copy<const _Elem*>(unsigned int, const _Elem*, const _Elem*);
}