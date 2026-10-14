// NEntityEvents::CEventHitBase::`scalar deleting destructor'(unsigned int) -- this class's vtable, the counted pointer at +8 released (destroy and delete its block at zero, both words cleared), the CEventBase vtable, the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811C30, length 86 bytes.
// Callees: the count block's destroy (slot +4, the object at +8 in ecx), operator delete @ 0x00BFE9BC (the block, then the object). Vtables 0x01272344 =
// NEntityEvents::CEventHitBase, 0x0124F2C4 = NEntityEvents::CEventBase (rtti-vtables.tsv). Shape: the inlined CCountedPointer destructor with its two clears
// scheduled after the flag test; the base vtable store before the `je`. Both the pointer release and the virtual destructor are `__forceinline`: left to the
// heuristics the release stays out of line and the deleting destructor shrinks to `call ??1 ; delete`. The chunk label "CEventHitBy" names a derived class; the stored vtable is CEventHitBase's.
// Leaf: ??_GCEventHitBase@NEntityEvents@@UAEPAXI@Z
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_00811c30 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};

struct CCountedPointer_00811c30 {                     // 8 bytes: the object and its count block
    void*                m_pObject;                // +0x00
    SCountBlock_00811c30*    m_pCount;                // +0x04
    __forceinline ~CCountedPointer_00811c30()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

namespace NEntityEvents {
struct CEventBase {                                // vtable 0x0124F2C4
    virtual ~CEventBase() { }
    long m_eventType;                              // +0x04
};

struct CEventHitBase : CEventBase {                // vtable 0x01272344
    CCountedPointer_00811c30 m_hitInfo;            // +0x08
    __forceinline virtual ~CEventHitBase() { }
};
}

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_00811c30()
{
    return new NEntityEvents::CEventHitBase;
}