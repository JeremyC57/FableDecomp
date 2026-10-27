// std::pair<long, CCompositeSurfaceInfo >::operator=(const pair& other) -- the compiler-generated memberwise assignment of a vector-map entry: copy the key, then the counted pointer's operator= (release the old count block, freeing it and deleting the data at zero; adopt and add-ref the new one)
// Leaf: ??4?$pair@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BED640, 78 bytes, thiscall, `ret 4`, returns this.
// Identity: called by the landed std::__adjust_heap 0x00BEDD70 (which declares this callee as the entry's implicit operator=; callers std::__make_heap 0x00BEE140 / __pop_heap 0x00BEE070 over pair<long, CCompositeSurfaceInfo>, the CVectorMap<long, CCompositeSurfaceInfo> sort) and std::__linear_insert 0x00BEE8F0; the second member's vptr (+4) is not assigned, Cache (+8) is.
// Shape (family K012, representative 0x006B8E20): the implicit pair operator= emitted out of line (its address is taken
//   below, as retail's sort helpers call it); CCountedPointer::operator= reads the source data and block into locals
//   first, `if (Info != info) { DeleteData(); Data = data; Info = info; if (info) ++info->RefCount; }`, the release
//   re-reading Info after the decrement (`dec [eax] ; mov eax,[esi+N] ; cmp [eax],0`) and calling the block's
//   __fastcall deleter on its data.
class CCPPointerInfo {                                   // Ego_r.pdb, 12 bytes
public:
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void* Data;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCPPointerInfo* GetInfo() const { return Info; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(GetInfo());
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0).
class CBaseClass {
public:
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): the implicit assignment copies Cache, never the vptr.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;      // +0x04
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<long, CCompositeSurfaceInfo > TEntry;
// Taking the implicit operator='s address makes VC7.1 emit it out of line.
TEntry& (TEntry::*g_pAssign_00bed640)(const TEntry&) = &TEntry::operator=;