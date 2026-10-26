// Behaviour test for CV_ExposeMayorScript::Init (0x00E75680).
#include <cstdio>

class CCharString { public:
    const char* m_text;
    CCharString& operator=(const CCharString& o) { m_text = o.m_text; return *this; }
};

class CScriptDef { public: CCharString ExposeMayorEvidence; float ExposeMayorMaxFlashDistFromStables; };
CScriptDef* g_pScriptDef = 0;

struct CV_ExposeMayorScript {
    long Path, StageNumber;
    bool HeroFlashedLightsCorrectly, PrisonerTalkedAboutLamp, ResetLamp, SaidYesToBecomeMayor, QuestCardGiven, MayorExposeGhostCutscenePlayed;
    CCharString Evidence;
    float MaxFlashDistFromStables;
    void Init();
};

void CV_ExposeMayorScript::Init()
{
    Path = 0;
    StageNumber = 0;
    Evidence = g_pScriptDef->ExposeMayorEvidence;
    HeroFlashedLightsCorrectly = false;
    PrisonerTalkedAboutLamp = false;
    ResetLamp = false;
    SaidYesToBecomeMayor = false;
    QuestCardGiven = false;
    MayorExposeGhostCutscenePlayed = false;
    MaxFlashDistFromStables = g_pScriptDef->ExposeMayorMaxFlashDistFromStables;
}

int main()
{
    CScriptDef def; def.ExposeMayorEvidence.m_text = "OBJECT_EVIDENCE"; def.ExposeMayorMaxFlashDistFromStables = 12.5f;
    g_pScriptDef = &def;
    CV_ExposeMayorScript s;
    s.Path = 3; s.StageNumber = 4; s.HeroFlashedLightsCorrectly = s.PrisonerTalkedAboutLamp = s.ResetLamp = true;
    s.SaidYesToBecomeMayor = s.QuestCardGiven = s.MayorExposeGhostCutscenePlayed = true; s.Evidence.m_text = 0; s.MaxFlashDistFromStables = 0.0f;
    s.Init();
    if (s.Path != 0 || s.StageNumber != 0 || s.HeroFlashedLightsCorrectly || s.PrisonerTalkedAboutLamp || s.ResetLamp
        || s.SaidYesToBecomeMayor || s.QuestCardGiven || s.MayorExposeGhostCutscenePlayed
        || s.Evidence.m_text != def.ExposeMayorEvidence.m_text || s.MaxFlashDistFromStables != 12.5f) {
        std::printf("00e75680_TEST FAIL\n");
        return 1;
    }
    std::printf("00e75680_TEST PASS\n");
    return 0;
}