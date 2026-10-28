// NTCScriptedControl::CActionSummonerLightningAttackTarget::CActionSummonerLightningAttackTarget(CThing*) -- summoner lightning attack on a target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713180, length 117 bytes (`ret 4`).
// Callees: CBase::CBase @ 0x0099A2F0 (x2: this action and the embedded turn action), CThing::GetScriptThing
// @ 0x004C7E50 (by-value CScriptThing built straight into PTarget), CThing::GetPosition @ 0x004C73D0.
// Vtables 0x0126586C = NTCScriptedControl::CActionSummonerLightningAttackTarget, 0x01265534 =
// NTCScriptedControl::CActionTurnToFacePosition (rtti-vtables.tsv).
//
// Shape: the landed scripted-action shape (novtable CActionBase zeroing PTCScript/Activated/Terminated from the
// ebx zero register before the vtable store), PTarget from the target's GetScriptThing, StartedAttack and
// TurnedToFace false, then the embedded CActionTurnToFacePosition member built by its inline constructor from
// the target's position (fetched BEFORE the member's out-of-line root constructor: the argument is evaluated
// first and kept in ebp): root CBase(), zeroed action words, its vtable, the position copied word by word,
// and the turn tolerance 1/72 (0x3C638E39) as an immediate. The position must be three SCALAR members copied
// in the init list (a C3DVector member copies through `lea ecx,[edi+0Ch]`). Members carry the PDB names.
// Leaf: ??0CActionSummonerLightningAttackTarget

struct CBase {                                  // root of the scripted actions (vtable 0x0129A7C4)
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

class CBaseObject { public:
    virtual void v00();
};

class CScriptThing : public CBaseObject { public:             // vtable 0x01238C8C, 12 bytes
    virtual void v00();
    void* PImp_Data;                            // +0x04
    void* PImp_Info;                            // +0x08
    CScriptThing(const CScriptThing& other);    // non-POD: returned through a hidden pointer
};

class C3DVector { public:
    float X, Y, Z;
};

class CThing { public:
    CScriptThing GetScriptThing();              // 0x004C7E50
    const C3DVector& GetPos() const;    // 0x004C73D0
};

namespace NTCScriptedControl {

struct __declspec(novtable) CActionBase : CBase {   // abstract: its own vtable store is dead
    void* PTCScript;                            // +0x04 CTCScriptedControl*
    bool  Activated;                            // +0x08
    bool  Terminated;                           // +0x09
    CActionBase() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionTurnToFacePosition : CActionBase {    // vtable 0x01265534, 0x1c bytes
    float     Position_X;                       // +0x0c C3DVector::X
    float     Position_Y;                       // +0x10 C3DVector::Y
    float     Position_Z;                       // +0x14 C3DVector::Z
    float     Tolerance;                        // +0x18 (descriptive)
    virtual void v00();
    CActionTurnToFacePosition(const C3DVector& position)
        : CActionBase(), Position_X(position.X), Position_Y(position.Y), Position_Z(position.Z), Tolerance(1.0f / 72.0f) { }
};

struct CActionSummonerLightningAttackTarget : CActionBase {   // vtable 0x0126586C, 0x38 bytes
    CScriptThing PTarget;                       // +0x0c
    bool StartedAttack;                         // +0x18
    bool TurnedToFace;                          // +0x19
    CActionTurnToFacePosition TurnToFaceAction; // +0x1c
    virtual void v00();
    CActionSummonerLightningAttackTarget(CThing* target);
};

CActionSummonerLightningAttackTarget::CActionSummonerLightningAttackTarget(CThing* target)
    : CActionBase()
    , PTarget(target->GetScriptThing())
    , StartedAttack(false)
    , TurnedToFace(false)
    , TurnToFaceAction(target->GetPos())
{
}

}