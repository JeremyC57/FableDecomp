// std::__push_heap(pair<long,CCompositeSurfaceInfo>* first, int holeIndex, int topIndex, pair<long,CCompositeSurfaceInfo> val, CKeyPairCompareLess<long,CCompositeSurfaceInfo> comp) -- STLport heap sift-up of the composed-surface vector map: move parents with a smaller key down, then store val in the hole
// Leaf: ??$__push_heap@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BED880, 280 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; topIndex, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): long key +0 (SIGNED compare, `jge`), CCompositeSurfaceInfo +4 (Ego_r.pdb, 12
//   bytes: the CBaseClass vptr +0, CCountedPointer<CCompositeSurfaceCache> Cache +4 = {Data, Info}; RTTI vtable
//   0x012A8154). The implicit assignment moves the key and Cache only (an implicit operator= never copies a vptr).
// Label correction: the catalog's `pair<unsigned long,CCountedPointer<CFaction>>` is propagated; the PDB name set
//   holds `_Push_heap<pair<long,CCompositeSurfaceInfo>*,...>` for this map (CVectorMap<long,CCompositeSurfaceInfo>,
//   CEnginePrimitiveManagerMeshBase), and the val destructor ends in ~CBaseClass 0x0099A2E0. Caller 0x00BEDE57 =
//   __adjust_heap 0x00BEDD70 (landed).
// Callees: operator delete @ 0x00BFE9BC (the inline CCountedPointer release: drop the count, call the block's
//   __fastcall destroy function on the object, free the block), CBaseClass::~CBaseClass @ 0x0099A2E0 (val, after
//   the implicit ~CCompositeSurfaceInfo released and nulled Cache -- a compiler-generated destructor stores no vtable).
// Shape: STLport 4.6 `__push_heap` (`(hole - 1) / 2` as `cdq ; sub ; sar`), every element move inline.
extern void __cdecl operator delete(void* block);                                    // 0x00BFE9BC

class CCPPointerInfo { public:                                 // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                             // +0x0
    void (__fastcall* DeleteFunc)(void* data);          // +0x4
    void* Data;                                         // +0x8
};

template <class T> class CCountedPointer {
public:
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info)
    {
        if (other.Info)
            ++other.Info->RefCount;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
};

class CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};

class CCompositeSurfaceCache;
class CCompositeSurfaceInfo : public CBaseClass {       // Ego_r.pdb, 12 bytes; vtable 0x012A8154
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;      // +0x4
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<long, CCompositeSurfaceInfo> CSurfacePair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template void __fastcall __push_heap(CSurfacePair*, int, int, CSurfacePair, CKeyPairCompareLess<long, CCompositeSurfaceInfo>);
}