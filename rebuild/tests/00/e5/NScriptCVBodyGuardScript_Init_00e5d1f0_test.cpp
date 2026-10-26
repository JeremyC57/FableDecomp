// Behaviour test for CV_BodyGuardScript::Init (0x00E5D1F0).
#include <cstdio>

class CGameScriptInterface { public:
    long m_timer, m_time;
    void SetTimer(long timer, long time) { m_timer = timer; m_time = time; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

struct CScriptOwner { long m_value08; };
class CTimer { public: long TimerIndex; };

struct CV_BodyGuardScript {
    CScriptOwner* Owner;
    long TotalWages;
    bool HeroCannotAffordHelp, HeroWieldingWeapon;
    CTimer CommentaryTimer;
    long ConversationIndex;
    long HenchmanGlobalStatus[4];
    void Init();
};

void CV_BodyGuardScript::Init()
{
    ConversationIndex = 0;
    TotalWages = 0;
    HeroCannotAffordHelp = false;
    Owner->m_value08 = 0;
    g_pGameScriptInterface->SetTimer(CommentaryTimer.TimerIndex, 0);
    HeroWieldingWeapon = false;
    HenchmanGlobalStatus[0] = 0;
    HenchmanGlobalStatus[1] = 0;
    HenchmanGlobalStatus[2] = 0;
    HenchmanGlobalStatus[3] = 0;
}

int main()
{
    CGameScriptInterface gsi; gsi.m_timer = -1; gsi.m_time = -1; g_pGameScriptInterface = &gsi;
    CScriptOwner owner; owner.m_value08 = 9;
    CV_BodyGuardScript s; s.Owner = &owner; s.TotalWages = 50; s.HeroCannotAffordHelp = s.HeroWieldingWeapon = true;
    s.CommentaryTimer.TimerIndex = 6; s.ConversationIndex = 3;
    for (int i = 0; i < 4; ++i) s.HenchmanGlobalStatus[i] = i + 1;
    s.Init();
    if (owner.m_value08 != 0 || s.TotalWages != 0 || s.HeroCannotAffordHelp || s.HeroWieldingWeapon || s.ConversationIndex != 0
        || gsi.m_timer != 6 || gsi.m_time != 0 || s.HenchmanGlobalStatus[0] || s.HenchmanGlobalStatus[3]) {
        std::printf("00e5d1f0_TEST FAIL\n");
        return 1;
    }
    std::printf("00e5d1f0_TEST PASS\n");
    return 0;
}