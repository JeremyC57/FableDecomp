// NCreatureMode::CModeChargeForcePush::OnActivate  @ 008d6960
// Leaf: ?OnActivate@CModeChargeForcePush@NCreatureMode@@UAEXXZ
// __thiscall virtual void OnActivate()
// Identity (NF385): retail vftable 0x01268328 (RTTI CModeChargeForcePush@NCreatureMode) slot 5 = FableWin vftable slot 5
// ?OnActivate@CModeChargeForcePush@NCreatureMode@@UAEXXZ; Ego_r and FableWin creature_modes.obj both name it (Ego_r 28 B = retail).
// The base call is CModeStandBase::OnActivate (0x008621A0: slot 5 of the retail CModeStandBase vftable 0x01276D8C).

class CThingAICreature { public:
    char pad0[0x10];
    unsigned char flag;   // +0x10
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

struct CModeStandBase {
    virtual void OnActivate();     // 0x008621A0
};

namespace NCreatureMode {
struct CModeChargeForcePush : CModeStandBase {
    // +0x00 vfptr
    CThingAICreature* state;   // +0x04
    virtual void OnActivate();
};
}

void NCreatureMode::CModeChargeForcePush::OnActivate()
{
    CModeStandBase::OnActivate();
    CThingAICreature* s = this->state;
    if (s->flag == 1)
        s->SuspendAI(1, 0);
}
