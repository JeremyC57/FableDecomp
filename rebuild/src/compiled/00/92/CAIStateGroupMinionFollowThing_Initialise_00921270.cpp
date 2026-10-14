// CAIStateGroup_MinionFollowThing::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// -- base initialisation, then the follow defaults.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00921270, length 66 bytes (+ int3 padding), `ret 14h`.
// Callee: CAIStateGroupBase::Initialise @ 0x008FCEC0 (same five arguments forwarded).
// Members are the PDB names from rebuild/include/engine/CAIStateGroup_MinionFollowThing.h: FollowDistance
// +0x1c (1.0f), AvoidDynamicObstacles +0x20 (true), PreviousPushableByHero +0x21, ExpressionUsed +0x22,
// FollowerAdded +0x23 (false, one shared `xor al,al`), CurrentMovementType +0x24 (3). The PFollowee
// CIntelligentPointer at +0x14 is untouched. The header flattens the base, so the class is modelled locally.
struct CAIBrain;
struct CThingAICreature;
struct CCharString;

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);  // 0x008FCEC0
};

struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    unsigned char PFollowee[8];             // +0x14 CIntelligentPointer<CThingCreatureBase>
    float FollowDistance;                   // +0x1c
    bool AvoidDynamicObstacles;             // +0x20
    bool PreviousPushableByHero;            // +0x21
    bool ExpressionUsed;                    // +0x22
    bool FollowerAdded;                     // +0x23
    long CurrentMovementType;               // +0x24 EMovementType
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);
};

void CAIStateGroup_MinionFollowThing::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
    FollowDistance = 1.0f;
    AvoidDynamicObstacles = true;
    PreviousPushableByHero = false;
    ExpressionUsed = false;
    FollowerAdded = false;
    CurrentMovementType = 3;
}