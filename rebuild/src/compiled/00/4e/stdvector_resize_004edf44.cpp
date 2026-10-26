// std::vector::resize(size_type __new_size, const _Tp& __x) -- (vector<CMultiStaticMeshEntryDef >) STLport resize: erase the surplus [begin() + n, end()) when shrinking, else insert n - size() copies of x at the end
// Leaf: ?resize@?$vector@VCMultiStaticMeshEntryDef@@V?$allocator@VCMultiStaticMeshEntryDef@@@std@@@std@@QAEXIABVCMultiStaticMeshEntryDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDF44, 72 bytes (size-optimised TU), thiscall, `ret 8`.
// Identity (proven): std::vector<CMultiStaticMeshEntryDef >::resize(size_type, const CMultiStaticMeshEntryDef&); evidence: its only caller is resize(n) 0x004EDF0A, which constructs T() through 0x0044C0C0, stores vtable 0x012438A4 (RTTI .?AVCMultiStaticMeshEntryDef@@) and destroys it afterwards; that resize(n) is called by CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef> 0x004EDE2B of CMultiStaticMeshDef::Transfer 0x004EDE1B on this+0x28 (Ego_r.pdb CMultiStaticMeshDef::Meshes, vector<CMultiStaticMeshEntryDef>)
// Label: the manifest's "push_back" is a behavioural / propagated label; this row is vector<CMultiStaticMeshEntryDef >::resize(n, x) (family J069: five 72-byte bodies, each its own element).
// Callees: erase(iterator, iterator) 0x004EDF8C, _M_fill_insert(iterator, size_type, const CMultiStaticMeshEntryDef&) 0x004EDFDC (both out of line, thiscall).
// Shape: STLport 4.6 vector.h `resize` verbatim with insert(pos, n, x) inlined to _M_fill_insert; size() is the exact
//   pointer difference (`idiv 0x38`, the divisor loaded by push/pop under /Os) and is recomputed on the insert path.
// Element: CMultiStaticMeshEntryDef, 0x38 bytes (vtable 0x012438A4 slot 20; base CSubDefClassBase, retail offsets = Ego_r.pdb)
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
    iterator erase(iterator __first, iterator __last);                          // 0x004EDF8C
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x004EDFDC
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
enum ELightingChannel { };                             // Ego_r.pdb (enumerators not needed)
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
class CMultiStaticMeshEntryDef : public CSubDefClassBase {   // vtable 0x012438A4, retail 0x38 bytes
public:
    long Mesh;                                         // +0x28
    bool OverrideLightingChannel;                      // +0x2C
    bool OverrideRenderFadeDistance;                   // +0x2D
    float RenderFadeDistance;                          // +0x30
    ELightingChannel LightingChannel;                  // +0x34
};
namespace std {
template void vector<CMultiStaticMeshEntryDef >::resize(unsigned int, const CMultiStaticMeshEntryDef&);
}