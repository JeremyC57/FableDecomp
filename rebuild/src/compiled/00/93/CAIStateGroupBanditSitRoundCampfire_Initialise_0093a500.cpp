// CAIStateGroup_BanditSitRoundCampfire::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A500, length 48 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (all five arguments forwarded).
// After the base Initialise the three bandit timers are reset in source order
// (FrameToNextSitOnSeat +0x24, WasCarryingWeapon +0x2c, FrameToNextCheckIsRunnable +0x28:
// retail stores them 0x24, 0x2c, 0x28 from one zeroed eax). Member names and offsets from
// struct_layouts_egor.tsv (CAIStateGroup_BanditSitRoundCampfire, 48 bytes); the generated
// engine header has no entry for this class.
struct CAIBrain;
struct CThingAICreature;
struct CCharString;
typedef long EScriptAIPriority;

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x008FCEC0
    unsigned char _pad[0x10];               // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    unsigned char PCampfire[8];             // +0x14 CIntelligentPointer<CThingObject>
    long          SeatPosIndex;             // +0x1c
    bool          GotSeatPos;               // +0x20
    bool          Sitting;                  // +0x21
    unsigned long FrameToNextSitOnSeat;     // +0x24
    unsigned long FrameToNextCheckIsRunnable;   // +0x28
    bool          WasCarryingWeapon;        // +0x2c
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
};

void CAIStateGroup_BanditSitRoundCampfire::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    FrameToNextSitOnSeat = 0;
    WasCarryingWeapon = false;
    FrameToNextCheckIsRunnable = 0;
}