// NCreatureMode::CModeDragonFlyingBase::`scalar deleting destructor'(unsigned int) -- frees the block at +0x1c, runs the inlined CModeBase destructor (its vtable, the name string, the anim-filter member's vtable) and frees the object for flag bit 0
// Leaf: ??_GCModeDragonFlyingBase@NCreatureMode@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743780, 62 bytes (`ret 4`). NCreatureMode::CModeDragonFlyingBase vtable 0x01268658 slot 0.
// Callees: free @ 0x00BFEA14 (the +0x1c block), CCharString::~CCharString @ 0x0099EAE0 (+0x0c), operator delete
// @ 0x00BFE9BC. Vtables stored: 0x012673CC (CTCCreatureModeManager::CModeBase) at +0, 0x0122FFCC (CAnimFilterBase,
// the member at +8) -- the landed CModeBase ??_G 0x00739EE0 has the same tail.
// Label: "CParticleMorphs_CEntry::~CParticleMorphs_CEntry" is propagated; the vtable slot names the class.
// Shape: no store of the derived vtable before the free: the derived class is `__declspec(novtable)` with a
// NON-virtual destructor over a base whose inline destructor stores its vtable (GOTCHAS wave 24m); a `delete`
// helper makes VC7.1 emit the class's scalar deleting destructor (`??_G...@@QAEPAXI@Z`).
extern "C" void __cdecl free(void* memory);         // 0x00BFEA14
void __cdecl operator delete(void* block);           // 0x00BFE9BC

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CAnimFilterBase {                             // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

struct CModeBaseRoot {
    virtual void v00();
};

namespace CTCCreatureModeManager {
struct CModeBase : CModeBaseRoot {                   // vtable 0x012673CC
    long            m_type;                          // +0x04
    CAnimFilterBase m_filter;                        // +0x08
    CCharString     m_name;                          // +0x0c
    ~CModeBase() { }
};
}

namespace NCreatureMode {
struct __declspec(novtable) CModeDragonFlyingBase : CTCCreatureModeManager::CModeBase {   // vtable 0x01268658
    unsigned char m_pad10[0x0c];
    void*         m_pFlightPath;                     // +0x1c (heap block, freed with free)
    ~CModeDragonFlyingBase()
    {
        if (m_pFlightPath)
            free(m_pFlightPath);
    }
};
}

void DeleteDragonFlyingMode(NCreatureMode::CModeDragonFlyingBase* mode)   // makes VC7.1 emit the scalar deleting destructor
{
    delete mode;
}
