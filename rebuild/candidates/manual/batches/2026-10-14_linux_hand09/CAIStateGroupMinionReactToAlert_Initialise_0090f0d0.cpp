// CAIStateGroup_MinionReactToAlert::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F0D0, length 51 bytes (+ int3 padding). Vtable slot 1
// of CAIStateGroup_MinionReactToAlert (0x01289070).
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (all five arguments forwarded). Global: the frame-rate
// long @ 0x01375550.
// Shape: base Initialise, then `Timer = g_FramesPerSecond * 2` (two seconds in frames: `lea eax,[edx+edx]`).
// Members from struct_layouts_egor.tsv (LastAlertReactedTo +0x14, Timer +0x18, 28 bytes); no engine header.
struct CAIBrain;
struct CThingAICreature;
struct CCharString;
typedef long EScriptAIPriority;

extern long g_FramesPerSecond;                  // 0x01375550

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x008FCEC0
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_MinionReactToAlert : CAIStateGroupBase {
    unsigned long LastAlertReactedTo;           // +0x14
    long          Timer;                        // +0x18
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
};

void CAIStateGroup_MinionReactToAlert::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    Timer = g_FramesPerSecond * 2;
}
