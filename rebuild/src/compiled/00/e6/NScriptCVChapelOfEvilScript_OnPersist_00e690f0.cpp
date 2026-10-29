// NScript::CV_ChapelOfEvilScript::OnPersist(CPersistContext& context) -- save/load the Chapel of Evil quest script's state: donation counters and flags, the three morality tallies, quest flags, the torture count and the victim's state, each under its own member name with a zero default
// Leaf: ?OnPersist@CV_ChapelOfEvilScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E690F0, 759 bytes, thiscall, `ret 4`.
// Identity: slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012E8080 that the CV_ChapelOfEvilScript ctor
//   0x00E68F70 stores (slot 0 dtor 0x00E6E280, slot 1 the landed RegisterMain 0x00E69030, slot 2 Main 0x00E693F0, slot 3
//   the landed Init 0x00E68FA0); Ego_r.pdb ?OnPersist@CV_ChapelOfEvilScript@NScript@@UAEXAAVCPersistContext@@@Z. Every
//   persisted name string is the Ego_r.pdb member at that offset + 0x14. Catalog label: sub_ (gap scan).
// Callees: CPersistContext::Transfer<long> 0x00410BE0, Transfer<bool> 0x004045C0, Transfer<float> 0x00410620 (name,
//   value, const default; the landed CV_AmbushScamScript::OnPersist 0x00E2AD50 binds the same three).
// Strings: "DonationTotal" 0x012E8258 .. "VictimIsTrader" 0x012E80B0 (shared "AttackedByHero" 0x012E58B0, "QuestFinished"
//   0x012C6FBC, "QuestActivated" 0x012E2778, "QuestComplete" 0x012E2788) -- each paired with retail by hand.
// Layout: the landed CV_ChapelOfEvilScript::Init 0x00E68FA0 model (Ego_r.pdb - 0x14). VictimGender, TortureSound and
//   TortureType are not persisted.
// Shape: one Transfer call per member in retail's order; each default is a temporary built in the dead argument slot.
class CCharString { public: void* PData; };
enum ESex { };
enum ChapelOfEvilTortureType { };
class CGameScriptInterface;
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>, 0x004045C0 <bool>, 0x00410620 <float>
};
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChapelOfEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long DonationTotal;                                 // +0x48
    long DonationThisVisit;                             // +0x4c
    long DonationCounter;                               // +0x50
    bool DonationInvalid;                               // +0x54
    bool DisplayQuestInfo;                              // +0x55
    bool WeaponWarningComplete;                         // +0x56
    bool WeaponUnsheathed;                              // +0x57
    bool ChapelIntroComplete;                           // +0x58
    bool InsideChapel;                                  // +0x59
    bool AttackedByHero;                                // +0x5a
    bool AcolyteAttackMessageComplete;                  // +0x5b
    bool ReceivedReward;                                // +0x5c
    bool GivenCoolThing1;                               // +0x5d
    bool GivenCoolThing2;                               // +0x5e
    bool AwardWeapon;                                   // +0x5f
    float MoralityInitial;                              // +0x60
    float MoralityCurrent;                              // +0x64
    float MoralityCumulative;                           // +0x68
    bool QuestFinished;                                 // +0x6c
    bool QuestComplete;                                 // +0x6d
    bool QuestActivated;                                // +0x6e
    long TortureCount;                                  // +0x70
    bool VictimTortured;                                // +0x74
    long VictimHealth;                                  // +0x78
    ESex VictimGender;                                  // +0x7c
    bool VictimNightTime;                               // +0x80
    bool VictimSunday;                                  // +0x81
    bool VictimIsGuard;                                 // +0x82
    bool VictimIsBandit;                                // +0x83
    bool VictimIsTrader;                                // +0x84
    CCharString TortureSound;                           // +0x88
    ChapelOfEvilTortureType TortureType;                // +0x8c
    virtual void OnPersist(CPersistContext& context);
};

void CV_ChapelOfEvilScript::OnPersist(CPersistContext& context)
{
    context.Transfer("DonationTotal", DonationTotal, 0L);
    context.Transfer("DonationThisVisit", DonationThisVisit, 0L);
    context.Transfer("DonationCounter", DonationCounter, 0L);
    context.Transfer("DonationInvalid", DonationInvalid, false);
    context.Transfer("DisplayQuestInfo", DisplayQuestInfo, false);
    context.Transfer("WeaponWarningComplete", WeaponWarningComplete, false);
    context.Transfer("WeaponUnsheathed", WeaponUnsheathed, false);
    context.Transfer("ChapelIntroComplete", ChapelIntroComplete, false);
    context.Transfer("InsideChapel", InsideChapel, false);
    context.Transfer("AttackedByHero", AttackedByHero, false);
    context.Transfer("AcolyteAttackMessageComplete", AcolyteAttackMessageComplete, false);
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
    context.Transfer("TortureCount", TortureCount, 0L);
    context.Transfer("VictimTortured", VictimTortured, false);
    context.Transfer("VictimHealth", VictimHealth, 0L);
    context.Transfer("VictimNightTime", VictimNightTime, false);
    context.Transfer("VictimSunday", VictimSunday, false);
    context.Transfer("VictimIsGuard", VictimIsGuard, false);
    context.Transfer("VictimIsBandit", VictimIsBandit, false);
    context.Transfer("VictimIsTrader", VictimIsTrader, false);
}
}