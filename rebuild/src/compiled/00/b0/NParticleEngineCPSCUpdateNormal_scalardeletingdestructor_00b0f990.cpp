// NParticleEngine::CPSCUpdateNormal::`scalar deleting destructor'(unsigned int) -- stores the CPSCUpdateNormal vtable, kills all decal emitters, clears the array member at +0x9c and frees its block, destroys the string at +0x14, runs CBase::~CBase and frees on bit 0
// Leaf: ??_GCPSCUpdateNormal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F990, length 81 bytes (vtable 0x0129FC1C slot 0).
// Callees: CPSCUpdateNormal::KillAllDecalEmitters @ 0x00B0C730, the array's element teardown @ 0x00B0F630
// (manifest "CGuiControlTreePane::SortTreeRecursively", propagated), free @ 0x00BFEA14,
// CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>", propagated), CBase::~CBase
// @ 0x0099A300, operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the NParticleEngine::CPSCUpdateNormal vtable 0x0129FC1C.
// Layout: no engine header; modelled as a polymorphic CBase-derived class with a CCharString at +0x14
// and an array member (data pointer first) at +0x9c.
// Shape: the destructor body call, then members in reverse order: the array's inline destructor
// (`DestroyElements(); if (data) free(data);`, member address kept in edi), the string, then the base.
// The array destructor must be `__forceinline` (a plain inline one is emitted out of line).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

namespace N_0099a300 { struct CBase {                                     // root polymorphic base
    CBase();
    virtual ~CBase();                              // 0x0099A300
}; } using N_0099a300::CBase;

namespace NParticleEngine {

struct CDecalEmitterArray {                        // array whose destructor tears the elements down, then frees
    void* m_pData;
    long  m_size;
    long  m_capacity;
    void DestroyElements();                        // 0x00B0F630
    CDecalEmitterArray() : m_pData(0), m_size(0), m_capacity(0) { }
    __forceinline ~CDecalEmitterArray()
    {
        DestroyElements();
        if (m_pData) free(m_pData);
    }
};

struct CPSCUpdateNormal : CBase {                  // vtable 0x0129FC1C
    unsigned char      _pad_0x04[0x10];            // +0x04
    CCharString        m_name;                     // +0x14
    unsigned char      _pad_0x18[0x84];            // +0x18
    CDecalEmitterArray m_decalEmitters;            // +0x9c
    void KillAllDecalEmitters();                   // 0x00B0C730
    CPSCUpdateNormal() { }
    __forceinline virtual ~CPSCUpdateNormal() { KillAllDecalEmitters(); }
};

}

NParticleEngine::CPSCUpdateNormal* Create_CPSCUpdateNormal()
{
    return new NParticleEngine::CPSCUpdateNormal;
}