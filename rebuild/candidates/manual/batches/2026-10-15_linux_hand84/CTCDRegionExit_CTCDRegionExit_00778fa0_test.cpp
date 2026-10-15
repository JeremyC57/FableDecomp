#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
static long g_pauseBehaviour = -1;
struct CThing { void SetCutscenePauseBehaviour(long behaviour) { g_pauseBehaviour = behaviour; note("P"); } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { note("I"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CTCDriverBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCDriverBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { note("D"); }
};
struct CTCDRegionExit : CTCDriverBase_Calls {
    bool Active, InLetterBoxMode, WaitingForConfirmation, WaitingForActivationOrPlayerToLeave, WaitingForPlayerToLeaveMessageArea;
    float Radius, MessageRadius; u64 EntranceConnectedToUID; bool ReversedOnMiniMap, HiddenOnMiniMap;
    CIntelligentPointer PEntranceConnectedTo;
    virtual void v00() { }
    CTCDRegionExit(CThing* thing)
        : CTCDriverBase_Calls(thing), Active(true), InLetterBoxMode(false), WaitingForConfirmation(false),
          WaitingForActivationOrPlayerToLeave(true), WaitingForPlayerToLeaveMessageArea(true), Radius(1.0f), MessageRadius(0.0f),
          EntranceConnectedToUID(0), ReversedOnMiniMap(false), HiddenOnMiniMap(false), PEntranceConnectedTo()
    { ParentThing->SetCutscenePauseBehaviour(2); }
};
int main()
{
    CThing thing;
    CTCDRegionExit* r = new CTCDRegionExit(&thing);
    if (std::strcmp(g_seq, "DIP") || g_pauseBehaviour != 2 || r->ParentThing != &thing || !r->Active || r->InLetterBoxMode || r->WaitingForConfirmation
        || !r->WaitingForActivationOrPlayerToLeave || !r->WaitingForPlayerToLeaveMessageArea || r->Radius != 1.0f || r->MessageRadius != 0.0f
        || r->EntranceConnectedToUID != 0 || r->ReversedOnMiniMap || r->HiddenOnMiniMap || r->PEntranceConnectedTo.m_pItem != 0)
    { std::printf("00778fa0_TEST FAIL\n"); return 1; }
    delete r;
    std::printf("00778fa0_TEST PASS\n");
    return 0;
}
