// Behaviour test for CAIScratchpadDef::Transfer @ 0x007AD300: a loading context copies each default into the member; check names, order and defaults.
#include <cstdio>
#include <cstring>
#include <string>
static int g_calls = 0; static int g_bad = 0; static std::string g_names;
static void Note(const char* name) { ++g_calls; g_names += name; g_names += ","; }
class CCharString { public:
    std::string s;
    CCharString(const char* text = "", long length = -1) : s(text) { (void)length; }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
};
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE = 0 };
class CPersistContext { public:
    void Transfer(const char* name, unsigned long& value, const unsigned long& d) { Note(name); value = d; }
    void Transfer(const char* name, C3DVector& value, const C3DVector& d) { Note(name); value = d; }
    void Transfer(const char* name, float& value, const float& d) { Note(name); value = d; }
    void Transfer(const char* name, long& value, const long& d) { Note(name); value = d; }
    void Transfer(const char* name, EScriptingStateGroups& value, const EScriptingStateGroups& d) { Note(name); value = d; }
    void Transfer(const char* name, CCharString& value, const CCharString& d) { Note(name); value = d; }
    void Transfer(const char* name, bool& value, const bool& d) { Note(name); value = d; }
};
class CDefClassBase { public:
    virtual ~CDefClassBase() {}
    virtual void Transfer(CPersistContext& context) { (void)context; }
};
class CAIScratchpadDef : public CDefClassBase { public:
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

int main()
{
    CAIScratchpadDef d;
    d.ThankingPhrase = 7; d.WanderCentrePoint = C3DVector(1, 2, 3); d.GossipCounter = 9; d.Action.s = "x";
    d.FollowDistance = 4; d.TimeToSpendHarassingHero = 11; d.CombatNearbyEnemyFleeingBreakOffRange = 0;
    d.CombatNearbyBreakOffRange = 0; d.AllowedToFollow = false; d.IsPushableByHero = false; d.TableName.s = "t";
    d.AvoidRegionExits = false; d.KillSelfAndStealableItemAfterReachingDestination = false; d.LookForFiniteTime = false;
    d.TargetingDistanceOffset = 5;
    CPersistContext ctx;
    d.Transfer(ctx);
    if (g_calls != 33) ++g_bad;
    if (d.ThankingPhrase != 0 || d.WanderCentrePoint.X != 0 || d.WanderCentrePoint.Z != 0 || d.GossipCounter != 0) ++g_bad;
    if (d.Action.s != "" || d.TableName.s != "" || d.FollowDistance != 0 || d.TimeToSpendHarassingHero != 0) ++g_bad;
    if (d.CombatNearbyEnemyFleeingBreakOffRange != 5.0f || d.CombatNearbyBreakOffRange != 10.0f) ++g_bad;
    if (!d.AllowedToFollow || !d.IsPushableByHero || !d.AvoidRegionExits || !d.LookForFiniteTime) ++g_bad;
    if (!d.KillSelfAndStealableItemAfterReachingDestination || d.DisableHeadLooking || d.TargetingDistanceOffset != 0) ++g_bad;
    if (g_names.find("ThankingPhrase,IgnoringPhrase,WanderCentrePoint,") != 0) ++g_bad;
    if (g_names.find("SeatName,DisableHeadLooking,IsPushableByHero,LookForFiniteTime,AvoidRegionExits,TargetingDistanceOffset,") == std::string::npos) ++g_bad;
    if (g_bad == 0) printf("007ad300_TEST PASS\n"); else printf("007ad300_TEST FAIL %d\n", g_bad);
    return 0;
}