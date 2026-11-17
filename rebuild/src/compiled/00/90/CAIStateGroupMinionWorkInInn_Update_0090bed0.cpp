// CAIStateGroup_MinionWorkInInn::Update() -- with a workplace: one time in five (Random(100) < 21) clean a bench; otherwise walk to a random point 0..6m from the workplace position (random heading), retrying while the move is in progress, and idle on arrival; terminate otherwise
// Leaf: ?Update@CAIStateGroup_MinionWorkInInn@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BED0, length 400 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the creature's workplace pointer at +0x170),
//          CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CleanBench @ 0x0090BD40, CThing::GetPos
//          @ 0x004C73D0, GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall: the seed in ecx),
//          C3DVector::Rotate @ 0x00A14260, CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (C3DVector by
//          value), AIAction_MinionIdle @ 0x0097FA00, Terminate @ 0x008FCF40.
// Globals: 0.0001f @ 0x0129BA3C.
// Shape: the *seed-form Random; the workplace position is a user-copy-ctor copy, the offset a (r + 0.0001, 0, 0) vector
//          rotated in place, the target `offset + position` built by a three-float ctor (Z first); a do/while re-tests
//          the workplace before each try (the sibling of CAIStateGroup_IdleAtHome::Update 0x00917380).
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04); the creature's
//          workplace intelligent pointer at +0x170 as in the landed TavernServer rows.
class C3DVector {
public:
    void Rotate(float a0, float a1);   // 0x00A14260 (Ego_r.pdb ?Rotate@C3DVector@@QAEXMM@Z)
private: public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
#include "engine/CThing.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingAICreature { public:
    unsigned char _pad[0x170];
    CIntelligentPointer<CThing> PWorkplace;         // +0x170 (descriptive)
    CThing* GetPWorkplace() { return PWorkplace.Get(); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
long __fastcall GFROR13(unsigned long);   // 0x00497890

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h;
// AIAction_MinionIdle 0x0097FA00 is ?AIAction_MinionIdle@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ (both PDBs; overlay opt-in _MINIONIDLE).
// Update overrides the base's virtual slot 12, so it now mangles ?Update@CAIStateGroup_MinionWorkInInn@@UAEXXZ (both PDBs; was QAE).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONIDLE
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

struct CAIStateGroup_MinionWorkInInn : CAIStateGroupBase {
    // (the row's helpers, moved here from its local base model)
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
private:
    void CleanBench();                          // 0x0090BD40
public:
    void Update();
};

void CAIStateGroup_MinionWorkInInn::Update()
{
    if (GetCreature()->GetPWorkplace() == 0) {
        Terminate();
        return;
    }
    if (Random(100) < 21) {
        CleanBench();
        return;
    }
    C3DVector workplace = GetCreature()->GetPWorkplace()->GetPos();
    C3DVector offset(GFFloatRandom(6.0f, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
    ((C3DVector*)&offset)->Rotate(GFFloatRandom(1.0f, GetPWorldSeed()), false);
    C3DVector target = offset + workplace;
    long result;
    do {
        if (GetCreature()->GetPWorkplace() == 0) {
            Terminate();
            return;
        }
        bool reached;
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, target, 1.0f, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 2 && GetCreature()->GetPWorkplace() != 0) {
        AIAction_MinionIdle();
        return;
    }
    Terminate();
}