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
class CThing;

class CTCBase { public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    void RemoveFromGlobalTCs();                          // 0x00686DF0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {  // CIntelligentPointer<CThing>: generated destructor
public:

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class C3DVector { public: float X, Y, Z; };

struct CTCHighlightItem : CTCBase {
    bool                StartRunning;                    // +0x0c
    bool                Running;                         // +0x0d
    long                LifeTimer;                       // +0x10
    C3DVector           StartPos;                        // +0x14
    CIntelligentPointer<CThing> PParticleEmitter;                // +0x20
    void DoStopRunning();                                // 0x0077CCA0
    virtual ~CTCHighlightItem() { DoStopRunning(); }
};

CTCHighlightItem* NewCTCHighlightItem()
{
    return new CTCHighlightItem;
}