// Grade: DIFFER(510v504) -- villager lookup matches; retail spills the HasTC(0x23) bool to the byte later reused as the __false_type tag, keeps the new list head in edi and best in ebp (iterator in the frame), ours keeps best in the frame and the iterator in ebp; frame 0x2c v 0x30
// CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire() -- the nearest live free bonfire (object type 5) of the creature's village; failing that, the nearest scan-visible campfire with space within 22 units
// Leaf: GetPNearCampfire@CAIStateGroup_BanditSitRoundCampfire
#include <stddef.h>
#include <stdlib.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return Y * Y + X * X + Z * Z; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0 (const overload)
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char ObjectType;                   // +0x10 (5: bonfire)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long type) const { return (m_presentBits[type >> 5] >> (type & 31)) & 1; }
    CTCBase* GetTC(long type) const
    {
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThingObject : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CListNode { CListNode* Next; CListNode* Prev; CBaseIntelligentPointer Value; };   // 0x10 bytes
struct __false_type { };
struct CListIterator {
    CListNode* Node;
    CListIterator(CListNode* node) : Node(node) {}
};
struct CThingObjectList {                       // STLport list<CIntelligentPointer<CThingObject>>
    CListNode* Node;
    CThingObjectList(const CThingObjectList& other)
    {
        CListNode* node = (CListNode*)malloc(sizeof(CListNode));
        node->Next = node;
        node->Prev = node;
        Node = node;
        __false_type tag;
        InsertDispatch(begin(), other.begin(), other.end(), tag);
    }
    ~CThingObjectList()
    {
        clear();
        free(Node);
    }
    CListIterator begin() const { return CListIterator(Node->Next); }
    CListIterator end() const { return CListIterator(Node); }
    void clear();                               // 0x004B8050
    void InsertDispatch(CListIterator pos, CListIterator first, CListIterator last, const __false_type&);   // 0x00618100
};
struct CTCVillage {
    unsigned char _pad[0x44];
    CThingObjectList Campfires;                 // +0x44
};
struct CTCVillageMember;
CTCVillage* __fastcall GetVillage(CTCVillageMember* member);   // 0x007EE540 (manifest IS_THING_ALIVE is propagated)
struct CIsFreeBonfire {                         // empty functor (Ego_r.pdb: 1 byte)
    bool operator()(CThingObject* thing) const; // 0x0093B5E0
};
struct CIsNearCampfireWithSpace {
    float DistanceSq;
    C3DVector FromPos;
    CIsNearCampfireWithSpace(float distanceSq, const C3DVector& from) : DistanceSq(distanceSq), FromPos(from) {}
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
struct CThingSearchTools {
    bool GetPBestThing(const CIsNearCampfireWithSpace& filter, const CThingCompare_Nearest& compare, CThingObject** best);   // 0x0093B640
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroup_BanditSitRoundCampfire {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CThingObject* GetPNearCampfire();
};
CThingObject* CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire()
{
    CThingObject* best = 0;
    CTCVillageMember* member;
    bool hasMember = PBrain->PCreature->HasTC(0x23);
    if (hasMember)
        member = (CTCVillageMember*)PBrain->PCreature->GetTC(0x23);
    CTCVillage* village = GetVillage(member);
    if (village) {
        CThingObjectList campfires(village->Campfires);
        float bestDistance = 0.0f;
        for (CListNode* node = campfires.Node->Next; node != campfires.Node; node = node->Next) {
            if (node->Value.GetPItem() && !(node->Value.GetPItem()->m_flags91 & 1) && node->Value.GetPItem()->ObjectType == 5) {
                CThingObject* fire = (CThingObject*)node->Value.GetPItem();
                if (CIsFreeBonfire()(fire)) {
                    float distance = (fire->GetPosition() - PBrain->PCreature->GetPosition()).GetLengthSquared();
                    if (!best || distance < bestDistance) {
                        bestDistance = distance;
                        best = fire;
                    }
                }
            }
        }
        if (best)
            return best;
    }
    PeekThingSearchTools()->GetPBestThing(CIsNearCampfireWithSpace(484.0f, PBrain->PCreature->GetPosition()),
        CThingCompare_Nearest(PBrain->PCreature->GetPosition()), &best);
    return best;
}
