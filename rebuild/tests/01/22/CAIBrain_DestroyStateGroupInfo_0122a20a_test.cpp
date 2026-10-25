// Behaviour test for 0x0122A20A the atexit destructor of CAIBrain::StateGroupInfo: every (state-group name,
// allocator) entry of the table is destroyed, last entry first
#include <stdio.h>
#include <string.h>
#if !defined(_MSC_VER)
#define __fastcall
#endif
static const char* gDestroyed[64];
static int gDtors = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long) : PStringData(text) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) {}
    ~CCharString() { if (gDtors < 64) gDestroyed[gDtors] = PStringData; ++gDtors; }
};
class CAIStateGroupBase { public: int Kind; };
static CAIStateGroupBase gWander = { 1 };
static CAIStateGroupBase gSummon = { 2 };
static CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_WANDER() { return &gWander; }
static CAIStateGroupBase* __fastcall AllocStateGroup_SG_SUMMONER_CHARGE() { return &gSummon; }
class CAIStateGroupInfo {
public:
    CCharString StateGroupName;
    CAIStateGroupBase* (__fastcall* AllocFunc)();
    CAIStateGroupInfo(CCharString name, CAIStateGroupBase* (__fastcall* alloc)()) : StateGroupName(name), AllocFunc(alloc) {}
    ~CAIStateGroupInfo();
};
CAIStateGroupInfo::~CAIStateGroupInfo() {}            // user-declared and out of line, as in the PDB
int main()
{
    bool ok = true;
    int mark = 0;
    {
        CAIStateGroupInfo table[3] = {
            CAIStateGroupInfo(CCharString("SG_MINION_WANDER", -1), AllocStateGroup_SG_MINION_WANDER),
            CAIStateGroupInfo(CCharString("SG_SUMMONER_CHARGE", -1), AllocStateGroup_SG_SUMMONER_CHARGE),
            CAIStateGroupInfo(CCharString("", -1), 0),
        };
        mark = gDtors;                                    // temporaries are gone; the table is live
        ok = ok && table[0].AllocFunc()->Kind == 1 && table[1].AllocFunc()->Kind == 2 && table[2].AllocFunc == 0;
        ok = ok && strcmp(table[1].StateGroupName.PStringData, "SG_SUMMONER_CHARGE") == 0;
    }
    ok = ok && gDtors - mark == 3;                        // each entry's name destroyed once
    ok = ok && mark + 2 < 64;
    ok = ok && strcmp(gDestroyed[mark], "") == 0;         // last entry first
    ok = ok && strcmp(gDestroyed[mark + 1], "SG_SUMMONER_CHARGE") == 0;
    ok = ok && strcmp(gDestroyed[mark + 2], "SG_MINION_WANDER") == 0;
    if (ok) { printf("0122a20a_TEST PASS\n"); return 0; }
    printf("0122a20a_TEST FAIL\n");
    return 1;
}