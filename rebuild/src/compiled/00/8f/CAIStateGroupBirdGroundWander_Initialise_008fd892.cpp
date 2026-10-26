#pragma optimize("s",on)
// CAIStateGroup_BirdGroundWander::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD892, oracle length 13 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebp ; mov ebp,esp ; and dword ptr [ecx+14h],0 ; pop ebp ; jmp CAIStateGroupBase::Initialise
// Resets the group's frame counter at +0x14 and tail-calls the base Initialise @ 0x008FCEC0
// with all five arguments left in place. The frame pointer around a one-instruction body is the
// unoptimised build of this translation unit (lander pragma sweep).
class CAIBrain;
class CThingAICreature;
class CCharString;
typedef long EScriptAIPriority;

struct CAIStateGroupBase_Retail {
    unsigned char _pad_0x00[0x14];
    unsigned long FrameLastFinishedWalking;    // +0x14 (PDB name for +0x14; +0x30 named from use)
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x008FCEC0
};

struct CAIStateGroup_BirdGroundWander_Retail : CAIStateGroupBase_Retail {
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
};

void CAIStateGroup_BirdGroundWander_Retail::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    this->FrameLastFinishedWalking = 0;
    CAIStateGroupBase_Retail::Initialise(brain, creature, priority, scriptPriority, name);
}