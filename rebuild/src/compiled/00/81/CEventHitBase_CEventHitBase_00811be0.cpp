// CEventHitBase::~CEventHitBase() -- this class's vtable (0x01272344), the counted pointer at +0x8 released (the count block's references decremented, the object destroyed through the block's function and the block freed at zero; object and block then cleared), then the CEventBase base's inline destructor stores its vtable (0x0124F2C4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811BE0, length 80 bytes.
// Leaf: ??1CEventHitBase@NEntityEvents@@QAE@XZ
// Shape: a non-virtual destructor in a polymorphic class, `this` in esi, the counted pointer's destructor inlined, `ret`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CHitObject_00811be0;
struct SCountBlock_00811be0 {                           // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CHitObject_00811be0*);       // +0x04
    CHitObject_00811be0* m_pObject;                              // +0x08
};
struct CCountedPointer_00811be0 {                       // {object, count block}
    CHitObject_00811be0*           m_pObject;                    // +0x00
    SCountBlock_00811be0* m_pCount;                     // +0x04
    __forceinline ~CCountedPointer_00811be0()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
namespace NEntityEvents {
struct CEventBase {                                  // vtable 0x0124F2C4
    virtual void v00();
    ~CEventBase() { }                                // inline: the vtable store
};
struct CEventHitBase : CEventBase {                        // vtable 0x01272344
    long          m_4;                             // +0x04
    CCountedPointer_00811be0 m_pointer;             // +0x8
    ~CEventHitBase();
};

}
NEntityEvents::CEventHitBase::~CEventHitBase()
{
}