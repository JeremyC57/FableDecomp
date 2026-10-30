// NScript::CV_TempleOfLightScript::OnPersist(CPersistContext& context) -- save/load the Temple of Light quest: the donation counters and flags, the weapon / intro / temple / attack flags, the three morality values, the quest finished / activated / complete flags and the reward flags, each under its member name
// Leaf: ?OnPersist@CV_TempleOfLightScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDAF30, 594 bytes, thiscall, `ret 4`.
// Identity (proven): the row is referenced once, from slot 4 (OnPersist, 0x012F3C78) of the RTTI-less level-script vtable
//   0x012F3C68 [dtor 0x00EDEB80, RegisterMain 0x00EDAE70 (landed NScript::CV_TempleOfLightScript::RegisterMain), Main 0x00EDB190,
//   Init 0x00EDAE20 (landed CV_TempleOfLightScript::Init), OnPersist 0x00EDAF30]; Ego_r.pdb
//   ?OnPersist@CV_TempleOfLightScript@NScript@@UAEXAAVCPersistContext@@@Z. Every key string is the Ego_r.pdb member at that
//   retail offset + 0x14. Catalog label sub_00edaf30.
// Callees: CPersistContext::Transfer<long> 0x00410BE0, Transfer<bool> 0x004045C0, Transfer<float> 0x00410620.
// Strings (paired by hand with the members): "DonationTotal" 0x012E8258 (+0x48), "DonationThisVisit" 0x012E8244 (+0x4c),
//   "DonationCounter" 0x012E8234 (+0x50), "DonationInvalid" 0x012E8224 (+0x54), "DonationActive" 0x012F3CD0 (+0x55),
//   "DisplayQuestInfo" 0x012E8210 (+0x56), "WeaponWarningComplete" 0x012E81F8 (+0x57), "WeaponUnsheathed" 0x012E81E4 (+0x58),
//   "TempleIntroComplete" 0x012F3CBC (+0x59), "InsideTemple" 0x012F3CAC (+0x5a), "AttackedByHero" 0x012E58B0 (+0x5b),
//   "AcolyteAttackMessageComplete" 0x012E81A0 (+0x5c), "AllAcolytesEscaped" 0x012F3C98 (+0x5d), "MoralityInitial" 0x012E8190
//   (+0x64), "MoralityCurrent" 0x012E8180 (+0x68), "MoralityCumulative" 0x012E816C (+0x6c), "QuestFinished" 0x012C6FBC (+0x70),
//   "QuestActivated" 0x012E2778 (+0x72), "QuestComplete" 0x012E2788 (+0x71), "ReceivedReward" 0x012E815C (+0x5e),
//   "GivenCoolThing1" 0x012E814C (+0x5f), "GivenCoolThing2" 0x012E813C (+0x60), "AwardWeapon" 0x012E8130 (+0x61).
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT, CScriptBase 0x40, CLevelScriptBase 0x48); CV_TempleOfLightScript
//   members at Ego_r.pdb - 0x14 (no containers before them; Ego_r size 0x88 -> retail 0x74).
// Shape: the landed CV_BeardyBaldyScript::OnPersist 0x00E4F7F0 model: one Transfer per member in retail's key order, every
//   default a temporary (the zero is kept in ebx).
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TempleOfLightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long DonationTotal;                                 // +0x48
    long DonationThisVisit;                             // +0x4c
    long DonationCounter;                               // +0x50
    bool DonationInvalid;                               // +0x54
    bool DonationActive;                                // +0x55
    bool DisplayQuestInfo;                              // +0x56
    bool WeaponWarningComplete;                         // +0x57
    bool WeaponUnsheathed;                              // +0x58
    bool TempleIntroComplete;                           // +0x59
    bool InsideTemple;                                  // +0x5a
    bool AttackedByHero;                                // +0x5b
    bool AcolyteAttackMessageComplete;                  // +0x5c
    bool AllAcolytesEscaped;                            // +0x5d
    bool ReceivedReward;                                // +0x5e
    bool GivenCoolThing1;                               // +0x5f
    bool GivenCoolThing2;                               // +0x60
    bool AwardWeapon;                                   // +0x61
    float MoralityInitial;                              // +0x64
    float MoralityCurrent;                              // +0x68
    float MoralityCumulative;                           // +0x6c
    bool QuestFinished;                                 // +0x70
    bool QuestComplete;                                 // +0x71
    bool QuestActivated;                                // +0x72
    virtual void OnPersist(CPersistContext& context);
};

void CV_TempleOfLightScript::OnPersist(CPersistContext& context)
{
    context.Transfer("DonationTotal", DonationTotal, 0L);
    context.Transfer("DonationThisVisit", DonationThisVisit, 0L);
    context.Transfer("DonationCounter", DonationCounter, 0L);
    context.Transfer("DonationInvalid", DonationInvalid, false);
    context.Transfer("DonationActive", DonationActive, false);
    context.Transfer("DisplayQuestInfo", DisplayQuestInfo, false);
    context.Transfer("WeaponWarningComplete", WeaponWarningComplete, false);
    context.Transfer("WeaponUnsheathed", WeaponUnsheathed, false);
    context.Transfer("TempleIntroComplete", TempleIntroComplete, false);
    context.Transfer("InsideTemple", InsideTemple, false);
    context.Transfer("AttackedByHero", AttackedByHero, false);
    context.Transfer("AcolyteAttackMessageComplete", AcolyteAttackMessageComplete, false);
    context.Transfer("AllAcolytesEscaped", AllAcolytesEscaped, false);
    context.Transfer("MoralityInitial", MoralityInitial, 0.0f);
    context.Transfer("MoralityCurrent", MoralityCurrent, 0.0f);
    context.Transfer("MoralityCumulative", MoralityCumulative, 0.0f);
    context.Transfer("QuestFinished", QuestFinished, false);
    context.Transfer("QuestActivated", QuestActivated, false);
    context.Transfer("QuestComplete", QuestComplete, false);
    context.Transfer("ReceivedReward", ReceivedReward, false);
    context.Transfer("GivenCoolThing1", GivenCoolThing1, false);
    context.Transfer("GivenCoolThing2", GivenCoolThing2, false);
    context.Transfer("AwardWeapon", AwardWeapon, false);
}
}   // namespace NScript