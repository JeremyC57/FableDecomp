#include <cstdio>

struct CThing { int id; };
struct CTCBase {
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    virtual int Kind() const { return 0; }
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
};
struct CIntelligentPointer_CThingAICreature : CBaseIntelligentPointer {
    virtual int Kind() const { return 2; }
};
struct CTCGossip : CTCBase {
    CIntelligentPointer_CThingAICreature Gossipee;
    bool                                 MyTurnToSpeak;
    CTCGossip(CThing* owner) : CTCBase(owner), Gossipee(), MyTurnToSpeak(false) { }
};

int main()
{
    CThing owner = { 1 };
    CTCGossip g(&owner);
    if (g.ParentThing != &owner || g.Gossipee.m_pItem != 0 || g.Gossipee.Kind() != 2 || g.MyTurnToSpeak)
        { std::printf("004d8c27_TEST FAIL\n"); return 1; }
    std::printf("004d8c27_TEST PASS\n");
    return 0;
}
