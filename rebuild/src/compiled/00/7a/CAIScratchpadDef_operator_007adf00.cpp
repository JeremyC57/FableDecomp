// CAIScratchpadDef::operator=(const CAIScratchpadDef&) -- memberwise def copy: CDefClassBase part out of line, phrases, wander centre (C3DVector), AI tuning values, four CCharString members and the flag bytes; returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ADF00, length 337 bytes (speed-optimised TU: copies rotate eax/ecx/edx).
// Callees: CDefClassBase::operator= @ 0x00431F10 (catalogued "CThingBuildingDef::operator=", propagated),
//          CCharString::operator= @ 0x0099EFB0.
// Identity: the catalog label "CThingBuildingDef::operator=" is propagated. Vtable 0x0126D014 (CAIScratchpadDef, RTTI)
// slot 19 (Copy) is the 5-byte `jmp 0x007ADF00` at 0x007ADEF0.
// Layout: engine/CAIScratchpadDef.h is the flattened pack(1) PDB view (no base class, raw string pointers), so the copy is
// written against a local model at the same Ego_r.pdb offsets and names (every copied offset +0x28..+0x98 agrees).
// The C3DVector at +0x30 is copied as a 12-byte struct (the `lea` source/destination pointer-register block).
class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};

class C3DVector { public:
    float X, Y, Z;
};

#pragma pack(push, 1)
class CDefClassBase { public:
    virtual void Copy(const CDefClassBase& other);
    unsigned char _base_0x04[0x18];
    void*         PDefManager;                          // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char DefaultValsApplied;                   // +0x24 bitfield group
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)

class CAIScratchpadDef : public CDefClassBase { public:               // vtable 0x0126D014
    unsigned long ThankingPhrase;                       // +0x28
    unsigned long IgnoringPhrase;                       // +0x2c
    C3DVector     WanderCentrePoint;                    // +0x30
    float         WanderMinDistance;                    // +0x3c
    float         WanderMaxDistance;                    // +0x40
    long          GossipCounter;                        // +0x44
    long          MaxGossipPhrase;                      // +0x48
    unsigned long WarningPhrase;                        // +0x4c
    unsigned long BeerRequestPhrase;                    // +0x50
    long          ScriptingStateGroup;                  // +0x54 EScriptingStateGroups
    float         MaxHeroReactionDistance;              // +0x58
    long          ActionFrequency;                      // +0x5c
    float         ActionFrequencyVariation;             // +0x60
    CCharString   Action;                               // +0x64
    bool          FaceHeroForAction;                    // +0x68
    CCharString   TargetName;                           // +0x6c
    float         FollowDistance;                       // +0x70
    bool          AttackHeroOnSight;                    // +0x74
    long          TimeToSpendHarassingHero;             // +0x78
    float         CombatNearbyEnemyFleeingBreakOffRange; // +0x7c
    float         CombatNearbyBreakOffRange;            // +0x80
    bool          StealStealableItems;                  // +0x84
    bool          RecoverStealableItems;                // +0x85
    bool          TakeStealableItemToRandomDestination; // +0x86
    bool          KillSelfAndStealableItemAfterReachingDestination; // +0x87
    bool          AllowedToFollow;                      // +0x88
    CCharString   TableName;                            // +0x8c
    CCharString   SeatName;                             // +0x90
    bool          DisableHeadLooking;                   // +0x94
    bool          IsPushableByHero;                     // +0x95
    bool          LookForFiniteTime;                    // +0x96
    bool          AvoidRegionExits;                     // +0x97
    float         TargetingDistanceOffset;              // +0x98
    CAIScratchpadDef& operator=(const CAIScratchpadDef& other);
};

CAIScratchpadDef& CAIScratchpadDef::operator=(const CAIScratchpadDef& other)
{
    CDefClassBase::operator=(other);
    ThankingPhrase = other.ThankingPhrase;
    IgnoringPhrase = other.IgnoringPhrase;
    WanderCentrePoint = other.WanderCentrePoint;
    WanderMinDistance = other.WanderMinDistance;
    WanderMaxDistance = other.WanderMaxDistance;
    GossipCounter = other.GossipCounter;
    MaxGossipPhrase = other.MaxGossipPhrase;
    WarningPhrase = other.WarningPhrase;
    BeerRequestPhrase = other.BeerRequestPhrase;
    ScriptingStateGroup = other.ScriptingStateGroup;
    MaxHeroReactionDistance = other.MaxHeroReactionDistance;
    ActionFrequency = other.ActionFrequency;
    ActionFrequencyVariation = other.ActionFrequencyVariation;
    Action = other.Action;
    FaceHeroForAction = other.FaceHeroForAction;
    TargetName = other.TargetName;
    FollowDistance = other.FollowDistance;
    AttackHeroOnSight = other.AttackHeroOnSight;
    TimeToSpendHarassingHero = other.TimeToSpendHarassingHero;
    CombatNearbyEnemyFleeingBreakOffRange = other.CombatNearbyEnemyFleeingBreakOffRange;
    CombatNearbyBreakOffRange = other.CombatNearbyBreakOffRange;
    StealStealableItems = other.StealStealableItems;
    RecoverStealableItems = other.RecoverStealableItems;
    TakeStealableItemToRandomDestination = other.TakeStealableItemToRandomDestination;
    KillSelfAndStealableItemAfterReachingDestination = other.KillSelfAndStealableItemAfterReachingDestination;
    AllowedToFollow = other.AllowedToFollow;
    TableName = other.TableName;
    SeatName = other.SeatName;
    DisableHeadLooking = other.DisableHeadLooking;
    IsPushableByHero = other.IsPushableByHero;
    LookForFiniteTime = other.LookForFiniteTime;
    AvoidRegionExits = other.AvoidRegionExits;
    TargetingDistanceOffset = other.TargetingDistanceOffset;
    return *this;
}