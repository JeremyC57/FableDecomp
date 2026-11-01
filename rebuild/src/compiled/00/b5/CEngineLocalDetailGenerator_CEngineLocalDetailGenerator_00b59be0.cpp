// CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator() -- local-detail generator destructor: delete every engine map's local-detail cache map, free the object-collection-type array, then ~CEngineComponent
// Leaf: ??1CEngineLocalDetailGenerator@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B59BE0, 129 bytes, thiscall, tail jump to CEngineComponent::~CEngineComponent 0x00B59730.
// Identity (proven): the first store is vtable 0x012A2588 (rtti-vtables.tsv: CEngineLocalDetailGenerator, RTTI
//   .?AVCEngineLocalDetailGenerator@@ : CEngineComponent); the sole caller 0x00B5A093 is the scalar deleting destructor
//   0x00B5A090, slot 0 of that vtable. Ego_r.pdb / FableWin.pdb ??1CEngineLocalDetailGenerator@@UAE@XZ. The catalog label
//   "CEngineLocalDetailGenerator::AddLocalDetailDynamic" is wrong (AddLocalDetailDynamic is a private no-argument member, not
//   a vtable-storing function that ends in ~CEngineComponent).
// Callees: ~CLocalDetailCacheMap 0x00BDED60 (inline `delete` of each map's LocalDetailCacheMap; the catalog's
//   "~CPatchTesselationEdgeStrip" label there is propagated), operator delete 0x00BFE9BC, free 0x00BFEA14 (STLport allocator
//   deallocate of ObjectCollectionTypes), ??1CEngineComponent@@UAE@XZ 0x00B59730 (tail jump).
// Globals: the engine world map pointer 0x01436E8C, spelt ?g_pEngineWorldMap@@3PAVCEngineWorldMap@@A as the landed
//   CEngineWorldMap callers spell it (no PDB data-symbol dump). CEngineWorldMap::MapList +0x20 (retail CArray<CEngineMap*>,
//   as the landed DeleteMap 0x00B42B10), CEngineMap::LocalDetailCacheMap +0x04 (Ego_r).
// Layout: retail CEngineLocalDetailGenerator : CEngineComponent (0x0C); ObjectCollectionTypes +0x0C (Ego_r name; retail
//   12-byte STLport vector) is the only container the retail dtor frees, so the later Ego_r members are not modelled.
//   engine/CEngineComponent.h / CEngineWorldMap.h are flattened records; local models.
// Shape: the PDB accessors CEngineWorldMap::GetMapCount() const (long: signed `jl`) and GetMap(long) const; each map access
//   is re-evaluated after the delete (retail reloads the global), and the zero store follows the null-tested delete.
// Layout: engine/CLocalDetailCacheMap.h and engine/CEngineWorldMap.h are flattened records (no member functions; the world map
//   header carries Ego_r offsets past MapList), so both are local models (CEngineWorldMap with the PDB accessors).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

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
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    const _Tp& operator[](size_type __n) const { return *(this->_M_start + __n); }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: the 12-byte STLport vector
};

class CLocalDetailCacheMap { public:
    ~CLocalDetailCacheMap();                                // 0x00BDED60
};
class CEngineLandscapeMap;
class CEngineMap { public:
    CEngineLandscapeMap*  LandscapeMap;                     // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;              // +0x04
};
class CEngineWorldMap { public:
    unsigned char     _pad_0x00[0x20];
    CArray<CEngineMap*> MapList;                            // +0x20
    long GetMapCount() const { return MapList.size(); }
    CEngineMap* GetMap(long index) const { return MapList[index]; }
};
extern CEngineWorldMap* g_pEngineWorldMap;                  // 0x01436E8C

class CEngineComponent {                                    // engine/CEngineComponent.h offsets, 0x0C bytes
public:
    virtual ~CEngineComponent();                            // 0x00B59730
    void**        GlobalPointerAddress;                     // +0x04
    bool          RenderEnabled;                            // +0x08
    unsigned char KeepAlive;                                // +0x09
};

class CLocalDetailObjectCollectionType;
class CEngineLocalDetailGenerator : public CEngineComponent {   // vtable 0x012A2588
public:
    virtual ~CEngineLocalDetailGenerator();
    CArray<CLocalDetailObjectCollectionType*> ObjectCollectionTypes;   // +0x0C
};

CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator()
{
    for (long i = 0; i < g_pEngineWorldMap->GetMapCount(); ++i) {
        if (g_pEngineWorldMap->GetMap(i)) {
            delete g_pEngineWorldMap->GetMap(i)->LocalDetailCacheMap;
            g_pEngineWorldMap->GetMap(i)->LocalDetailCacheMap = 0;
        }
    }
}