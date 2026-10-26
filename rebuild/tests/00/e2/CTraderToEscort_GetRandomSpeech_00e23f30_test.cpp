#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CTraderToEscort::GetRandomSpeech() -- while the escorted trader follows (quest stage 2) or waits (stage 3), pick one of the 10 follow / 5 wait speech lines at random, never either of the last two picks of that kind, yielding a script frame per rejected pick (or other stage); the empty string when the script is done
// Leaf: GetRandomSpeech
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E23F30, length 347 bytes (`ret 4`, CCharString by hidden pointer).
// Callees: CCharString default ctor @ 0x0099E4B0, copy ctor @ 0x0099EC30, operator= @ 0x0099EFB0, dtor @ 0x0099EAE0,
//   rand @ 0x00BFEB16, the entity script's IsDone @ 0x00F35B30 (manifest label "CCreatureAction_TrollWhackGroundBase::
//   Initialise" is propagated), the game script interface (+4) slot 7 (+0x1c, one script frame).
// Layout: Ego_r.pdb CTraderToEscort (44 bytes): ParentClass +0x14 (CQR_EscortTraderScript*), PMasterData +0x18,
//   lastRandomSpeechFollowIdx1/2 +0x1c/+0x20, lastRandomSpeechWaitIdx1/2 +0x24/+0x28 (as the PDB).
//   CQR_EscortTraderScript (retail, read here): RandomSpeechFollow[10] +0x88, RandomSpeechWait[5] +0xb0, QuestStage +0x11c
//   (Ego_r.pdb +0x9c / +0xc4 / +0x148: the record's earlier members are 0x14 smaller in retail and each of the six
//   std::vector<CCharString> route members is 12 bytes, not 16).
// Shape: the landed CAC_Owner::GetRandomSpeech (0x00E2E440) with two stages: an empty string, a copy of it returned on every
//   IsDone exit, and the chosen line assigned to a third local and copied out (no NRVO).
class CCharString
{ public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)

class CGameScriptInterface
{ public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual void NewScriptFrame();                  // slot 7 (+0x1c): yield one frame
};

enum EscortTraderQuestStage { ESCORT_STAGE_FOLLOW = 2, ESCORT_STAGE_WAIT = 3 };

struct CQR_EscortTraderScript                       // retail layout of the members read here
{
    unsigned char          _pad00[0x88];
    CCharString            RandomSpeechFollow[10];  // +0x88
    CCharString            RandomSpeechWait[5];     // +0xb0
    unsigned char          _padc4[0x11c - 0xc4];
    EscortTraderQuestStage QuestStage;              // +0x11c
};

struct CTraderToEscort
{
    virtual void v00();
    CGameScriptInterface*   GSI;                    // +0x04
    unsigned char           _pad08[0x14 - 0x08];    // Me (CScriptThing)
    CQR_EscortTraderScript* ParentClass;            // +0x14
    void*                   PMasterData;            // +0x18
    long lastRandomSpeechFollowIdx1;                // +0x1c
    long lastRandomSpeechFollowIdx2;                // +0x20
    long lastRandomSpeechWaitIdx1;                  // +0x24
    long lastRandomSpeechWaitIdx2;                  // +0x28
    bool IsDone();                                  // 0x00F35B30
    CCharString GetRandomSpeech();
};

CCharString CTraderToEscort::GetRandomSpeech()
{
    CCharString empty;
    CCharString none(empty);
    CCharString speech;
    long index;
    for (;;)
    {
        if (IsDone())
            return none;
        if (ParentClass->QuestStage == ESCORT_STAGE_FOLLOW)
        {
            if (IsDone())
                return none;
            index = rand() % 10;
            if (index != lastRandomSpeechFollowIdx1 && index != lastRandomSpeechFollowIdx2)
            {
                if (IsDone())
                    return none;
                lastRandomSpeechFollowIdx2 = lastRandomSpeechFollowIdx1;
                lastRandomSpeechFollowIdx1 = index;
                speech = ParentClass->RandomSpeechFollow[index];
                break;
            }
        }
        else if (ParentClass->QuestStage == ESCORT_STAGE_WAIT)
        {
            if (IsDone())
                return none;
            index = rand() % 5;
            if (index != lastRandomSpeechWaitIdx1 && index != lastRandomSpeechWaitIdx2)
            {
                if (IsDone())
                    return none;
                lastRandomSpeechWaitIdx2 = lastRandomSpeechWaitIdx1;
                lastRandomSpeechWaitIdx1 = index;
                speech = ParentClass->RandomSpeechWait[index];
                break;
            }
        }
        GSI->NewScriptFrame();
    }
    if (IsDone())
        return none;
    return speech;
}


static int g_live = 0;
CCharString::CCharString() : PStringData((void*)"") { ++g_live; }
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const CCharString& o) { PStringData = o.PStringData; return *this; }
static int g_rolls[16]; static int g_nroll = 0;
extern "C" int __cdecl rand() { return g_rolls[g_nroll++]; }
static int g_frames = 0;
void CGameScriptInterface::v00() {} void CGameScriptInterface::v01() {} void CGameScriptInterface::v02() {}
void CGameScriptInterface::v03() {} void CGameScriptInterface::v04() {} void CGameScriptInterface::v05() {}
void CGameScriptInterface::v06() {}
void CGameScriptInterface::NewScriptFrame() { ++g_frames; }
void CTraderToEscort::v00() {}
static int g_doneAfter = 1000; static int g_doneCalls = 0;
bool CTraderToEscort::IsDone() { return ++g_doneCalls > g_doneAfter; }
static const char* Text(const CCharString& s) { return (const char*)s.PStringData; }
#define CHECK(c) if (!(c)) { std::printf("00e23f30_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static const char* follow[10] = { "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9" };
    static const char* wait[5] = { "w0", "w1", "w2", "w3", "w4" };
    static CQR_EscortTraderScript rec;
    for (int i = 0; i < 10; ++i) rec.RandomSpeechFollow[i].PStringData = (void*)follow[i];
    for (int i = 0; i < 5; ++i) rec.RandomSpeechWait[i].PStringData = (void*)wait[i];
    const int base = g_live;
    static CGameScriptInterface gsi;
    static CTraderToEscort t;
    t.GSI = &gsi; t.ParentClass = &rec;
    t.lastRandomSpeechFollowIdx1 = 3; t.lastRandomSpeechFollowIdx2 = 4;
    t.lastRandomSpeechWaitIdx1 = 1; t.lastRandomSpeechWaitIdx2 = 2;
    rec.QuestStage = ESCORT_STAGE_FOLLOW;
    g_rolls[0] = 13; g_rolls[1] = 24; g_rolls[2] = 17;     // 3 and 4 are rejected, 7 taken
    {
        CCharString s = t.GetRandomSpeech();
        CHECK(std::strcmp(Text(s), "f7") == 0 && g_frames == 2 && g_nroll == 3);
        CHECK(t.lastRandomSpeechFollowIdx1 == 7 && t.lastRandomSpeechFollowIdx2 == 3 && t.lastRandomSpeechWaitIdx1 == 1);
    }
    CHECK(g_live == base);
    rec.QuestStage = ESCORT_STAGE_WAIT; g_nroll = 0; g_frames = 0;
    g_rolls[0] = 6; g_rolls[1] = 9;                         // 1 rejected, 4 taken
    {
        CCharString s = t.GetRandomSpeech();
        CHECK(std::strcmp(Text(s), "w4") == 0 && g_frames == 1 && t.lastRandomSpeechWaitIdx1 == 4 && t.lastRandomSpeechWaitIdx2 == 1);
    }
    rec.QuestStage = (EscortTraderQuestStage)5; g_nroll = 0; g_frames = 0; g_doneCalls = 0; g_doneAfter = 4;
    {
        CCharString s = t.GetRandomSpeech();                // another stage: yields until done
        CHECK(std::strcmp(Text(s), "") == 0 && g_frames == 4 && g_nroll == 0);
    }
    rec.QuestStage = ESCORT_STAGE_FOLLOW; g_nroll = 0; g_rolls[0] = 1; g_doneCalls = 0; g_doneAfter = 3;
    {
        CCharString s = t.GetRandomSpeech();                // done just before the pick is copied out
        CHECK(std::strcmp(Text(s), "") == 0 && t.lastRandomSpeechFollowIdx1 == 1 && t.lastRandomSpeechFollowIdx2 == 7);
    }
    g_doneCalls = 0; g_doneAfter = 0;
    {
        CCharString s = t.GetRandomSpeech();
        CHECK(std::strcmp(Text(s), "") == 0 && t.lastRandomSpeechFollowIdx1 == 1);
    }
    CHECK(g_live == base);
    std::printf("00e23f30_TEST PASS\n");
    return 0;
}