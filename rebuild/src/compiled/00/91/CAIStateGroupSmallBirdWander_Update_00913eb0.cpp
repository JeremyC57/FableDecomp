// CAIStateGroup_SmallBirdWander::Update() -- pick a random flight position near the bird, fly there, and finish
// Leaf: ?Update@CAIStateGroup_SmallBirdWander@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00913EB0, length 65 bytes (+ int3 padding).
// Callees: CThingPhysical::GetPos @ 0x004C73D0 (on PBrain->Creature),
// CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&, float, C3DVector*, bool)
// @ 0x00912700, CAIStateGroup_FlyingThingBase::ExecuteFlyingPath(const C3DVector&) @ 0x009130D0,
// CAIStateGroupBase::Terminate @ 0x008FCF40 (sets Terminated at +0x10).
// Shape: a 12-byte C3DVector local is the out-parameter (frame 0xc); the arguments go right to
// left (`push 1`, the out pointer, 15.0f, then the position reference); the creature is a named
// local so retail's `mov eax,[esi+4] ; mov ecx,[eax+18h]` pair survives. The `mov ecx,esi` that
// retail hoists ABOVE the `je` (and repeats after ExecuteFlyingPath) is the early-return form
// `if (!ok) { Terminate(); return; }` with the duplicated Terminate tail merged -- the plain
// `if (ok) Execute(); Terminate();` schedules the ecx load after the argument push.
#include "engine/C3DVector.h"

#include "engine/CThing.h"
class CThingAICreature : public CThing { public:
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
protected:
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector& out, bool allowGround) const;   // 0x00912700 ?GetRandomFlightPos@CAIStateGroup_FlyingThingBase@@IBE_NABVC3DVector@@MAAV2@_N@Z (Ego_r: protected const)
public:
    void ExecuteFlyingPath(const C3DVector& target);                                                  // 0x009130D0
};

struct CAIStateGroup_SmallBirdWander : CAIStateGroup_FlyingThingBase {
    virtual void Update();
};

void CAIStateGroup_SmallBirdWander::Update()
{
    C3DVector target;
    CThingAICreature* creature = PBrain->Creature;
    if (!GetRandomFlightPos(creature->GetPos(), 15.0f, target, true)) {
        Terminate();
        return;
    }
    ExecuteFlyingPath(target);
    Terminate();
}