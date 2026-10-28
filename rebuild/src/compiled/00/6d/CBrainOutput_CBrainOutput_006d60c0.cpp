// CBrainOutput::~CBrainOutput() -- release the look-at thing and the shared action, leaving the action pointer null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D60C0, length 57 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (LookAtThing, +0x40),
// the action's destroy function through the count block (`call [eax+4]` with the object in ecx),
// operator delete @ 0x00BFE9BC on the count block.
// Layout from Ego_r.pdb (CBrainOutput, 72 bytes): Types +0, LookFlags +4, NavigationInput +8,
// PAction (CCountedPointer<CCreatureActionBase>) +0x20, MoveToPos +0x28, LookAtPos +0x34,
// LookAtThing (CIntelligentPointer<CThing const>) +0x40. Members are destroyed in reverse order:
// the intelligent pointer out of line, then the counted pointer's inline destructor (decrement,
// re-read, destroy + free on zero, both words nulled). The counted pointer's destructor must be
// `__forceinline` (VC7.1 otherwise calls it out of line); under plain /O2 the compiler then feeds
// the two compares and the two null stores from one zero register (`xor edi,edi`), where the same
// body inlined into the scalar deleting destructor @ 0x006D61B0 uses immediates.

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // shares the base vtable in this TU (no store of its own in the destructor)
};

class CCreatureActionBase;

struct SCountBlock {                            // the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;             // +0x08
};

struct CCountedPointer_CCreatureActionBase {    // {object, count block}
    CCreatureActionBase* m_pObject;             // +0x00
    SCountBlock*         m_pCount;              // +0x04
    __forceinline ~CCountedPointer_CCreatureActionBase()   // forced: VC7.1 would call it out of line
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

class C3DVector { public: float X, Y, Z; };

class CNavigationInput { public:                       // 24 bytes, trivially destructible
    unsigned char _body[0x18];
};

class CBrainOutput { public:
    unsigned long                     Types;            // +0x00
    long                              LookFlags;        // +0x04
    CNavigationInput                  NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;        // +0x20
    C3DVector                         MoveToPos;        // +0x28
    C3DVector                         LookAtPos;        // +0x34
    CIntelligentPointer               LookAtThing;      // +0x40
    ~CBrainOutput();
};

CBrainOutput::~CBrainOutput()
{
}