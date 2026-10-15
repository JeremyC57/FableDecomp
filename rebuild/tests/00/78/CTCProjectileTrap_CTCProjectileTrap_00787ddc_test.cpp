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
struct CTCTrapBase : CTCBase { bool Active; CTCTrapBase(CThing* thing) : CTCBase(thing), Active(true) { } };
struct CTCProjectileTrap : CTCTrapBase {
    long ShotTimer, IdleTimer, NumShotsThisRound;
    CIntelligentPointer PTarget;
    CTCProjectileTrap(CThing* thing) : CTCTrapBase(thing), ShotTimer(-1), IdleTimer(-1), NumShotsThisRound(0), PTarget() { }
};
int main()
{
    CThing t; t.id = 1;
    CTCProjectileTrap p(&t);
    if (p.ParentThing != &t || !p.Active || p.ShotTimer != -1 || p.IdleTimer != -1 || p.NumShotsThisRound != 0 || p.PTarget.PData != 0) { std::printf("00787ddc_TEST FAIL\n"); return 1; }
    std::printf("00787ddc_TEST PASS\n");
    return 0;
}