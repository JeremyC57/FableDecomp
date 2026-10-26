// CEngineSurfaceCompositionManager::AddSurface(CCompositeSurfaceInfo& info) -- gives a composite-surface request its cache entry on first use: allocates a CCompositeSurfaceCache owned by the manager, keeps a counted reference in CachedSurfaces and registers it as a bank resource
// Leaf: ?AddSurface@CEngineSurfaceCompositionManager_Methods@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2C460, length 121 bytes, thiscall `ret 4`.
// Signature from Ego_r.pdb: ?AddSurface@CEngineSurfaceCompositionManager@@QAEXAAVCCompositeSurfaceInfo@@@Z.
//   Caller: CEngineInternalPrimitiveMorphInfo::InitialiseCompositeSurfaces @ 0x00BECBA0 (call 0x00BECC56).
// Callees: operator new @ 0x00BFEA1A (0x68 bytes: retail CCompositeSurfaceCache, PDB 0x70), CCompositeSurfaceCache::
//   CCompositeSurfaceCache(CEngineSurfaceCompositionManager&) @ 0x00B2C400, CCountedPointer<CCompositeSurfaceCache>::
//   Reset(CCompositeSurfaceCache*) @ 0x00B2E290 (catalog "Reset CCountedPointer<CIPhysicsFacesSegmentBase const>" is a
//   propagated label), malloc @ 0x00BFEA0E (the 0x10-byte STLport list node), CResourceBank::AddResource @ 0x009FC380
//   (the manager's CResourceBank base at offset 0).
// Layout (Ego_r.pdb): CCompositeSurfaceInfo::Cache CCountedPointer<CCompositeSurfaceCache> at +4 (Data +0, Info +4 ->
//   CCPPointerInfo whose RefCount is +0); CEngineSurfaceCompositionManager::CachedSurfaces at +0x88 (retail STLport
//   list = one sentinel pointer; the shared header keeps the PDB's 12 opaque bytes).
// Shape: `if (!info.Cache) { info.Cache.Reset(new CCompositeSurfaceCache(*this)); CachedSurfaces.push_back(info.Cache);
//   AddResource(info.Cache.Data); }` -- push_back inline (end() read before the node malloc, placement-new null test,
//   CCountedPointer copy constructor with its `++Info->RefCount`, then the four link stores).  The list iterator needs
//   its declared copy constructor (GOTCHAS retry 10i): without it VC7.1 sinks `push edi` past the new-expression and
//   gives the early exit its own epilogue (DIFFER 121v121).
// Label: catalog "CResourceBank_InitializeCompositeSurface" is behavioural.
#include "engine/CEngineSurfaceCompositionManager.h"

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A (import thunk)
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CCPPointerInfo { public:
    unsigned long RefCount;                                 // +0x00
    void (__cdecl* DeleteFunc)(void*);                      // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    operator bool() const { return Data != 0; }
    void Reset(T* p);                                       // 0x00B2E290 (out of line)
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                               // +0
    _List_node_base* _M_prev;                               // +4
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                              // +8
};
template <class T> struct _List_iterator {
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
    _List_node_base* _M_node;
};

template <class T> class list {                             // STLport list: one pointer to the sentinel node
public:
    typedef _List_iterator<T> iterator;
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* tmp = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&tmp->_M_data) T(x);
        return tmp;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
    _List_node_base* _M_node;                               // +0
};

} // namespace std

class CResource;
class CResourceBank {
public:
    void AddResource(CResource* resource);                  // 0x009FC380
};

class CCompositeSurfaceCache;
class CCompositeSurfaceCache {                              // retail 0x68 bytes; its CResource base is at offset 0
public:
    unsigned char _pad_0x00[0x68];
    CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner);   // 0x00B2C400
    CResource* AsResource() { return (CResource*)this; }
};

class CCompositeSurfaceInfo {
public:
    unsigned char                           _pad_0x00[0x04];
    CCountedPointer<CCompositeSurfaceCache> Cache;          // +0x04
};

struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    typedef std::list<CCountedPointer<CCompositeSurfaceCache> > CSurfaceList;
    CSurfaceList& Surfaces() { return *(CSurfaceList*)CachedSurfaces; }     // +0x88
    CResourceBank& Bank() { return *(CResourceBank*)this; }                // the CResourceBank base at offset 0
    void AddSurface(CCompositeSurfaceInfo& info);
};

void CEngineSurfaceCompositionManager_Methods::AddSurface(CCompositeSurfaceInfo& info)
{
    if (!info.Cache) {
        info.Cache.Reset(new CCompositeSurfaceCache(*this));
        Surfaces().push_back(info.Cache);
        Bank().AddResource(info.Cache.Data->AsResource());
    }
}