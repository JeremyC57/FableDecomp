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
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10 (compiler-generated in retail, emitted out of line)
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC, retail 0x39 (packed)
public:
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0 (compiler-generated in retail, emitted out of line)
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
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