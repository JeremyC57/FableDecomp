// CTCHighlightItem::`scalar deleting destructor'(unsigned int) -- its vtable, DoStopRunning, ~PParticleEmitter, ~CTCBase, free for flag bit 0
// Leaf: ??_GCTCHighlightItem
// Retail: Fable.exe 0x0077D140, 51 bytes; CTCHighlightItem vtable 0x0126AF14 slot 0 (rtti-vtables.tsv). The
// manifest label "CThingTrackNode::`vector_deleting_destructor'" is propagated.
// Callees: CTCHighlightItem::DoStopRunning 0x0077CCA0 (the destructor body), CBaseIntelligentPointer::
// ~CBaseIntelligentPointer 0x00A01C10 (+0x20), CTCBase::~CTCBase 0x00686830, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CTCHighlightItem (0x28 bytes, same in retail). engine/CTCHighlightItem.h is a flat dump
// (vptr as a data word, the intelligent pointer as bytes) that cannot carry the base destructor and the
// member's destructor, so the class is restated with the same member names and offsets.
// Shape: a user virtual destructor whose body calls DoStopRunning after the vtable store; the intelligent
// pointer's generated destructor calls the base destructor directly; then the out-of-line CTCBase destructor.
struct CThing;

struct CTCBase {
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    void RemoveFromGlobalTCs();                          // 0x00686DF0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PItem;                                         // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>: generated destructor
    virtual void v00();
};

struct C3DVector { float X, Y, Z; };

struct CTCHighlightItem : CTCBase {
    bool                StartRunning;                    // +0x0c
    bool                Running;                         // +0x0d
    long                LifeTimer;                       // +0x10
    C3DVector           StartPos;                        // +0x14
    CIntelligentPointer PParticleEmitter;                // +0x20
    void DoStopRunning();                                // 0x0077CCA0
    virtual ~CTCHighlightItem() { DoStopRunning(); }
};

CTCHighlightItem* NewCTCHighlightItem()
{
    return new CTCHighlightItem;
}