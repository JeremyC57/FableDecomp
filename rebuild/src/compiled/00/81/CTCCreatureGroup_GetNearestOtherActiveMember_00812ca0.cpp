// CTCCreatureGroup::GetNearestOtherActiveMember(const CThingAICreature* creature) const -- walks the group's member map and returns the active member (other than the creature itself, matched by unique id) nearest to the creature; null for a null creature
// Leaf: GetNearestOtherActiveMember
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812CA0, 384 bytes (`ret 4`).
// Callees: CThing::GetPosition() @ 0x004C73D0 (the query creature) and its byte-identical copy @ 0x006A4D00 (the members),
// CIntelligentPointer<CThingAICreature const>::GetPItem() @ 0x00A01B50.
// Layout: Ego_r.pdb CTCCreatureGroup: Members (STLport map<unsigned __int64, CGroupMemberData>, header pointer) +0x18;
// node: key +0x10, CGroupMemberData {PMember +0x18, IsActive +0x20}; CThing::UniqueID +0x18 (as the landed DrawPrepare rows).
// Shape: the map walk is the inlined STLport _M_increment; the best distance lives in the dead argument slot and starts as the
// integer zero; retail compares a squared distance against the squared best (squared) distance, kept as written.
#include "engine/C3DVector.h"
class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    const C3DVector& GetPos() const;                        // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb)
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;                       // 0x006A4D00 ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ (Ego_r donor order; the members' call; the query creature's call goes through CThing::GetPos 0x004C73D0)
};
class CThingAICreature : public CThingPhysical { public:
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointer_AICreature {                     // CIntelligentPointer<CThingAICreature const>: 8 bytes
    void* Data[2];
};
class CGroupMemberData { public:
    CIntelligentPointer_AICreature PMember;                 // +0x00
    bool                           IsActive;                // +0x08
    unsigned long                  JoinedFrame;             // +0x0c
    unsigned long                  DeactivatedFrame;        // +0x10
};
struct CMemberNode {                                        // _Rb_tree_node<pair<const unsigned __int64, CGroupMemberData> >
    long             Colour;
    CMemberNode*     Parent;
    CMemberNode*     Left;
    CMemberNode*     Right;
    unsigned __int64 Key;                                   // +0x10
    CGroupMemberData Data;                                  // +0x18
};
inline CMemberNode* IncrementNode(CMemberNode* node)        // STLport _Rb_global::_M_increment
{
    if (node->Right != 0) {
        node = node->Right;
        while (node->Left != 0)
            node = node->Left;
    } else {
        CMemberNode* y = node->Parent;
        while (node == y->Right) {
            node = y;
            y = y->Parent;
        }
        if (node->Right != y)
            node = y;
    }
    return node;
}

class CTCCreatureGroup { public:
    void*         __vftable;                                // +0x00
    unsigned char _pad04[0x8];                              // +0x04
    unsigned long MemberCount;                              // +0x0c
    unsigned long ActiveMemberCount;                        // +0x10
    bool          ShutdownOnEmpty;                          // +0x14
    unsigned char _pad15[0x3];
    CMemberNode*  Members;                                  // +0x18 map header
    const CThingAICreature* GetNearestOtherActiveMember(const CThingAICreature* creature) const;
};

const CThingAICreature* CTCCreatureGroup::GetNearestOtherActiveMember(const CThingAICreature* creature) const
{
    if (creature == 0)
        return 0;
    C3DVector pos = static_cast<const CThing*>(creature)->GetPos();
    unsigned __int64 id = creature->UniqueID;
    const CThingAICreature* nearest = 0;
    float bestDistance = 0;
    for (CMemberNode* it = Members->Left; it != Members; it = IncrementNode(it)) {
        if (!it->Data.IsActive)
            continue;
        if (it->Key == id)
            continue;
        const CThingAICreature* member = ((CThingAICreature const*)((CBaseIntelligentPointer*)&(it->Data.PMember))->GetPItem());
        if (nearest != 0) {
            float distance = (member->PeekPos() - pos).GetSquaredMagnitude();
            if (distance < bestDistance * bestDistance) {
                nearest = ((CThingAICreature const*)((CBaseIntelligentPointer*)&(it->Data.PMember))->GetPItem());
                bestDistance = distance;
            }
        } else {
            nearest = member;
            bestDistance = (((CThingAICreature const*)((CBaseIntelligentPointer*)&(it->Data.PMember))->GetPItem())->PeekPos() - pos).GetSquaredMagnitude();
        }
    }
    return nearest;
}