// Behaviour test for CTCAIScratchpad::OnCreate (0x007AD880): every setting is copied from the sub-def.
#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    const char* text;
    int assigns;
    const CCharString& operator=(const CCharString& other) { text = other.text; ++assigns; return *this; }
};
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE };

class CAIScratchpadDef { public:
    long          _base;                                     // CDefClassBase part (size irrelevant here)
    unsigned long ThankingPhrase;                            // +0x28
    unsigned long IgnoringPhrase;                            // +0x2c
    C3DVector     WanderCentrePoint;                         // +0x30
    float         WanderMinDistance;                         // +0x3c
    float         WanderMaxDistance;                         // +0x40
    long          GossipCounter;                             // +0x44
    long          MaxGossipPhrase;                           // +0x48
    unsigned long WarningPhrase;                             // +0x4c
    unsigned long BeerRequestPhrase;                         // +0x50
    EScriptingStateGroups ScriptingStateGroup;               // +0x54
    float         MaxHeroReactionDistance;                   // +0x58
    long          ActionFrequency;                           // +0x5c
    float         ActionFrequencyVariation;                  // +0x60
    CCharString   Action;                                    // +0x64
    bool          FaceHeroForAction;                         // +0x68
    CCharString   TargetName;                                // +0x6c
    float         FollowDistance;                            // +0x70
    bool          AttackHeroOnSight;                         // +0x74
    long          TimeToSpendHarassingHero;                  // +0x78
    float         CombatNearbyEnemyFleeingBreakOffRange;     // +0x7c
    float         CombatNearbyBreakOffRange;                 // +0x80
    bool          StealStealableItems;                       // +0x84
    bool          RecoverStealableItems;                     // +0x85
    bool          TakeStealableItemToRandomDestination;      // +0x86
    bool          KillSelfAndStealableItemAfterReachingDestination;   // +0x87
    bool          AllowedToFollow;                           // +0x88
    CCharString   TableName;                                 // +0x8c
    CCharString   SeatName;                                  // +0x90
    bool          DisableHeadLooking;                        // +0x94
    bool          IsPushableByHero;                          // +0x95
    bool          LookForFiniteTime;                         // +0x96
    bool          AvoidRegionExits;                          // +0x97
    float         TargetingDistanceOffset;                   // +0x98
};

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CDefClassBase { public:
    const CAIScratchpadDef* sub;
    bool GetSubDef(CDefPointer<const CAIScratchpadDef>& out) { out.Object = sub; return true; }
};
class CThing { public:
    CDefClassBase* PBaseDef;
};

class CTCAIScratchpad { public:
    void*         __vftable;                                 // +0x00 (CTCBase)
    CThing*       ParentThing;                               // +0x04
    bool          ToBeRemoved;                               // +0x08
    bool          IsInGlobalUpdateTCs;                       // +0x09
    unsigned long ThankingPhrase;                            // +0x0c
    unsigned long IgnoringPhrase;                            // +0x10
    C3DVector     WanderCentrePoint;                         // +0x14
    float         WanderMinDistance;                         // +0x20
    float         WanderMaxDistance;                         // +0x24
    long          GossipCounter;                             // +0x28
    long          MaxGossipPhrase;                           // +0x2c
    unsigned long WarningPhrase;                             // +0x30
    unsigned long BeerRequestPhrase;                         // +0x34
    EScriptingStateGroups ScriptingStateGroup;               // +0x38
    float         MaxHeroReactionDistance;                   // +0x3c
    long          ActionFrequency;                           // +0x40
    float         ActionFrequencyVariation;                  // +0x44
    CCharString   Action;                                    // +0x48
    bool          FaceHeroForAction;                         // +0x4c
    CCharString   TargetName;                                // +0x50
    float         FollowDistance;                            // +0x54
    bool          AttackHeroOnSight;                         // +0x58
    long          TimeToSpendHarassingHero;                  // +0x5c
    float         CombatNearbyEnemyFleeingBreakOffRange;     // +0x60
    float         CombatNearbyBreakOffRange;                 // +0x64
    bool          StealStealableItems;                       // +0x68
    bool          RecoverStealableItems;                     // +0x69
    bool          TakeStealableItemToRandomDestination;      // +0x6a
    bool          KillSelfAndStealableItemAfterReachingDestination;   // +0x6b
    bool          AllowedToFollow;                           // +0x6c
    CCharString   TableName;                                 // +0x70
    CCharString   SeatName;                                  // +0x74
    bool          DisableHeadLooking;                        // +0x78
    bool          IsPushableByHero;                          // +0x79
    bool          LookForFiniteTime;                         // +0x7a
    bool          AvoidRegionExits;                          // +0x7b
    float         TargetingDistanceOffset;                   // +0x7c
    CDefPointer<const CAIScratchpadDef> PDef;                // +0x80
    void OnCreate();
};

void CTCAIScratchpad::OnCreate()
{
    ParentThing->PBaseDef->GetSubDef(PDef);
    ThankingPhrase = PDef->ThankingPhrase;
    IgnoringPhrase = PDef->IgnoringPhrase;
    WanderCentrePoint = PDef->WanderCentrePoint;
    WanderMinDistance = PDef->WanderMinDistance;
    WanderMaxDistance = PDef->WanderMaxDistance;
    GossipCounter = PDef->GossipCounter;
    MaxGossipPhrase = PDef->MaxGossipPhrase;
    WarningPhrase = PDef->WarningPhrase;
    BeerRequestPhrase = PDef->BeerRequestPhrase;
    ScriptingStateGroup = PDef->ScriptingStateGroup;
    MaxHeroReactionDistance = PDef->MaxHeroReactionDistance;
    ActionFrequency = PDef->ActionFrequency;
    ActionFrequencyVariation = PDef->ActionFrequencyVariation;
    Action = PDef->Action;
    FaceHeroForAction = PDef->FaceHeroForAction;
    TargetName = PDef->TargetName;
    FollowDistance = PDef->FollowDistance;
    AttackHeroOnSight = PDef->AttackHeroOnSight;
    TimeToSpendHarassingHero = PDef->TimeToSpendHarassingHero;
    CombatNearbyEnemyFleeingBreakOffRange = PDef->CombatNearbyEnemyFleeingBreakOffRange;
    CombatNearbyBreakOffRange = PDef->CombatNearbyBreakOffRange;
    StealStealableItems = PDef->StealStealableItems;
    RecoverStealableItems = PDef->RecoverStealableItems;
    TakeStealableItemToRandomDestination = PDef->TakeStealableItemToRandomDestination;
    KillSelfAndStealableItemAfterReachingDestination = PDef->KillSelfAndStealableItemAfterReachingDestination;
    AllowedToFollow = PDef->AllowedToFollow;
    TableName = PDef->TableName;
    SeatName = PDef->SeatName;
    DisableHeadLooking = PDef->DisableHeadLooking;
    IsPushableByHero = PDef->IsPushableByHero;
    LookForFiniteTime = PDef->LookForFiniteTime;
    AvoidRegionExits = PDef->AvoidRegionExits;
    TargetingDistanceOffset = PDef->TargetingDistanceOffset;
}

int main()
{
    CAIScratchpadDef d;
    std::memset(&d, 0, sizeof(d));
    d.ThankingPhrase = 11; d.IgnoringPhrase = 12; d.WanderCentrePoint.X = 1.0f; d.WanderCentrePoint.Y = 2.0f; d.WanderCentrePoint.Z = 3.0f;
    d.WanderMinDistance = 4.0f; d.WanderMaxDistance = 5.0f; d.GossipCounter = 6; d.MaxGossipPhrase = 7; d.WarningPhrase = 8;
    d.BeerRequestPhrase = 9; d.ScriptingStateGroup = (EScriptingStateGroups)10; d.MaxHeroReactionDistance = 11.5f;
    d.ActionFrequency = 12; d.ActionFrequencyVariation = 13.5f; d.Action.text = "wave"; d.FaceHeroForAction = true;
    d.TargetName.text = "hero"; d.FollowDistance = 14.5f; d.AttackHeroOnSight = true; d.TimeToSpendHarassingHero = 15;
    d.CombatNearbyEnemyFleeingBreakOffRange = 16.5f; d.CombatNearbyBreakOffRange = 17.5f; d.StealStealableItems = true;
    d.RecoverStealableItems = false; d.TakeStealableItemToRandomDestination = true; d.KillSelfAndStealableItemAfterReachingDestination = true;
    d.AllowedToFollow = true; d.TableName.text = "table"; d.SeatName.text = "seat"; d.DisableHeadLooking = true;
    d.IsPushableByHero = false; d.LookForFiniteTime = true; d.AvoidRegionExits = true; d.TargetingDistanceOffset = 18.5f;
    CDefClassBase base; base.sub = &d;
    CThing thing; thing.PBaseDef = &base;
    CTCAIScratchpad s;
    std::memset(&s, 0, sizeof(s));
    s.ParentThing = &thing;
    s.OnCreate();
    int ok = s.PDef.Object == &d;
    if (s.ThankingPhrase != 11 || s.IgnoringPhrase != 12 || s.WanderCentrePoint.Z != 3.0f || s.WanderMinDistance != 4.0f) ok = 0;
    if (s.WanderMaxDistance != 5.0f || s.GossipCounter != 6 || s.MaxGossipPhrase != 7 || s.WarningPhrase != 8) ok = 0;
    if (s.BeerRequestPhrase != 9 || s.ScriptingStateGroup != 10 || s.MaxHeroReactionDistance != 11.5f) ok = 0;
    if (s.ActionFrequency != 12 || s.ActionFrequencyVariation != 13.5f || std::strcmp(s.Action.text, "wave") != 0) ok = 0;
    if (!s.FaceHeroForAction || std::strcmp(s.TargetName.text, "hero") != 0 || s.FollowDistance != 14.5f) ok = 0;
    if (!s.AttackHeroOnSight || s.TimeToSpendHarassingHero != 15 || s.CombatNearbyEnemyFleeingBreakOffRange != 16.5f) ok = 0;
    if (s.CombatNearbyBreakOffRange != 17.5f || !s.StealStealableItems || s.RecoverStealableItems) ok = 0;
    if (!s.TakeStealableItemToRandomDestination || !s.KillSelfAndStealableItemAfterReachingDestination || !s.AllowedToFollow) ok = 0;
    if (std::strcmp(s.TableName.text, "table") != 0 || std::strcmp(s.SeatName.text, "seat") != 0 || !s.DisableHeadLooking) ok = 0;
    if (s.IsPushableByHero || !s.LookForFiniteTime || !s.AvoidRegionExits || s.TargetingDistanceOffset != 18.5f) ok = 0;
    if (s.Action.assigns != 1 || s.SeatName.assigns != 1) ok = 0;
    if (!ok) { std::printf("007ad880_TEST FAIL\n"); return 1; }
    std::printf("007ad880_TEST PASS\n");
    return 0;
}