// CGameScriptThingManager::~CGameScriptThingManager() -- destroy the CScriptThing member at +4 inline (its vtable, the counted block at +0xc released: destroy and delete at zero, both words cleared, CBase::~CBase), then the base: clear the manager (0x004BA810) and free the buffer at +0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BB600, length 94 bytes.
// Callees: the count block's destroy (slot +4 with the object at +8 in ecx), operator delete @ 0x00BFE9BC, CBase::~CBase @ 0x0099A2E0, the clear @ 0x004BA810 (on `this`;
// the manifest label "remove_if<...>" is propagated), free @ 0x00BFEA14. Vtable 0x01238C8C = CScriptThing (rtti-vtables.tsv). Shape: the member first (reverse
// declaration order), then the inlined base destructor; `this` in edi, the member in esi.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SCountBlock_004bb600 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct __declspec(novtable) CBase {
    virtual ~CBase();                              // 0x0099A2E0 (the base vtable store is elided: novtable)
};
struct CCountedPointer_004bb600 {                  // the thing and its count block (a member object: its destructor runs after the vtable store)
    void*                 m_pThing;                // +0x00
    SCountBlock_004bb600* m_pCount;                // +0x04
    __forceinline ~CCountedPointer_004bb600()
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
class CScriptThing : public CBase { public:                      // vtable 0x01238C8C
    CCountedPointer_004bb600 m_thing;              // +0x04
    virtual ~CScriptThing() {}
};

struct CScriptThingListBase_004bb600 {             // the base at +0: the thing buffer, cleared then freed
    void* m_pBuffer;                               // +0x00
    void Clear();                                  // 0x004BA810
    ~CScriptThingListBase_004bb600()
    {
        Clear();
        if (m_pBuffer)
            free(m_pBuffer);
    }
};

class CGameScriptThingManager : public CScriptThingListBase_004bb600 { public:
    CScriptThing m_nullThing;                      // +0x04
    ~CGameScriptThingManager();
};

CGameScriptThingManager::~CGameScriptThingManager()
{
}