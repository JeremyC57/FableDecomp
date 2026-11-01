// CModeChargeForcePush::OnActivate  @ 008d6900
// Leaf: OnActivate
// __fastcall void OnActivate(CModeChargeForcePush* this)

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct CMember {
    char pad[0x10];
    unsigned char flag;                    // +0x10
};

struct CModeChargeForcePush {
    void*    base;        // +0x00
    CMember* member;      // +0x04
};

// external target
extern void __fastcall CMode_Base_OnActivate(void* self);      // 0x861d00

void __fastcall CModeChargeForcePush_OnActivate(CModeChargeForcePush* self)
{
    CMember* m;
    CMode_Base_OnActivate(self);
    m = self->member;
    if (m->flag == 1)
        ((CThingAICreature*)m)->SuspendAI(0, 0);
}