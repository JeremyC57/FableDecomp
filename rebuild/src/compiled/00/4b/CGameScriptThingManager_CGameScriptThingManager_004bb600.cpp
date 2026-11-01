// CGameScriptThingManager::~CGameScriptThingManager() -- destroy the CScriptThing member at +4 inline (its vtable, the counted block at +0xc released: destroy and delete at zero, both words cleared, CBaseClass::~CBaseClass), then the base: clear the manager (0x004BA810) and free the buffer at +0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BB600, length 94 bytes.
// Callees: the count block's destroy (slot +4 with the object at +8 in ecx), operator delete @ 0x00BFE9BC, CBaseClass::~CBaseClass @ 0x0099A2E0, the clear @ 0x004BA810 (on `this`;
// the manifest label "remove_if<...>" is propagated), free @ 0x00BFEA14. Vtable 0x01238C8C = CScriptThing (rtti-vtables.tsv). Shape: the member first (reverse
// declaration order), then the inlined base destructor; `this` in edi, the member in esi.
// Naming (naming wave 24): the "base" at +0 is Ego_r.pdb's first member, list<CScriptThingInfo> ScriptThings, whose inline destructor
//   clears it through ?clear@?$_List_base@VCScriptThingInfo@CGameScriptThingManager@@V?$allocator@...@std@@@std@@QAEXXZ 0x004BA810
//   (labels_chunk691; the landed own row) and frees the sentinel; the second member is NullThing. Was a CScriptThingListBase_004bb600
//   stand-in base with a Clear(); the members are destroyed in reverse order as before (NullThing, then ScriptThings).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SCountBlock_004bb600 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
class __declspec(novtable) CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0 (the base vtable store is elided: novtable)
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
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedPointer_004bb600 m_thing;              // +0x04
    virtual ~CScriptThing() {}
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _List_base {      // STLport list base: the sentinel node pointer
public:
    void* _M_node;                                 // +0x00
    void clear();                                  // 0x004BA810
    ~_List_base()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };
}

class CGameScriptThingManager { public:
    class CScriptThingInfo;
    std::list<CScriptThingInfo> ScriptThings;      // +0x00
    CScriptThing NullThing;                        // +0x04
    ~CGameScriptThingManager();
};

CGameScriptThingManager::~CGameScriptThingManager()
{
}