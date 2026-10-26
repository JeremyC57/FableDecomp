// NCreatureMode::CModeBlock::OnDeactivate() -- mark the mode inactive and resume a live creature's animation if it is in state 1
// Leaf: ?OnDeactivate@CModeBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6D70, 38 bytes (+ int3 padding), `ret`; vtable 0x01267C3C slot 6.
// Callees: CThingAICreature::SetSuspendAnimation(bool, long) @ 0x00833A10.
// Name: FableWin.pdb `?OnDeactivate@CModeBlock@NCreatureMode@@UAEXXZ` (slot 6 of the CModeBlock vtable, after
//   OnActivate 0x008D8CE0 slot 5, landed).
// Layout: CModeBase (retail 0x1c): ParentThing +0x04, Active +0x18. Thing: state byte +0x10, dead bit +0x91 bit 0.
// Shape: the inline CModeBase::OnDeactivate clears Active first; then ParentThing is tested for null, alive and state 1
//   (VC reuses the `mov al,1` of the dead-bit test for the state compare) before the suspend-off call.
class CThingAICreature { public:
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

class CThing : public CThingAICreature { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                 // +0x10 creature state byte (compared with 1)
    unsigned char _pad11[0x91 - 0x11];
    unsigned char m_flags91;               // +0x91 bit 0: dead
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x10];             // WeaponBasedFilter, FramesBeforeRemovingSelf
    bool          Active;                  // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeBlock : CModeBase {            // vtable 0x01267C3C
    CThing* PCreature;                     // +0x1c
    void OnDeactivate();
};
}

void NCreatureMode::CModeBlock::OnDeactivate()
{
    CModeBase::OnDeactivate();
    if (ParentThing != 0 && !(ParentThing->m_flags91 & 1) && ParentThing->m_state == 1)
        ParentThing->SuspendAI(false, 0);
}