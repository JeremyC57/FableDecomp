// Behaviour test for 00d17da0: the model below is a copy of src/00d17da0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __thiscall
#define __forceinline inline
#define __int64 long long
#endif
struct CGameScriptInterface {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
};

namespace NScript {
struct CQ_SunnyvaleMasterData {
    unsigned char _pad000[0xda];
    bool          BountyHuntTimeLimitExceeded;          // +0xda
};

struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_BountyHuntScript : CScriptBase {
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool QuestFailed;                                   // +0x48
    bool QuestSucceeded;                                // +0x49
    bool QuestOver;                                     // +0x4a
    bool QuestStarted;                                  // +0x4b
    unsigned char _flags04c[0x2c];                      // +0x4c .. +0x78 (kill / intro / boss flags, grunt counter)
    bool BoastActiveTimeLimit;                          // +0x78
    bool BoastActiveDecapitation;                       // +0x79
    long BoastTimerID;                                  // +0x7c
    long BoastTime;                                     // +0x80 (CTimer)
    bool BoastQuestInfoDisplayed;                       // +0x84
    void WatchForQuestInfoDisplayConditions();
    void RemoveBoastQuestInfo_00d17e70();               // 0x00D17E70
    void AddBoastQuestInfo();                           // 0x00D17EA0
};

void CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions()
{
    if (!BoastActiveTimeLimit) {
        IsActiveThreadTerminating();
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (BoastQuestInfoDisplayed) {
            if (QuestSucceeded || QuestFailed || PMasterData->BountyHuntTimeLimitExceeded) {
                if (IsActiveThreadTerminating())
                    return;
                RemoveBoastQuestInfo_00d17e70();
                IsActiveThreadTerminating();
                return;
            }
        } else if (BoastActiveTimeLimit && !QuestSucceeded && !QuestFailed) {
            if (IsActiveThreadTerminating())
                return;
            while (!QuestStarted) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            AddBoastQuestInfo();
        }
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
}

// ---- test doubles and driver ----
static int g_frames = 0, g_termAt = 1000, g_startAt = 1000, g_succeedAt = 1000, g_limitAt = 1000;
static int g_adds = 0, g_removes = 0, g_termCalls = 0;
static NScript::CQ_BountyHuntScript* g_s = 0;
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_frames == g_startAt) g_s->QuestStarted = true;
    if (g_frames == g_succeedAt) g_s->QuestSucceeded = true;
    if (g_frames == g_limitAt) g_s->PMasterData->BountyHuntTimeLimitExceeded = true;
}
bool NScript::CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_frames >= g_termAt; }
void NScript::CQ_BountyHuntScript::AddBoastQuestInfo() { ++g_adds; BoastQuestInfoDisplayed = true; }
void NScript::CQ_BountyHuntScript::RemoveBoastQuestInfo_00d17e70() { ++g_removes; BoastQuestInfoDisplayed = false; }

static bool Run(bool active, int termAt, int startAt, int succeedAt, int limitAt, int expectFrames, int expectAdds, int expectRemoves)
{
    static CGameScriptInterface gsi;
    static NScript::CQ_SunnyvaleMasterData data;
    std::memset(&data, 0, sizeof(data));
    NScript::CQ_BountyHuntScript s;
    std::memset(&s.QuestFailed, 0, (char*)&s.BoastQuestInfoDisplayed + 1 - (char*)&s.QuestFailed);
    s.GSI = &gsi; s.PMasterData = &data; s.BoastActiveTimeLimit = active;
    g_s = &s; g_frames = 0; g_termAt = termAt; g_startAt = startAt; g_succeedAt = succeedAt; g_limitAt = limitAt;
    g_adds = g_removes = g_termCalls = 0;
    s.WatchForQuestInfoDisplayConditions();
    return g_frames == expectFrames && g_adds == expectAdds && g_removes == expectRemoves && g_termCalls > 0;
}

int main()
{
    bool ok = true;
    ok = ok && Run(false, 1000, 1000, 1000, 1000, 0, 0, 0);   // boast not taken: nothing to watch
    ok = ok && Run(true, 1000, 2, 5, 1000, 5, 1, 1);          // shown once the quest starts, removed on success
    ok = ok && Run(true, 1000, 1, 1000, 4, 4, 1, 1);          // removed when the time limit is exceeded
    ok = ok && Run(true, 3, 1000, 1000, 1000, 3, 0, 0);       // terminated while waiting for the quest start
    ok = ok && Run(true, 6, 2, 1000, 1000, 6, 1, 0);          // terminated while the info is shown
    if (!ok) { std::printf("00d17da0_TEST FAIL\n"); return 1; }
    std::printf("00d17da0_TEST PASS\n");
    return 0;
}
