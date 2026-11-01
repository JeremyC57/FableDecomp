// CModeDragonRearUp::OnActivate  @ 008d69f0
struct CModeDragonRearUp;

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct SomeState {
    char pad[0x10];
    unsigned char flag;                        // +0x10
};

struct ModeBase {
    virtual ~ModeBase();     // vptr at +0 -> base subobject occupies 4 bytes
    void BaseOnActivate();   // -> 0x8621a0 (thiscall)
};

struct CModeDragonRearUp : public ModeBase {
    SomeState* state;        // +4
    void OnActivate();
};

void CModeDragonRearUp::OnActivate()
{
    this->BaseOnActivate();
    if (this->state->flag == 1)
        ((CThingAICreature*)(this->state))->SuspendAI(1, this);
}