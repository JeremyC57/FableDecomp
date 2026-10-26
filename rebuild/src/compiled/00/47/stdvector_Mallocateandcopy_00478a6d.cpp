// std::vector::_M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last) -- (vector<CAbilityTrackDef >) allocate __n elements of 0x50 bytes and copy-construct [__first, __last) into them; return the block
// Leaf: ??$_M_allocate_and_copy@PBVCAbilityTrackDef@@@?$vector@VCAbilityTrackDef@@V?$allocator@VCAbilityTrackDef@@@std@@@std@@IAEPAVCAbilityTrackDef@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478A6D, 74 bytes (size-optimised TU, odd entry), thiscall (this unused), `ret 0Ch`.
// Identity (proven): std::vector<CAbilityTrackDef >::_M_allocate_and_copy<const CAbilityTrackDef*>; evidence: its only caller is std::vector<CAbilityTrackDef >::operator= 0x0047857D (its reallocation branch), whose callers are CHeroStatsDef::StatLevels at this+0xFC (vector operator= call 0x004782D9 in CHeroStatsDef::operator= 0x00478132, whose Copy thunk 0x0047812D is slot 19 of vtable 0x012339DC, RTTI .?AVCHeroStatsDef@@); the element copy constructor 0x004786CF stores vtable 0x0123649C (RTTI .?AVCAbilityTrackDef@@, base CParentDefClassBase), and the stride / malloc scale 0x50 equal that vtable's slot-20 size
//   STLport spelling (the Dinkumware PDB name sets have no _M_ helpers). Family J066 (five def-element instances, each its own type).
// Label: the manifest's "CAbilityTrackDef_Allocate" is a behavioural label for this element; the leaf is the STLport member template.
// Callees: malloc 0x00BFEA0E (allocator::allocate); CAbilityTrackDef::CAbilityTrackDef(const CAbilityTrackDef&) 0x004786CF (out of line, thiscall: its body stores
//   vtable 0x0123649C, RTTI .?AVCAbilityTrackDef@@, after the CParentDefClassBase copy constructor 0x0046EE98).
// Shape: agent951's STLport 4.6 model (0x0045C71C) with a class element: placement new null-tests the slot before the out-of-line
//   copy constructor; the __uninitialized_copy loop keeps one (result - first) offset register.
// Element: CAbilityTrackDef, 0x50 bytes in retail (vtable 0x0123649C slot 20 returns 0x50; Ego_r.pdb 0x58 with the 0x39-byte packed
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
enum EHeroExperienceStatCategory { };                  // Ego_r.pdb (enumerators not needed)
class CAbilityTrackLevelDef;                       // Ego_r.pdb (only its vector's pointers are held here)
class CAbilityTrackDef : public CParentDefClassBase {   // vtable 0x0123649C, retail 0x50 bytes
public:
    CAbilityTrackDef(const CAbilityTrackDef& other);   // 0x004786CF
    EHeroExperienceStatCategory ExperienceStat;        // +0x3C (Ego_r +0x40)
    std::vector<CAbilityTrackLevelDef > Abilities;     // +0x40 (Ego_r +0x44)
    long MaxedOutDescription;                          // +0x4C (Ego_r +0x54)
};
namespace std {
typedef CAbilityTrackDef _Elem;
template _Elem* vector<_Elem>::_M_allocate_and_copy<const _Elem*>(unsigned int, const _Elem*, const _Elem*);
}