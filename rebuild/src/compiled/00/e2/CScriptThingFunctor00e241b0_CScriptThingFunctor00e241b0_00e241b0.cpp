// CScriptThingFunctor_00e241b0::~CScriptThingFunctor_00e241b0() -- this class's vtable (0x012C32F0), the CScriptThing member at +4 destroyed inline (its vtable 0x01238C8C, the counted block at +0xc released: destroy and delete at zero, both words cleared, CBaseClass::~CBaseClass @ 0x0099A2E0), then the functor base's vtable (0x012C2FF4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E241B0, length 96 bytes.
// Leaf: ??1CScriptThingFunctor_00e241b0@@QAE@XZ
// The chunk label "CIsThingAlive::~CIsThingAlive" is shared by two functors with different vtables; named by address. Shape: `this` in edi, the member in esi.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_00e241b0 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
class __declspec(novtable) CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0 (the base vtable store is elided: novtable)
};
struct CCountedPointer_00e241b0 {                      // the thing and its count block (a member object: its destructor runs after the vtable store)
    void*               m_pThing;                  // +0x00
    SCountBlock_00e241b0*   m_pCount;                  // +0x04
    __forceinline ~CCountedPointer_00e241b0()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pThing = 0;
        m_pCount = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedPointer_00e241b0 m_thing;                  // +0x04
    virtual ~CScriptThing() {}
};
struct CThingFunctorBase_012c2ff4 {                // vtable 0x012C2FF4
    virtual void v00();
    ~CThingFunctorBase_012c2ff4() { }              // non-virtual: keeps the deleting destructor out of the TU
};
struct CScriptThingFunctor_00e241b0 : CThingFunctorBase_012c2ff4 {   // vtable 0x012C32F0
    CScriptThing m_thing;                          // +0x04
    virtual void v00();
    ~CScriptThingFunctor_00e241b0();
};

CScriptThingFunctor_00e241b0::~CScriptThingFunctor_00e241b0()
{
}