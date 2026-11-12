// Behaviour test for CCreatureAction_UseLamp::Initialise (0x00847DF0): the same logic through a small standalone model.
#include <cstdio>
#include <cstring>
#include <string>

static unsigned long Crc(const char* data, unsigned long length)
{
    unsigned long crc = 0;
    for (unsigned long i = 0; i < length; ++i) {
        crc ^= (unsigned char)data[i];
        for (int b = 0; b < 8; ++b)
            crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
    }
    return crc;
}
class CAnimFilterOfGroup { public:
    std::string Group;
    unsigned long GroupCRC;
    CAnimFilterOfGroup(const std::string& group) { Group = group; GroupCRC = Crc(group.c_str(), (unsigned long)group.size()); }
};
class CTCActionUseBase { public: std::string AnimGroup; std::string GetActionUseAnimationGroupName() const { return AnimGroup; } };
class CThing { public:
    bool Dead;
    const CTCActionUseBase* ActionUse;       // TCI_ON_ACTION_USE component, 0 when absent
    bool IsDead() const { return Dead; }
    bool PeekTC(const CTCActionUseBase** out) const { if (ActionUse) { *out = ActionUse; return true; } return false; }
};
class CGameCameraManager { public:
    int ModesSet; const CThing* ModeCreature; const CThing* ModeLamp; long ModeDuration;
    void SetScriptedCameraMode(const CThing& creature, const CThing& lamp, long duration)
    { ++ModesSet; ModeCreature = &creature; ModeLamp = &lamp; ModeDuration = duration; }
};
class CThingCreatureBase : public CThing { public:
    bool PlayerCharacter;
    std::string AskedName, AskedGroup; unsigned long AskedCRC; int Token;
    const int& GetRandomAnimFromNameWithFilter(const std::string& name, const CAnimFilterOfGroup& filter)
    { AskedName = name; AskedGroup = filter.Group; AskedCRC = filter.GroupCRC; return Token; }
    bool IsPlayerCharacter() const { return PlayerCharacter; }
};
class CCreatureAction_UseLamp { public:
    CThingCreatureBase* PCreature; CThing* PObjectToUse; CGameCameraManager* Manager;   // Manager 0 = the camera has none
    long TotalTime; const int* Anim; float AnimA, AnimB; int BaseInitialised;
    void SetAnimation(const int& anim, float a, float b) { Anim = &anim; AnimA = a; AnimB = b; }
    bool GetCameraManager(CGameCameraManager** out) { if (Manager == 0) return false; *out = Manager; return true; }
    void Initialise()
    {
        std::string anim_group;
        if (PObjectToUse != 0 && !PObjectToUse->IsDead()) {
            const CTCActionUseBase* action_use;
            if (PObjectToUse->PeekTC(&action_use))
                anim_group = action_use->GetActionUseAnimationGroupName();
        }
        CAnimFilterOfGroup filter(anim_group);
        const int& anim = PCreature->GetRandomAnimFromNameWithFilter("LIGHT_LAMP", filter);
        SetAnimation(anim, -1.0f, 1.0f);
        if (PCreature->IsPlayerCharacter()) {
            CGameCameraManager* camera_manager;
            if (GetCameraManager(&camera_manager))
                camera_manager->SetScriptedCameraMode(*PCreature, *PObjectToUse, TotalTime);
        }
        ++BaseInitialised;
    }
};
static bool Run(bool haveLamp, bool dead, bool haveUse, bool player, bool haveManager)
{
    CTCActionUseBase use; use.AnimGroup = "LAMP_GROUP";
    CThing lamp; lamp.Dead = dead; lamp.ActionUse = haveUse ? &use : 0;
    CThingCreatureBase creature; creature.Dead = false; creature.ActionUse = 0; creature.PlayerCharacter = player; creature.Token = 7; creature.AskedCRC = 1;
    CGameCameraManager manager; manager.ModesSet = 0; manager.ModeCreature = 0; manager.ModeLamp = 0; manager.ModeDuration = 0;
    CCreatureAction_UseLamp action; action.PCreature = &creature; action.PObjectToUse = haveLamp ? &lamp : 0;
    action.Manager = haveManager ? &manager : 0; action.TotalTime = 123; action.Anim = 0; action.AnimA = action.AnimB = 0; action.BaseInitialised = 0;
    if (!haveLamp && player && haveManager) return true;   // retail dereferences the lamp there: not a reachable state
    action.Initialise();
    std::string expectGroup = (haveLamp && !dead && haveUse) ? "LAMP_GROUP" : "";
    if (creature.AskedName != "LIGHT_LAMP") return false;
    if (creature.AskedGroup != expectGroup) return false;
    if (creature.AskedCRC != Crc(expectGroup.c_str(), (unsigned long)expectGroup.size())) return false;
    if (action.Anim != &creature.Token || action.AnimA != -1.0f || action.AnimB != 1.0f) return false;
    if (action.BaseInitialised != 1) return false;
    int expectModes = (player && haveManager) ? 1 : 0;
    if (manager.ModesSet != expectModes) return false;
    if (expectModes && (manager.ModeCreature != &creature || manager.ModeLamp != &lamp || manager.ModeDuration != 123)) return false;
    return true;
}
int main()
{
    bool ok = true;
    for (int m = 0; m < 32; ++m)
        if (!Run((m & 1) != 0, (m & 2) != 0, (m & 4) != 0, (m & 8) != 0, (m & 16) != 0)) ok = false;
    if (Crc("", 0) != 0 || Crc("LAMP_GROUP", 10) == 0) ok = false;
    if (!ok) { std::printf("00847df0_TEST FAIL\n"); return 1; }
    std::printf("00847df0_TEST PASS\n");
    return 0;
}