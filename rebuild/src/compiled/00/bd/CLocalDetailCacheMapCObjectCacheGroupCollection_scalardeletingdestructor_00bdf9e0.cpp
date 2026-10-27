// CLocalDetailCacheMap::CObjectCacheGroupCollection::`scalar deleting destructor'(unsigned int flags) -- the virtual destructor, then (bit 0) the class operator delete: the block goes back to the landscape memory manager
// Leaf: ??_GCObjectCacheGroupCollection@CLocalDetailCacheMap@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF9E0, 38 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): slot 0 of vtable 0x012A7DDC (rtti-vtables: CLocalDetailCacheMap::CObjectCacheGroupCollection; RTTI bases CMovableResource); the destructor it calls,
//   0x00BDDE80, is the landed ~CObjectCacheGroupCollection; FableWin.pdb ??1CObjectCacheGroupCollection@CLocalDetailCacheMap@@UAE@XZ (virtual) and the class's own
//   ??3CObjectCacheGroupCollection@CLocalDetailCacheMap@@SIXPAX@Z (static __fastcall operator delete).
// Callees: CLocalDetailCacheMap::CObjectCacheGroupCollection::~CObjectCacheGroupCollection() 0x00BDDE80, CEngineLandscapeMemoryManager::FreeVirtual(void*) 0x00BD9B90 on the manager at +0x6B0
//   of the engine global 0x01436EA8.
// Shape (family K057, representative 0x00BDF9B0): the landed CLandscapeBackgroundSubNodeSplitBranch ??_G 0x00BF8D10 with the
//   destructor out of line; the class operator delete inlines to `mov eax,[global] ; mov ecx,[eax+6B0h] ; push esi ; call`.
//   The engine global keeps the landed name and stand-in type g_pEngine_01436ea8 / SEngine_01436ea8 (descriptive).
class CEngineLandscapeMemoryManager {
public:
    void FreeVirtual(void* block);                       // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6B0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;              // 0x01436EA8
class CMovableResource {                                 // vtable 0x0129B860
public:
    CMovableResource();
    virtual ~CMovableResource();
};
class CLocalDetailCacheMap {
public:
class CObjectCacheGroupCollection : public CMovableResource {
public:
    CObjectCacheGroupCollection() { }                                          // model device: emits the vtable (and ??_G) in this file
    virtual ~CObjectCacheGroupCollection();                                    // 0x00BDDE80
    static void __fastcall operator delete(void* block) { g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(block); }
};
};
inline void* operator new(unsigned int, void* where) { return where; }
CLocalDetailCacheMap::CObjectCacheGroupCollection* Construct_00bdf9e0(void* where) { return new (where) CLocalDetailCacheMap::CObjectCacheGroupCollection; }   // not part of the retail body