// CBrainOutput::`scalar deleting destructor'(unsigned int flags) -- the inlined CBrainOutput destructor, then operator delete when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D61B0, length 82 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (LookAtThing, +0x40),
// the action's destroy function through its count block (`call [eax+4]`), operator delete
// @ 0x00BFE9BC (the count block, and `this` when flags & 1).
// CBrainOutput is not polymorphic: VC7.1 still emits a non-virtual `??_G` for a class with a
// non-trivial destructor in every TU that `delete`s one (modelled by the Destroy helper); the
// destructor body (PDB layout: PAction +0x20, LookAtThing +0x40, reverse order) is the speed form
// (`test eax,eax`, immediate zero stores) -- the size form is the standalone destructor
// @ 0x006D60C0. The flag test is scheduled before the two null stores.
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // shares the base vtable in this TU (no store of its own in the destructor)
};

struct CCreatureActionBase;

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

struct C3DVector { float X, Y, Z; };

struct CNavigationInput {                       // 24 bytes, trivially destructible
    unsigned char _body[0x18];
};

struct CBrainOutput {
    unsigned long                     Types;            // +0x00
    long                              LookFlags;        // +0x04
    CNavigationInput                  NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;        // +0x20
    C3DVector                         MoveToPos;        // +0x28
    C3DVector                         LookAtPos;        // +0x34
    CIntelligentPointer               LookAtThing;      // +0x40
};

// The delete-expression that makes the compiler emit the (implicit-destructor) scalar deleting destructor.
void Destroy(CBrainOutput* p)                   // pulls the scalar deleting destructor into the TU
{
    delete p;
}