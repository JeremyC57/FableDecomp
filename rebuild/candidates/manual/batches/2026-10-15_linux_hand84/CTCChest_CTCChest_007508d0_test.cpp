#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
static long g_pauseBehaviour = -1;
struct CThing { void SetCutscenePauseBehaviour(long behaviour) { g_pauseBehaviour = behaviour; note("P"); } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { note("I"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CTCContainerRewardHero_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; unsigned char _pad0a[2];
    void* ContainerItems[3]; bool Enabled; bool CreateObjectsThatTimeOut;
    CTCContainerRewardHero_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false), Enabled(true), CreateObjectsThatTimeOut(true)
    { ContainerItems[0] = ContainerItems[1] = ContainerItems[2] = 0; note("B"); }
};
struct COpenCloseHelper {
    bool Opened, Closed, Opening, Closing;
    float OpenCloseTransitionAmount, LastOpenCloseTransitionAmount, TotalTimeToOpen, TotalTimeToClose;
    COpenCloseHelper() { Reset(); TotalTimeToOpen = 1.0f; TotalTimeToClose = 1.0f; }
    void Reset() { Opened = false; Closed = true; OpenCloseTransitionAmount = LastOpenCloseTransitionAmount = 0.0f; Opening = false; Closing = false; }
};
struct CTCChest : CTCContainerRewardHero_Calls {
    COpenCloseHelper ChestOpener; bool ChestOpen; CIntelligentPointer POpenParticle;
    virtual void v00() { }
    CTCChest(CThing* thing) : CTCContainerRewardHero_Calls(thing), ChestOpener(), ChestOpen(false), POpenParticle()
    { ParentThing->SetCutscenePauseBehaviour(2); }
};
int main()
{
    CThing thing;
    CTCChest* c = new CTCChest(&thing);
    const COpenCloseHelper& h = c->ChestOpener;
    if (std::strcmp(g_seq, "BIP") || g_pauseBehaviour != 2 || c->ParentThing != &thing || c->ChestOpen || c->POpenParticle.m_pItem != 0
        || h.Opened || !h.Closed || h.Opening || h.Closing || h.OpenCloseTransitionAmount != 0.0f || h.LastOpenCloseTransitionAmount != 0.0f
        || h.TotalTimeToOpen != 1.0f || h.TotalTimeToClose != 1.0f) { std::printf("007508d0_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("007508d0_TEST PASS\n");
    return 0;
}
