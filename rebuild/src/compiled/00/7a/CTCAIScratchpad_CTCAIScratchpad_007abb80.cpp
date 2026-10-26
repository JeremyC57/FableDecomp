// CTCAIScratchpad::CTCAIScratchpad(CThing*) -- CTCBase(owner), own vtable, four empty strings, null def pointer
// Leaf: ??0CTCAIScratchpad
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ABB80, length 67 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name), CCharString::CCharString() @ 0x0099E4B0 (x4). Own vtable 0x0126D07C (rtti-vtables.tsv).
//
// Layout = Ego_r.pdb CTCAIScratchpad (0x84 bytes): the CCharString members Action +0x48, TargetName +0x50,
// TableName +0x70, SeatName +0x74 and the CDefPointer<CAIScratchpadDef> PDef +0x80; the scalar members in
// between (phrases, wander centre/distances, flags) are left uninitialised by the constructor.
// Shape: base constructor, own vtable store (scheduled after the first member's `lea`), the four string
// default constructors in declaration order, then the def pointer's inline constructor (a single
// zero store). engine/CTCBase.h holds the layout only, so the base is modelled as `CTCBase_Calls`.
class CThing;
class CAIScratchpadDef;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

class CCharString { public:
    void* m_pData;
    CCharString();                                          // 0x0099E4B0
};

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class C3DVector { public: float X, Y, Z; };

class CTCAIScratchpad : public CTCBase_Calls { public:                    // vtable 0x0126D07C
    unsigned long  ThankingPhrase;                          // +0x0c
    unsigned long  IgnoringPhrase;                          // +0x10
    C3DVector      WanderCentrePoint;                       // +0x14
    float          WanderMinDistance;                       // +0x20
    float          WanderMaxDistance;                       // +0x24
    long           GossipCounter;                           // +0x28
    long           MaxGossipPhrase;                         // +0x2c
    unsigned long  WarningPhrase;                           // +0x30
    unsigned long  BeerRequestPhrase;                       // +0x34
    long           ScriptingStateGroup;                     // +0x38
    float          MaxHeroReactionDistance;                 // +0x3c
    long           ActionFrequency;                         // +0x40
    float          ActionFrequencyVariation;                // +0x44
    CCharString    Action;                                  // +0x48
    bool           FaceHeroForAction;                       // +0x4c
    CCharString    TargetName;                              // +0x50
    float          FollowDistance;                          // +0x54
    bool           AttackHeroOnSight;                       // +0x58
    long           TimeToSpendHarassingHero;                // +0x5c
    float          CombatNearbyEnemyFleeingBreakOffRange;   // +0x60
    float          CombatNearbyBreakOffRange;               // +0x64
    bool           StealStealableItems;                     // +0x68
    bool           RecoverStealableItems;                   // +0x69
    bool           TakeStealableItemToRandomDestination;    // +0x6a
    bool           KillSelfAndStealableItemAfterReachingDestination; // +0x6b
    bool           AllowedToFollow;                         // +0x6c
    CCharString    TableName;                               // +0x70
    CCharString    SeatName;                                // +0x74
    bool           DisableHeadLooking;                      // +0x78
    bool           IsPushableByHero;                        // +0x79
    bool           LookForFiniteTime;                       // +0x7a
    bool           AvoidRegionExits;                        // +0x7b
    float          TargetingDistanceOffset;                 // +0x7c
    CDefPointer<CAIScratchpadDef> PDef;                     // +0x80
    virtual void v00();
    CTCAIScratchpad(CThing* owner);
};

CTCAIScratchpad::CTCAIScratchpad(CThing* owner)
    : CTCBase_Calls(owner)
{
}