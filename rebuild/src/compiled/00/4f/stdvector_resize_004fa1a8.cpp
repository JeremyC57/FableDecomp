// std::vector::resize(size_type __new_size, const _Tp& __x) -- (vector<CCardDef >) STLport resize: erase the surplus [begin() + n, end()) when shrinking, else insert n - size() copies of x at the end
// Leaf: ?resize@?$vector@VCCardDef@@V?$allocator@VCCardDef@@@std@@@std@@QAEXIABVCCardDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FA1A8, 72 bytes (size-optimised TU), thiscall, `ret 8`.
// Identity (proven): std::vector<CCardDef >::resize(size_type, const CCardDef&); evidence: its only caller is CPersistContext::TransferVectorOfSubComponents<CCardDef> 0x004FA0A9 (site 0x004FA174) of CTavernGameCardBaseDef::Transfer 0x004F9A90 on this+0x78 (Ego_r.pdb CardPack +0x7C, vector<CCardDef>); its erase is the J068 row 0x004FA1F0, vector<CCardDef>::erase
// Label: the manifest's "push_back" is a behavioural / propagated label; this row is vector<CCardDef >::resize(n, x) (family J069: five 72-byte bodies, each its own element).
// Callees: erase(iterator, iterator) 0x004FA1F0 (the J068 row vector<CCardDef>::erase), _M_fill_insert(iterator, size_type, const CCardDef&) 0x004FA277 (both out of line, thiscall).
// Shape: STLport 4.6 vector.h `resize` verbatim with insert(pos, n, x) inlined to _M_fill_insert; size() is the exact
//   pointer difference (`idiv 0x44`, the divisor loaded by push/pop under /Os) and is recomputed on the insert path.
// Element: CCardDef, 0x44 bytes (vtable 0x01243A44 slot 20; base CParentDefClassBase, retail = Ego_r.pdb - 4)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x004FA1F0
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x004FA277
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
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
class CCardDef : public CParentDefClassBase {   // vtable 0x01243A44, retail 0x44 bytes
public:
    long CardName;                                     // +0x3C (Ego_r +0x40)
    long CardVal;                                      // +0x40 (Ego_r +0x44)
};
namespace std {
template void vector<CCardDef >::resize(unsigned int, const CCardDef&);
}