// CAIStateGroup_NonVillageGuardPatrol::Update() -- follow track nodes while AIAction_MoveToPos asks for repetition.
// Native: Fable.exe TLC VC7.1 @ 0x0093cb20, 306-byte body (320 including 14 bytes of int3 padding).
// Retail evidence: CurrentGuardpostThing is the 8-byte CIntelligentPointer<const CThingTrackNode> at +0x14; successor is +0xd0.
// Native callees: CBaseIntelligentPointer::{CBaseIntelligentPointer,GetPItem,SetPItem,~CBaseIntelligentPointer}
// @ 0x00a01b10/0x00a01b50/0x00a01b90/0x00a01c10; GetPosition @ 0x006a4d00;
// CAIStateGroupBase::AIAction_MoveToPos @ 0x0097ec00; PeekTrackStartNode @ 0x00836da0;
// CAIStateGroupBase::Terminate @ 0x008fcf40. The generated CThingTrackNode header is unavailable and its donor
// layout differs on the host; the native view retains its CBaseObject base plus native successor +0xd0.
// The generated CAIStateGroupBase is a flattened data layout with its existing +0 vptr word; this view adds
// no virtuals (and therefore no additional vptr). CBaseIntelligentPointer's real virtual destructor models
// its native two-word typed pointer and its converting temporaries.
#include "engine/CAIStateGroupBase.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CBaseObject.h"

// PDB: egor_pdb_names.tsv20904 establishes by-value vector and bool&;
// line20318 establishes const PeekTrackStartNode. struct_layouts_egor.tsv142325
// names CurrentGuardpostThing as CIntelligentPointer<CThingTrackNode const>.
// C3DVector is the donor 12-byte X/Y/Z vector. Its native inlined copy
// constructor assigns each component; default construction leaves them unset.
// CThing's donor view is retained above as provenance, but does not extend to
// native track-node layout on a 64-bit host. This view addresses the native
// successor after its real CBaseObject prefix, using member access on the host.
// The 0xc8-byte native state is independent of sizeof(CThing) in the donor view.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) { X=v.X; Y=v.Y; Z=v.Z; }
};
class CThingTrackNode : public CBaseObject { public:
    unsigned char _nativeThingAndTrackState[0xd0 - 8];
    CThingTrackNode* m_pNextNode;
    const C3DVector* GetPosition() const; // 0x006a4d00
    const CThingTrackNode* PeekTrackStartNode() const; // 0x00836da0
};
struct CBaseObjectPointer {
    CBaseObject* object;
    unsigned long refcount;
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer(); // 0x00a01c10
    CBaseObjectPointer* PData;
    CBaseObject* GetPItem() const; // 0x00a01b50
    void SetPItem(CBaseObject* item); // 0x00a01b90
    CBaseIntelligentPointer(); // 0x00a01b10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(item))); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    { SetPItem(other.GetPItem()); return *this; }
};
struct CAIStateGroup_NonVillageGuardPatrol : CAIStateGroupBase {
    CIntelligentPointer<const CThingTrackNode> CurrentGuardpostThing; // +0x14 CIntelligentPointer<const CThingTrackNode>
    long AIAction_MoveToPos(long, C3DVector, bool&, long, bool, bool, const CThing*, bool, float);
    void Terminate();
    void Update();
};

void CAIStateGroup_NonVillageGuardPatrol::Update()
{
    bool reached = false;
    if (CurrentGuardpostThing.GetPItem() == 0)
        return;
    for (;;) {
        const CThingTrackNode* movingNode = (const CThingTrackNode*)CurrentGuardpostThing.GetPItem();
        long result = AIAction_MoveToPos(2, *movingNode->GetPosition(), reached, 3, true, true, 0, true, 0.0f);
        if (CurrentGuardpostThing.GetPItem() != 0 && result == 4)
            continue;
        if (result != 2) {
            Terminate();
            return;
        }
        const CThingTrackNode* node = (const CThingTrackNode*)CurrentGuardpostThing.GetPItem();
        const CThingTrackNode* next = node->m_pNextNode;
        if (next == 0) {
            next = ((const CThingTrackNode*)CurrentGuardpostThing.GetPItem())->PeekTrackStartNode();
            if (next != 0)
                CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(next);
        } else {
            CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(next);
        }
        if (CurrentGuardpostThing.GetPItem() == 0)
            return;
    }
}