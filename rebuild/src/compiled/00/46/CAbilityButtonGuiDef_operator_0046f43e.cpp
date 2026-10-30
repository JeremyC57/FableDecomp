// CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then ControllerType, ControllerButton, MouseButton, ButtonGraphic, Position, Offset memberwise
// Leaf: ??4CAbilityButtonGuiDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046F43E, 69 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`, returns this.
// Identity (proven): CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef&); evidence: its only def caller is CAbilityButtonGuiDef::Copy 0x0046F49F (a `jmp` to it), slot 19 of vtable 0x0123610C (RTTI .?AVCAbilityButtonGuiDef@@), so `*this = *static_cast<const CAbilityButtonGuiDef*>(other)`;
//   RTTI bases CParentDefClassBase <- CDefClassBase; the copied offsets and widths are Ego_r.pdb CAbilityButtonGuiDef's members (retail = Ego_r - 4 after the shorter CParentDefClassBase); FableWin.pdb name set holds ??4CAbilityButtonGuiDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "operator=" is propagated (family J059: five byte-similar def copy assignments); the Copy thunk's vtable names CAbilityButtonGuiDef.
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0 (landed under that name).
// Shape: the implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax. The Copy thunk is defined
//   here only to make the compiler emit the implicit operator= (retail emits it beside Copy, a `jmp` to it).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };   // STLport, 12 bytes
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes (its _Rb_tree: header, count, compare)
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
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
class C2DVector {                                        // Ego_r.pdb 8 bytes
public:
    float X;
    float Y;
};
enum EControllerType { };                              // Ego_r.pdb (enumerators not needed)
enum EMouseButtonControl { };                          // Ego_r.pdb (enumerators not needed)
enum EXboxControllerButton { };                        // Ego_r.pdb (enumerators not needed)
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
class CAbilityButtonGuiDef : public CParentDefClassBase {                     // vtable 0x0123610C
public:
    virtual void Copy(const CDefClassBase* other);       // vtable 0x0123610C slot 19, 0x0046F49F
    EControllerType ControllerType;                    // +0x3C (Ego_r +0x40)
    EXboxControllerButton ControllerButton;            // +0x40 (Ego_r +0x44)
    EMouseButtonControl MouseButton;                   // +0x44 (Ego_r +0x48)
    long ButtonGraphic;                                // +0x48 (Ego_r +0x4C)
    C2DVector Position;                                // +0x4C (Ego_r +0x50)
    C2DVector Offset;                                  // +0x54 (Ego_r +0x58)
};
void CAbilityButtonGuiDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CAbilityButtonGuiDef*>(other);
}