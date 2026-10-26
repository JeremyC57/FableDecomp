// CTCAIScratchpad::OnCreate() -- fetch the owner's CAIScratchpadDef sub-def into PDef and copy every scratchpad setting (phrases, wander area, gossip, action, follow/combat/steal/seat options) from it
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD880, 312 bytes (+8 int3 padding), `ret`.
// Callees: CDefClassBase::GetSubDef<CAIScratchpadDef>(CDefPointer<const CAIScratchpadDef>&) 0x007AD9C0 (manifest label
//   GetSubDef<CAICreatureWillPowerIndicatorDef> is propagated; this is the instantiation called on the owner's base def),
//   CCharString::operator=(const CCharString&) 0x0099EFB0.
// Layout: Ego_r.pdb's CTCAIScratchpad (0x84 bytes) and CAIScratchpadDef match the retail offsets: the member block
//   +0x0C..+0x7F mirrors the def's +0x28..+0x9B, PDef +0x80; the owner thing's base def at +0x70 (landed CTCLook::OnCreate).
// Shape: one assignment per member in declaration order through the def pointer's inline operator-> (re-read after each
//   out-of-line string assignment); WanderCentrePoint is the implicit C3DVector copy (`lea` of both sides).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE };

class CAIScratchpadDef { public:
    unsigned char _base[0x28];                               // CDefClassBase part
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
    bool GetSubDef(CDefPointer<const CAIScratchpadDef>& out);    // 0x007AD9C0
};
class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PBaseDef;                                 // +0x70
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