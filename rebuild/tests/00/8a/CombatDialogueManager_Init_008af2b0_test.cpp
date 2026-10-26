// Behaviour test (0x008af2b0): CombatDialogueManager::Init fetches COMBAT_DIALOGUE_DEF_INSTANCE and registers the def's 17 speech groups (EnemySighted .. Flee) with NextAllowedFrame 0; an already-present group keeps its entry
#include <cstdio>
#include <cstring>
class CCombatDialogueDef { public:
    int EnemySighted, RunForCover, Hiding, Ambushing, WillSeenUsed, Threat, Encourage, FoeKnockedDownByColleague, MenLosing,
        EnemyKnockedDown, RangedCombatAiming, RangedCombatAimingBlocked, Injured, ReactionToDecapitation, ColleagueInjured, Scared, Flee;
};
static CCombatDialogueDef g_def;
static char g_requested[64];
class CGameDefinitionManager { public:
    bool GetDef(const char* name, const CCombatDialogueDef** out) { std::strcpy(g_requested, name); *out = &g_def; return true; }
};
static CGameDefinitionManager g_manager;
struct TSpeechGroupInfo { int NextAllowedFrame; TSpeechGroupInfo() : NextAllowedFrame(0) {} };
struct CSpeechGroupInfoMap {
    unsigned int Keys[32]; TSpeechGroupInfo Values[32]; int Size;
    bool insert(unsigned int key, const TSpeechGroupInfo& v) { for (int i = 0; i < Size; ++i) if (Keys[i] == key) return false; Keys[Size] = key; Values[Size] = v; ++Size; return true; }
    int Find(unsigned int key) const { for (int i = 0; i < Size; ++i) if (Keys[i] == key) return i; return -1; }
};
class CombatDialogueManager { public:
    CSpeechGroupInfoMap SpeechGroupInfoMap; const CCombatDialogueDef* PDialogueDef;
    void AddSpeechGroup(unsigned int group) { SpeechGroupInfoMap.insert(group, TSpeechGroupInfo()); }
    void Init()
    {
        g_manager.GetDef("COMBAT_DIALOGUE_DEF_INSTANCE", &PDialogueDef);
        AddSpeechGroup(PDialogueDef->EnemySighted); AddSpeechGroup(PDialogueDef->RunForCover); AddSpeechGroup(PDialogueDef->Hiding);
        AddSpeechGroup(PDialogueDef->Ambushing); AddSpeechGroup(PDialogueDef->WillSeenUsed); AddSpeechGroup(PDialogueDef->Threat);
        AddSpeechGroup(PDialogueDef->Encourage); AddSpeechGroup(PDialogueDef->FoeKnockedDownByColleague); AddSpeechGroup(PDialogueDef->MenLosing);
        AddSpeechGroup(PDialogueDef->EnemyKnockedDown); AddSpeechGroup(PDialogueDef->RangedCombatAiming); AddSpeechGroup(PDialogueDef->RangedCombatAimingBlocked);
        AddSpeechGroup(PDialogueDef->Injured); AddSpeechGroup(PDialogueDef->ReactionToDecapitation); AddSpeechGroup(PDialogueDef->ColleagueInjured);
        AddSpeechGroup(PDialogueDef->Scared); AddSpeechGroup(PDialogueDef->Flee);
    }
};
int main()
{
    int* fields = &g_def.EnemySighted;
    for (int i = 0; i < 17; ++i) fields[i] = 100 + i;
    g_def.Scared = 100;                                   // shares a group with EnemySighted: inserted once
    CombatDialogueManager m; m.SpeechGroupInfoMap.Size = 0; m.PDialogueDef = 0;
    m.Init();
    if (std::strcmp(g_requested, "COMBAT_DIALOGUE_DEF_INSTANCE") != 0 || m.PDialogueDef != &g_def || m.SpeechGroupInfoMap.Size != 16) { std::printf("008af2b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 17; ++i) {
        int at = m.SpeechGroupInfoMap.Find((unsigned int)fields[i]);
        if (at < 0 || m.SpeechGroupInfoMap.Values[at].NextAllowedFrame != 0) { std::printf("008af2b0_TEST FAIL\n"); return 1; }
    }
    std::printf("008af2b0_TEST PASS\n");
    return 0;
}