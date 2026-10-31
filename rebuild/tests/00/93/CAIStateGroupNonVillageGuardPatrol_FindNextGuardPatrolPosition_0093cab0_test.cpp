// Behaviour test for CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition (0x0093CAB0): a copy of the model with
// the out-of-line pointer and track-node callees implemented in place.
#include <cstdio>

class CBaseObject { public: int id; };

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++g_setCount; }
    static int g_setCount;
private:
    CBaseObject* PData;
};
int CBaseIntelligentPointer::g_setCount = 0;

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingTrackNode : public CBaseObject {
public:
    CThingTrackNode*       m_pNextNode;
    CThingTrackNode*       m_pStart;
    const CThingTrackNode* PeekTrackStartNode() const { return m_pStart; }
};

class CAIStateGroup_NonVillageGuardPatrol {
public:
    CAIStateGroup_NonVillageGuardPatrol() : CurrentGuardpostThing(0) {}
    CIntelligentPointer<const CThingTrackNode> CurrentGuardpostThing;
    void FindNextGuardPatrolPosition()
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
};

int main()
{
    CThingTrackNode a, b, c, lone;
    a.m_pNextNode = &b; b.m_pNextNode = &c; c.m_pNextNode = 0;
    a.m_pStart = b.m_pStart = c.m_pStart = &a;
    lone.m_pNextNode = 0; lone.m_pStart = 0;
    CAIStateGroup_NonVillageGuardPatrol group;
    group.CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(&a);
    group.FindNextGuardPatrolPosition();
    if (group.CurrentGuardpostThing.GetPItem() != &b) { std::printf("0093cab0_TEST FAIL\n"); return 1; }
    group.FindNextGuardPatrolPosition();
    if (group.CurrentGuardpostThing.GetPItem() != &c) { std::printf("0093cab0_TEST FAIL\n"); return 1; }
    group.FindNextGuardPatrolPosition();                       // end of track: wrap to the start node
    if (group.CurrentGuardpostThing.GetPItem() != &a) { std::printf("0093cab0_TEST FAIL\n"); return 1; }
    group.CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(&lone);
    int before = CBaseIntelligentPointer::g_setCount;
    group.FindNextGuardPatrolPosition();                       // no successor, no start: stay put, no assignment
    if (group.CurrentGuardpostThing.GetPItem() != &lone || CBaseIntelligentPointer::g_setCount != before) {
        std::printf("0093cab0_TEST FAIL\n"); return 1;
    }
    std::printf("0093cab0_TEST PASS\n");
    return 0;
}