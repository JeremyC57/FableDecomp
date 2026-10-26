// CAIBrain_0088bb60::ForceResetWithOutput() -- build a CBrainOutput on the stack (0x00835620), force-reset the brain with it (0x0088BA40, result dropped) and destroy the output: its look-at intelligent pointer (+0x40, 0x00A01C10) then its counted action (+0x20/+0x24: the count block's references decremented, the action destroyed through the block's function and the block freed at zero)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088BB60, length 79 bytes.
// Callees: CBrainOutput::CBrainOutput @ 0x00835620, CAIBrain::ForceReset(CBrainOutput*) @ 0x0088BA40, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, operator delete @ 0x00BFE9BC. The chunk label "CManager::GetEmbeddedEventNames" is propagated; the 0x48-byte frame is the CBrainOutput of the landed 0x00835620 constructor. Shape: the output at [esp+4], `pop esi` before the inlined release, `ret`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCreatureActionBase;
struct SCountBlock {                               // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;                // +0x08
};
struct CCountedPointer_CCreatureActionBase {       // {object, count block}
    CCreatureActionBase* m_pObject;                // +0x00
    SCountBlock*         m_pCount;                 // +0x04
    __forceinline ~CCountedPointer_CCreatureActionBase()
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
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                 // +0x04
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // no destructor of its own (no vtable store before the base call)
};
class C3DVector { public: float X, Y, Z; };
class CNavigationInput { public: long m_00, m_04, m_08, m_0c, m_10, m_14; };
class CBrainOutput { public:                              // 0x48 bytes (Ego_r.pdb)
    unsigned long                       Types;             // +0x00
    long                                LookFlags;         // +0x04
    CNavigationInput                    NavigationInput;   // +0x08
    CCountedPointer_CCreatureActionBase PAction;           // +0x20
    C3DVector                           MoveToPos;         // +0x28
    C3DVector                           LookAtPos;         // +0x34
    CIntelligentPointer                 LookAtThing;       // +0x40
    CBrainOutput();                                // 0x00835620
};

struct CAIBrain_0088bb60 {
    bool ForceReset(CBrainOutput* output);         // 0x0088BA40
    void ForceResetWithOutput();
};

void CAIBrain_0088bb60::ForceResetWithOutput()
{
    CBrainOutput output;
    ForceReset(&output);
}