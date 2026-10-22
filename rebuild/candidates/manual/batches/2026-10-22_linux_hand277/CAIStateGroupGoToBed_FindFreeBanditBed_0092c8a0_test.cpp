// Behaviour test for CAIStateGroup_GoToBed::FindFreeBanditBed @ 0x0092C8A0: nearest free sleep point is chosen and claimed.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float M[6]; };
struct CThing;
struct CTCRegionLink;
struct CTCSleepPoint { CThing* user; void SetUser(CThing* u) { user = u; } };
struct CThingGraphic { CRightHandedSet set; const CRightHandedSet* GetRHSet() { return &set; } };
struct CThing {
    C3DVector pos; bool has4a; bool has23; bool hasac; unsigned char m_flags91;
    CTCRegionLink* link; CTCSleepPoint point; CThingGraphic* PGraphic; bool mine; bool blocked;
    const C3DVector& GetPosition() const { return pos; }
    bool HasTC(long t) const { return t == 0x4a ? has4a : t == 0x23 ? has23 : t == 0xac ? hasac : false; }
    void* GetTC(long t) const { return t == 0x23 ? (void*)link : (void*)&((CThing*)this)->point; }
};
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CIntelligentPointerThing(CThing* item = 0) : m_pItem(item) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CPointerListNode { CPointerListNode* _M_next; CPointerListNode* _M_prev; CIntelligentPointerThing _M_data; };
struct CRegionThing { CPointerListNode* _M_node; };
struct CTCRegionLink { CRegionThing* region; CRegionThing* GetPRegion() { return region; } };
struct CThingFilter_IsMySleepPoint {
    CThing* Creature;
    CThingFilter_IsMySleepPoint(CThing* creature) : Creature(creature) { }
    bool operator()(CThing* thing) const { return thing->mine; }
};
struct CThingFilter_IsBlocked { bool operator()(CThing* thing) const { return thing->blocked; } };
struct CAIBrain { CThing* Creature; };
struct CAIStateGroupBase { CAIBrain* PBrain; CThing* GetCreature() const { return PBrain->Creature; } };
struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointerThing PBed; bool IsBedSleepPointMarker; C3DVector BedPos; CRightHandedSet Rhset;
    CAIStateGroup_GoToBed() : PBed(0), IsBedSleepPointMarker(false) { }
    bool FindFreeBanditBed();
};
bool CAIStateGroup_GoToBed::FindFreeBanditBed()
{
    const CThing* creature = GetCreature();
    if (creature->HasTC(0x4a)) {
        CTCRegionLink* link;
        if (creature->HasTC(0x23))
            link = (CTCRegionLink*)creature->GetTC(0x23);
        CRegionThing* region = link->GetPRegion();
        if (region && region->_M_node->_M_next != region->_M_node) {
            CThingFilter_IsMySleepPoint isMine(GetCreature());
            CThingFilter_IsBlocked isBlocked;
            PBed = CIntelligentPointerThing(0);
            float bestDistanceSquared = 0.0f;
            CPointerListNode* head = region->_M_node;
            for (CPointerListNode* node = head->_M_next; node != head; node = node->_M_next) {
                CThing* bed = node->_M_data.GetPItem();
                if (bed && !(bed->m_flags91 & 1) && isMine(bed) && !isBlocked(bed)) {
                    CThing* me = GetCreature();
                    const C3DVector& bedPos = bed->GetPosition();
                    const C3DVector& myPos = me->GetPosition();
                    float dx = myPos.X - bedPos.X;
                    float dy = myPos.Y - bedPos.Y;
                    float dz = myPos.Z - bedPos.Z;
                    float distanceSquared = dz * dz + dy * dy + dx * dx;
                    if (!PBed.GetPItem() || bestDistanceSquared > distanceSquared) {
                        PBed.SetPItem(bed);
                        bestDistanceSquared = distanceSquared;
                        BedPos = bed->GetPosition();
                        Rhset = *bed->PGraphic->GetRHSet();
                    }
                }
            }
            if (PBed.GetPItem()) {
                IsBedSleepPointMarker = true;
                CTCSleepPoint* point;
                CThing* chosen = PBed.GetPItem();
                if (chosen->HasTC(0xac))
                    point = (CTCSleepPoint*)chosen->GetTC(0xac);
                point->SetUser(GetCreature());
                return true;
            }
        }
    }
    PBed = CIntelligentPointerThing(0);
    return false;
}

static CThing MakeBed(float x, bool mine, bool blocked, CThingGraphic* g)
{
    CThing b; b.pos.X = x; b.pos.Y = 0; b.pos.Z = 0; b.has4a = b.has23 = false; b.hasac = true; b.m_flags91 = 0;
    b.link = 0; b.point.user = 0; b.PGraphic = g; b.mine = mine; b.blocked = blocked; return b;
}
int main()
{
    int bad = 0;
    CThingGraphic g1, g2; g1.set.M[0] = 1; g2.set.M[0] = 2;
    CThing beds[4];
    beds[0] = MakeBed(10, true, false, &g1);
    beds[1] = MakeBed(3, true, false, &g2);
    beds[2] = MakeBed(1, false, false, &g1);
    beds[3] = MakeBed(2, true, true, &g1);
    CPointerListNode head; CPointerListNode nodes[4];
    head._M_next = &nodes[0];
    for (int i = 0; i < 4; ++i) { nodes[i]._M_data.m_pItem = &beds[i]; nodes[i]._M_next = i < 3 ? &nodes[i + 1] : &head; }
    CRegionThing region; region._M_node = &head;
    CTCRegionLink link; link.region = &region;
    CThing me = MakeBed(0, false, false, 0); me.has4a = true; me.has23 = true; me.link = &link;
    CAIBrain brain; brain.Creature = &me;
    CAIStateGroup_GoToBed sg; sg.PBrain = &brain;
    if (!sg.FindFreeBanditBed() || sg.PBed.GetPItem() != &beds[1] || sg.BedPos.X != 3 || sg.Rhset.M[0] != 2) ++bad;
    if (!sg.IsBedSleepPointMarker || beds[1].point.user != &me) ++bad;
    beds[1].mine = false; beds[0].m_flags91 = 1;
    sg.IsBedSleepPointMarker = false;
    if (sg.FindFreeBanditBed() || sg.PBed.GetPItem() != 0 || sg.IsBedSleepPointMarker) ++bad;
    beds[0].m_flags91 = 0;
    me.has4a = false;
    if (sg.FindFreeBanditBed() || sg.PBed.GetPItem() != 0) ++bad;
    if (bad == 0) printf("0092c8a0_TEST PASS\n"); else printf("0092c8a0_TEST FAIL %d\n", bad);
    return 0;
}
