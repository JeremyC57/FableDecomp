// CModeChargeForcePush::OnActivate  @ 008d68e0
// __fastcall void OnActivate(CModeChargeForcePush* this)

class CThingAICreature { public:
    char pad0[0x10];
    unsigned char flag;   // +0x10
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

// base-class OnActivate (direct thiscall member call @ 0x8621a0)
void __fastcall BaseMode_OnActivate(void* self);

struct CModeChargeForcePush {
    void* vtbl;          // +0x00
    CThingAICreature* state;   // +0x04
    void OnActivate();
};

void CModeChargeForcePush::OnActivate()
{
    BaseMode_OnActivate(this);
    CThingAICreature* s = this->state;
    if (s->flag == 1)
        s->SuspendAI(1, 0);
}