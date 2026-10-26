// std::vector::erase(iterator __first, iterator __last) -- (vector<CShopItemDef >) STLport range erase: assign the tail [last, finish) down over first, destroy the leftover elements, pull finish back, return first
// Flags: /Gr
// Leaf: ?erase@?$vector@VCShopItemDef@@V?$allocator@VCShopItemDef@@@std@@@std@@QAEPAVCShopItemDef@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ED0C6, 60 bytes (size-optimised TU with a frame pointer), thiscall, `ret 8`.
// Identity (proven): std::vector<CShopItemDef >::erase(iterator, iterator); evidence: its only caller is std::vector<CShopItemDef >::resize 0x004ED07D (n < size(): erase(begin() + n, end()), the size by `idiv 0x48`) through resize(n) 0x004ED04D, called from CPersistContext::TransferVectorOfSubComponents<CShopItemDef> 0x004ECF6E of CShopDef::Transfer 0x004ECB4D (slot 18 of vtable 0x01241F9C, RTTI .?AVCShopDef@@) on CShopDef::DefaultStock at this+0x40 (Ego_r.pdb +0x40, vector<CShopItemDef>; site 0x004ECB9D); CShopDef::StockCategoryDefaults at this+0x4C (Ego_r.pdb +0x50, vector<CShopItemDef>; site 0x004ECBA8); the element is CShopItemDef (vtable 0x0123B644)
// Label: the manifest's "Vector_InsertRange_Pair_DummyObject" is a behavioural label; this row is vector<CShopItemDef >::erase(first, last) (family J068: five 60-byte bodies, each its own element).
// Callees: __copy_ptrs(first, last, result, const __false_type&) 0x004ED102 (out of line, __fastcall in this /Gr TU, `ret 8`: the
//   implicit CShopItemDef::operator= inlined every 0x48 bytes after CDefClassBase::operator= 0x00431F10), __destroy_aux(first, last, const __false_type&)
//   0x004DA5BD (__fastcall, `ret 4`: the virtual destructor, slot 0 with flag 0, every 0x48 bytes; the bindings' ??$__destroy_aux@VCShopItemDef@@@std@@ spelling).
// Shape: STLport 4.6 vector.h `erase` verbatim. The _TrivialAss() tag is a constructor-less empty __false_type, value-initialised
//   (`stosb`) in its own frame byte for the unseen __copy_ptrs; the _Trivial_destructor() tag of __destroy_aux, whose body this
//   TU compiles first, gets no store and is homed in the top byte of the `first` slot (`lea eax,[ebp+0Bh]`).
// Element: CShopItemDef, 0x48 bytes in retail (vtable 0x0123B644 slot 20 returns 0x48, base CSubDefClassBase), Ego_r.pdb members
//   rebased to retail (retail offset = Ego_r offset).
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
_OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&);   // 0x004ED102
template <class _Tp>
void __destroy_aux(_Tp* __first, _Tp* __last, const __false_type&);   // 0x004DA5BD (defined below, ahead of erase)
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
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
};
#pragma pack(pop)
class CShopItem {                                         // Ego_r.pdb 28 bytes
public:
    float                                    Quantity;
    float                                    PriceMultiplier;
    float                                    MaxStock;
    long                                     RestockPeriod;
    float                                    RestockPeriodRandomness;
    float                                    SalesPerDay;
    float                                    SalesPerDayRandomness;
};
class CShopItemDef : public CSubDefClassBase {   // vtable 0x0123B644, retail 0x48 bytes
public:
    long Type;                                         // +0x28
    CShopItem Item;                                    // +0x2C
};
namespace std {
template <class _Tp>
void __destroy_aux(_Tp* __first, _Tp* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->~_Tp();
}
template void __destroy_aux<CShopItemDef >(CShopItemDef*, CShopItemDef*, const __false_type&);
template vector<CShopItemDef >::iterator vector<CShopItemDef >::erase(iterator, iterator);
}