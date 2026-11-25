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
};

struct CModeDragonRearUp : public ModeBase {
    SomeState* state;        // +4
    void OnActivate();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x008621A0 ?OnActivate@CModeStandBase@@UAEXXZ (was ?BaseOnActivate@ModeBase@@QAEXXZ)
class CModeStandBase { public: virtual void OnActivate(); };
void CModeDragonRearUp::OnActivate()
{
    reinterpret_cast<CModeStandBase*>(this)->CModeStandBase::OnActivate();
    if (this->state->flag == 1)
        ((CThingAICreature*)(this->state))->SuspendAI(1, this);
}