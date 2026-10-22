// CAIScratchpadDef::Transfer(CPersistContext&) -- persist every AI scratchpad def field by name with its default value
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD300, 976 bytes (ret 4).
// Callees: CPersistContext::Transfer overloads -- unsigned long 0x004106F0, C3DVector 0x004859AD, float 0x00410620,
//   long 0x00410BE0, EScriptingStateGroups 0x007AD6D0, CCharString 0x004109A0, bool 0x004045C0 (all catalogued
//   "Transfer<signed_char>"/"Transfer<CFloatRange>", propagated template labels); CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0 ("" @ 0x0122D70E).
// Layout: engine/CAIScratchpadDef.h is the flattened pack(1) PDB view (no base class, raw string pointers, the
// wander centre split into floats), so the members are modelled locally at the same Ego_r.pdb offsets and names
// (as the landed CAIScratchpadDef::operator= 0x007ADF00 does); every transferred offset +0x28..+0x98 agrees.
// Shape: one Transfer per member in declaration order; each default is a compiler temporary (the first takes a
// frame word, the rest the dead context-argument slot); the string defaults are CCharString("") temporaries.
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE = 0 };

struct CPersistContext {
    void Transfer(const char* name, unsigned long& value, const unsigned long& defaultValue);   // 0x004106F0
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);           // 0x004859AD
    void Transfer(const char* name, float& value, const float& defaultValue);                   // 0x00410620
    void Transfer(const char* name, long& value, const long& defaultValue);                     // 0x00410BE0
    void Transfer(const char* name, EScriptingStateGroups& value, const EScriptingStateGroups& defaultValue);   // 0x007AD6D0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);       // 0x004109A0
    void Transfer(const char* name, bool& value, const bool& defaultValue);                     // 0x004045C0
};

struct CDefClassBase {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base_0x04[0x24];
};

struct CAIScratchpadDef : CDefClassBase {               // vtable 0x0126D014
    unsigned long ThankingPhrase;                       // +0x28
    unsigned long IgnoringPhrase;                       // +0x2c
    C3DVector     WanderCentrePoint;                    // +0x30
    float         WanderMinDistance;                    // +0x3c
    float         WanderMaxDistance;                    // +0x40
    long          GossipCounter;                        // +0x44
    long          MaxGossipPhrase;                      // +0x48
    unsigned long WarningPhrase;                        // +0x4c
    unsigned long BeerRequestPhrase;                    // +0x50
    EScriptingStateGroups ScriptingStateGroup;          // +0x54
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
    virtual void Transfer(CPersistContext& context);
};

void CAIScratchpadDef::Transfer(CPersistContext& context)
{
    context.Transfer("ThankingPhrase", ThankingPhrase, 0UL);
    context.Transfer("IgnoringPhrase", IgnoringPhrase, 0UL);
    context.Transfer("WanderCentrePoint", WanderCentrePoint, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("WanderMinDistance", WanderMinDistance, 0.0f);
    context.Transfer("WanderMaxDistance", WanderMaxDistance, 0.0f);
    context.Transfer("GossipCounter", GossipCounter, 0L);
    context.Transfer("MaxGossipPhrase", MaxGossipPhrase, 0L);
    context.Transfer("WarningPhrase", WarningPhrase, 0UL);
    context.Transfer("BeerRequestPhrase", BeerRequestPhrase, 0UL);
    context.Transfer("ScriptingStateGroup", ScriptingStateGroup, SCRIPTING_STATE_GROUP_NONE);
    context.Transfer("MaxHeroReactionDistance", MaxHeroReactionDistance, 0.0f);
    context.Transfer("ActionFrequency", ActionFrequency, 0L);
    context.Transfer("ActionFrequencyVariation", ActionFrequencyVariation, 0.0f);
    context.Transfer("Action", Action, CCharString(""));
    context.Transfer("FaceHeroForAction", FaceHeroForAction, false);
    context.Transfer("TargetName", TargetName, CCharString(""));
    context.Transfer("FollowDistance", FollowDistance, 0.0f);
    context.Transfer("AttackHeroOnSight", AttackHeroOnSight, false);
    context.Transfer("TimeToSpendHarassingHero", TimeToSpendHarassingHero, 0);
    context.Transfer("CombatNearbyEnemyFleeingBreakOffRange", CombatNearbyEnemyFleeingBreakOffRange, 5.0f);
    context.Transfer("CombatNearbyBreakOffRange", CombatNearbyBreakOffRange, 10.0f);
    context.Transfer("StealStealableItems", StealStealableItems, false);
    context.Transfer("RecoverStealableItems", RecoverStealableItems, false);
    context.Transfer("TakeStealableItemToRandomDestination", TakeStealableItemToRandomDestination, false);
    context.Transfer("KillSelfAndStealableItemAfterReachingDestination", KillSelfAndStealableItemAfterReachingDestination, true);
    context.Transfer("AllowedToFollow", AllowedToFollow, true);
    context.Transfer("TableName", TableName, CCharString(""));
    context.Transfer("SeatName", SeatName, CCharString(""));
    context.Transfer("DisableHeadLooking", DisableHeadLooking, false);
    context.Transfer("IsPushableByHero", IsPushableByHero, true);
    context.Transfer("LookForFiniteTime", LookForFiniteTime, true);
    context.Transfer("AvoidRegionExits", AvoidRegionExits, true);
    context.Transfer("TargetingDistanceOffset", TargetingDistanceOffset, 0.0f);
}