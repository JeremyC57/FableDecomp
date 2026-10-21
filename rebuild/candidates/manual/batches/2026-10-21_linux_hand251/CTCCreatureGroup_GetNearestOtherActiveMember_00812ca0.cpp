// CTCCreatureGroup::GetNearestOtherActiveMember(const CThingAICreature* creature) const -- walks the group's member map and returns the active member (other than the creature itself, matched by unique id) nearest to the creature; null for a null creature
// Leaf: GetNearestOtherActiveMember
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812CA0, 384 bytes (`ret 4`).
// Callees: CThing::GetPosition() @ 0x004C73D0 (the query creature) and its byte-identical copy @ 0x006A4D00 (the members),
// CIntelligentPointer<CThingAICreature const>::GetPItem() @ 0x00A01B50.
// Layout: Ego_r.pdb CTCCreatureGroup: Members (STLport map<unsigned __int64, CGroupMemberData>, header pointer) +0x18;
// node: key +0x10, CGroupMemberData {PMember +0x18, IsActive +0x20}; CThing::UniqueID +0x18 (as the landed DrawPrepare rows).
// Shape: the map walk is the inlined STLport _M_increment; the best distance lives in the dead argument slot and starts as the
// integer zero; retail compares a squared distance against the squared best (squared) distance, kept as written.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CThingAICreature : CThing {
    const C3DVector& GetPosition() const;                   // 0x006A4D00 (the members' out-of-line copy; the query creature's call goes through CThing's 0x004C73D0)
};
struct CIntelligentPointer_AICreature {                     // CIntelligentPointer<CThingAICreature const>: 8 bytes
    void* Data[2];
    const CThingAICreature* GetPItem() const;               // 0x00A01B50
};
struct CGroupMemberData {
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

struct CTCCreatureGroup {
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
    C3DVector pos = static_cast<const CThing*>(creature)->GetPosition();
    unsigned __int64 id = creature->UniqueID;
    const CThingAICreature* nearest = 0;
    float bestDistance = 0;
    for (CMemberNode* it = Members->Left; it != Members; it = IncrementNode(it)) {
        if (!it->Data.IsActive)
            continue;
        if (it->Key == id)
            continue;
        const CThingAICreature* member = it->Data.PMember.GetPItem();
        if (nearest != 0) {
            float distance = (member->GetPosition() - pos).GetLengthSquared();
            if (distance < bestDistance * bestDistance) {
                nearest = it->Data.PMember.GetPItem();
                bestDistance = distance;
            }
        } else {
            nearest = member;
            bestDistance = (it->Data.PMember.GetPItem()->GetPosition() - pos).GetLengthSquared();
        }
    }
    return nearest;
}
