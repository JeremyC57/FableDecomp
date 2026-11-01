// CModeDragonRearUp::OnDeactivate  @ 008d6a10

struct CModeDragonRearUp;

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct SomeState {
    char pad[0x10];
    unsigned char flag;   // +0x10
};

struct CModeDragonRearUp {
    void* vfptr;          // +0
    SomeState* state;     // +4
    void OnDeactivate();
};

// base-class deactivate, tail-called (thiscall) -- 0x861d00
void __fastcall DragonRearUpBase_861d00(CModeDragonRearUp* self);

void CModeDragonRearUp::OnDeactivate()
{
    SomeState* s = this->state;
    if (s->flag == 1)
        ((CThingAICreature*)s)->SuspendAI(0, this);
    DragonRearUpBase_861d00(this);
}