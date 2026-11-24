// NCreatureMode::CModeChargeForcePush::OnDeactivate  @ 008d6980  __thiscall virtual void()
// Leaf: ?OnDeactivate@CModeChargeForcePush@NCreatureMode@@UAEXXZ
// Identity (NF385): retail vftable 0x01268328 (RTTI CModeChargeForcePush@NCreatureMode) slot 6 = FableWin vftable slot 6
// ?OnDeactivate@CModeChargeForcePush@NCreatureMode@@UAEXXZ (slot 5 is OnActivate = 0x008D6960); Ego_r and FableWin
// creature_modes.obj both name it (Ego_r 28 B = retail). The body undoes OnActivate: SuspendAI(false, 0).
// The base call is CModeStandBase::OnDeactivate (0x00861D00: slot 6 of the retail CModeStandBase vftable 0x01276D8C).
//
// push esi; mov esi,ecx; call 0x861d00 (base OnDeactivate)
// mov ecx,[esi+4]              ; ecx = this->sub
// cmp byte [ecx+0x10],1        ; sub->flag == 1 ?
// pop esi; jne skip
// push 0; push 0; call 0x833a10  ; SuspendAI(0,0)
// skip: ret
//
// The pointer at +4 is loaded into ecx because it is BOTH the object whose flag
// is tested AND the `this` for the conditional call 0x833a10.

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct Sub {
    char pad[0x10];
    unsigned char flag;          // +0x10
};

struct CModeStandBase {
    virtual void OnDeactivate(); // 0x00861D00
};

namespace NCreatureMode {
struct CModeChargeForcePush : CModeStandBase {
    // +0x00 vfptr
    Sub*  sub;                   // +0x04
    virtual void OnDeactivate();
};
}

void NCreatureMode::CModeChargeForcePush::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
    Sub* s = sub;
    if (s->flag == 1)
        ((CThingAICreature*)s)->SuspendAI(0, 0);
}
