// CAIStateGroup_MazeTutorialBattleTeleport::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority,
// const CCharString&) -- base initialisation, then stamp the teleport frame and remember the creature's position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953FE0, length 84 bytes (+ int3 padding), `ret 14h`.
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (five arguments forwarded), CAIStateGroupBase::GetPWorld
// @ 0x008FCFC0 with its result DROPPED (`GetPWorld()->GetFrameCount()`, a static GetFrameCount), GFGetCurrentFrame
// @ 0x0049D870, CThing::GetPosition @ 0x004C73D0 (returns a const C3DVector& into the creature; manifest
// "CVertexBufferWin32::DoSizeof" is a propagated label).
// Shape: FrameOfLastTeleport (+0x20) stored between the position call's argument load and the call; the
// C3DVector assignment at +0x24 is the IMPLICIT operator= of a class with a user copy constructor (three dword
// moves through `add esi,24h`: the compiler-generated assignment is inlined with the member's address formed
// by an add; a user-written memberwise operator= addresses [esi+24h] directly). Member
// names from rebuild/include/engine/CAIStateGroup_MazeTutorialBattleTeleport.h; the header flattens the base,
// so the layout is local.
struct CAIBrain;
struct CThingAICreature;
struct CCharString;

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CThingCreatureBase {
    const C3DVector& GetPosition();         // 0x004C73D0
};

long __fastcall GFGetCurrentFrame();        // 0x0049D870

struct CWorld {
    static long GetFrameCount() { return GFGetCurrentFrame(); }
};

struct CAIBrainModel {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrainModel* PBrain;                  // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);  // 0x008FCEC0
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_MazeTutorialBattleTeleport : CAIStateGroupBase {
    unsigned char _pad14[0xc];              // +0x14 maze base state (untouched here)
    long FrameOfLastTeleport;               // +0x20
    C3DVector LastTeleportPos;              // +0x24
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);
};

void CAIStateGroup_MazeTutorialBattleTeleport::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
    FrameOfLastTeleport = GetPWorld()->GetFrameCount();
    LastTeleportPos = PBrain->Creature->GetPosition();
}
