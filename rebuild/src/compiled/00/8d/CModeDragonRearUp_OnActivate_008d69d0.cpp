// CModeDragonRearUp::OnActivate  @ 008d69d0
// void __fastcall OnActivate(CModeDragonRearUp* this)

struct CModeDragonRearUp;

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct CSomeState {
    char pad[0x10];
    unsigned char flag;   // +0x10
};

// base OnActivate: thiscall on this (ecx). Declared extern -> real call.
void __fastcall DragonRear_BaseActivate(CModeDragonRearUp* self);

struct CModeDragonRearUp {
    void* vtbl;           // +0x00
    CSomeState* state;    // +0x04
    void OnActivate();
};

void CModeDragonRearUp::OnActivate() {
    DragonRear_BaseActivate(this);
    CSomeState* s = this->state;
    if (s->flag == 1) {
        ((CThingAICreature*)s)->SuspendAI(0, this);
    }
}