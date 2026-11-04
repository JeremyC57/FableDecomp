// CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator() -- local-detail generator destructor: delete every engine map's local-detail cache map, free the object-collection-type array, then ~CEngineComponent
// Leaf: ??1CEngineLocalDetailGenerator@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
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

#include "engine/CArray.h"


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