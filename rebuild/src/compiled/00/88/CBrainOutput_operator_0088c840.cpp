// CBrainOutput::operator=(const CBrainOutput&) -- memberwise assignment: flags, navigation input, counted action pointer, the two positions and the look-at intelligent pointer
// Leaf: ??4CBrainOutput
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088C840, 187 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (LookAtThing +0x40), the
//          action's destroy function through its count block (`call [eax+4]`), operator delete @ 0x00BFE9BC.
// Layout from Ego_r.pdb (CBrainOutput, 72 bytes): Types +0, LookFlags +4, NavigationInput +8 (24 bytes: C3DVector
//          Destination, Proximity, MovementType, Speed), PAction +0x20, MoveToPos +0x28, LookAtPos +0x34, LookAtThing +0x40.
// Shape: C3DVector has a user copy constructor and no operator=, so each vector (and the implicit
//          CNavigationInput assignment) copies through a `lea` pointer register. The counted pointer's inline
//          operator= takes the source count and object into locals (ebx/ebp) first, skips everything when the count blocks are equal, releases the
//          old block (count re-read after the decrement), then stores object, count and bumps it.
struct CBaseObject;

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
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
    CCountedPointer_CCreatureActionBase& operator=(const CCountedPointer_CCreatureActionBase& other)
    {
        SCountBlock* count = other.m_pCount;
        CCreatureActionBase* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CNavigationInput {                       // 24 bytes
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long MovementType;                          // +0x10 EMovementType
    float Speed;                                // +0x14
};

struct CBrainOutput {
    unsigned long                     Types;            // +0x00
    long                              LookFlags;        // +0x04
    CNavigationInput                  NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;        // +0x20
    C3DVector                         MoveToPos;        // +0x28
    C3DVector                         LookAtPos;        // +0x34
    CIntelligentPointer               LookAtThing;      // +0x40
    CBrainOutput& operator=(const CBrainOutput& other);
};

CBrainOutput& CBrainOutput::operator=(const CBrainOutput& other)
{
    Types = other.Types;
    LookFlags = other.LookFlags;
    NavigationInput = other.NavigationInput;
    PAction = other.PAction;
    MoveToPos = other.MoveToPos;
    LookAtPos = other.LookAtPos;
    LookAtThing = other.LookAtThing;
    return *this;
}