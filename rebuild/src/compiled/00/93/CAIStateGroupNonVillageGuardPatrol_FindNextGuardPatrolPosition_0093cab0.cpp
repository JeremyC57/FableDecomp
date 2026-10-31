// CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition() -- move CurrentGuardpostThing on to the current track node's successor, or wrap to the track's start node when the node has none; stay put when neither exists
// Leaf: ?FindNextGuardPatrolPosition@CAIStateGroup_NonVillageGuardPatrol@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093CAB0, length 104 bytes, thiscall, plain `ret`.
// Identity: FableWin.pdb ?FindNextGuardPatrolPosition@CAIStateGroup_NonVillageGuardPatrol@@AAEXXZ (private, no arguments);
//   member CurrentGuardpostThing (Ego_r.pdb: CIntelligentPointer<const CThingTrackNode> at +0x14; its converting temporary stores
//   the CIntelligentPointer<const CThingTrackNode> vtable 0x0125A894, as in the landed sibling Cleanup 0x0093CA60). The landed
//   sibling Update 0x0093CB20 carries this exact body inlined (same successor read, same per-arm assignments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice on the member, once on the temporary),
//   CThingTrackNode::PeekTrackStartNode() const 0x00836DA0, CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10,
//   SetPItem 0x00A01B90 (temporary, then member), ~CBaseIntelligentPointer 0x00A01C10.
// Shape: one whole `CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(next);` statement per arm. VC7.1 merges
//   the two identical tails into one block and keeps the merged block's this-before-argument order (`lea ecx,[esp+8]` before
//   `push edi`, `mov ecx,esi` before `push eax`) and the member address in esi / the successor in edi. A single assignment
//   after an early-return null test pushes before forming ecx and swaps esi/edi (the parked 2026-10-15 form).
// Layout: `this` from engine/CAIStateGroupBase.h (the `#define` include pattern of the landed sibling Update). The track node's
//   successor pointer at +0xD0 is the retail TLC offset (Ego_r's layout of CThingTrackNode is longer; the PDB declares an
//   out-of-line GetNextNode() that retail inlines as this read). m_pNextNode is the landed sibling's descriptive spelling.
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase

class CBaseObject;
class CBaseObjectPointer;

class CBaseIntelligentPointer {                          // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <const CThingTrackNode>: vtable 0x0125A894
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingTrackNode {
public:
    unsigned char          _nativeThingAndTrackState[0xd0];   // CThing base and track state (retail TLC offsets)
    CThingTrackNode*       m_pNextNode;                       // +0xD0 successor (inlined GetNextNode)
    const CThingTrackNode* PeekTrackStartNode() const;        // 0x00836DA0
};

class CAIStateGroupBase : public CAIStateGroupBaseLayout {
};

class CAIStateGroup_NonVillageGuardPatrol : public CAIStateGroupBase {
public:
    CIntelligentPointer<const CThingTrackNode> CurrentGuardpostThing;   // +0x14
private:
    void FindNextGuardPatrolPosition();
};

void CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition()
{
    const CThingTrackNode* next = CurrentGuardpostThing.GetPItem()->m_pNextNode;
    if (next != 0)
        CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(next);
    else {
        next = CurrentGuardpostThing.GetPItem()->PeekTrackStartNode();
        if (next != 0)
            CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(next);
    }
}