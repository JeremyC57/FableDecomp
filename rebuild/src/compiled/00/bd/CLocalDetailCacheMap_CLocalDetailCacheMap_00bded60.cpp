// CLocalDetailCacheMap::~CLocalDetailCacheMap() -- free the collection-type palette's entry array and the area update list, then destroy the QuadTree root element
// Leaf: ??1CLocalDetailCacheMap@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDED60, 44 bytes, thiscall, tail `jmp ~CQuadTreeElement` on this+0xC.
// Identity (proven): the three callers are the landed CEngineLocalDetailGenerator::DeleteMap 0x00B59840 and
//   ~CEngineLocalDetailGenerator 0x00B59BE0 (both `delete map->LocalDetailCacheMap`, binding ??1CLocalDetailCacheMap@@QAE@XZ)
//   and the deleting destructor 0x00B59D10; non-polymorphic (no vtable store, no base call). Ego_r.pdb (0x00A92180, after the
//   ctor 0x00A920F0) and FableWin.pdb (0x02E3AE50) spell ??1CLocalDetailCacheMap@@QAE@XZ. The catalog label
//   "~CPatchTesselationEdgeStrip" is propagated.
// Callees: free 0x00BFEA14 (STLport allocator deallocate, twice), CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement
//   0x00BDDFB0 (stores the nested class's vtable 0x012A7DF0, RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; Ego_r 0x00A8F770 /
//   FableWin 0x02E384B0 ??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ).
// Shape: an empty user destructor; members in reverse order: CollectionTypePalette (+0x64; its Entries CArray, the STLport
//   vector's _Vector_base deallocate = `if (start) free(start)`; trivially destructible pointer elements), AreaUpdateList
//   (+0x58, vector<C2DBoxI>), then the QuadTree element's out-of-line virtual dtor called directly as the tail jump.
// Layout: Ego_r.pdb CLocalDetailCacheMap with retail container sizes (engine/CLocalDetailCacheMap.h, 0x70): LockedFromMemoryRelocation
//   +0, FileBlockManager +4, Map +8, QuadTree (CQuadTreeElement, 0x4C) +0xC, AreaUpdateList +0x58, CollectionTypePalette +0x64.
//   The engine header is a flattened record with opaque members (no element types, no nested classes), so the class is a local
//   model with the header's offsets (header debt); STLport vector as in the landed caller 0x00B59BE0.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: the 12-byte STLport vector
};

class CEngineMap;
class CEngineStaticMapFileBlockManager;
class CLocalDetailObjectCollectionType;

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
};

class CLocalDetailCacheMap {                                // 0x70 bytes (retail)
public:
    class CQuadTreeElement {                                // vtable 0x012A7DF0, 0x4C bytes
    public:
        CQuadTreeElement(CQuadTreeElement* parent, long x, long y, long size, long level);
        virtual ~CQuadTreeElement();                        // 0x00BDDFB0
        unsigned char _body[0x4c - 0x4];
    };
    class CObjectTypeCollectionPalette {
    public:
        CArray<const CLocalDetailObjectCollectionType*> Entries;   // +0x00
    };
    CLocalDetailCacheMap(CEngineMap* map);
    ~CLocalDetailCacheMap();
    bool LockedFromMemoryRelocation;                        // +0x00
    CEngineStaticMapFileBlockManager* FileBlockManager;     // +0x04
    CEngineMap* Map;                                        // +0x08
    CQuadTreeElement QuadTree;                              // +0x0C
    std::vector<C2DBoxI> AreaUpdateList;                    // +0x58
    CObjectTypeCollectionPalette CollectionTypePalette;     // +0x64
};

CLocalDetailCacheMap::~CLocalDetailCacheMap()
{
}