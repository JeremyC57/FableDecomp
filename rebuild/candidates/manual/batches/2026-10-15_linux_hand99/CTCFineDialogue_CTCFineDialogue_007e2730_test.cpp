#include <cstdio>
struct CThing { int id; };
static int g_globalAdds = 0;
struct CTCBase {
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
    void AddToGlobalUpdateTCs() { if (!IsInGlobalUpdateTCs) { IsInGlobalUpdateTCs = true; ++g_globalAdds; } }
};
struct CIntelligentPointer {
    void* PData;
    CIntelligentPointer() : PData(0) { }
    virtual ~CIntelligentPointer() { }
};
struct CTCVillage;
struct CTCFineDialogue : CTCBase {
    long Fine, Money; bool GivePlayerChanceToPayFine, WaitingForConfirmation; CTCVillage* Village;
    CTCFineDialogue(CThing* thing) : CTCBase(thing), GivePlayerChanceToPayFine(true), WaitingForConfirmation(false) { AddToGlobalUpdateTCs(); }
};
int main()
{
    CThing t; t.id = 2;
    CTCFineDialogue d(&t);
    if (d.ParentThing != &t || !d.GivePlayerChanceToPayFine || d.WaitingForConfirmation || !d.IsInGlobalUpdateTCs || g_globalAdds != 1) { std::printf("007e2730_TEST FAIL\n"); return 1; }
    std::printf("007e2730_TEST PASS\n");
    return 0;
}
