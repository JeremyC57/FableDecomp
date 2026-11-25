// CModeHitByEpicSpell::OnActivate  @ 008d6a80
// push esi; mov esi,ecx; call 0x8621a0(this)
// ecx=[esi+4]; if ([ecx+0x10]==1) { (obj)->m(1, this); }

class CThingAICreature
{ public:
    char pad[0x10];
    char flag;      // +0x10
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

struct CModeHitByEpicSpell
{
    int   vslot;    // +0x00 (padding to reach +4)
    CThingAICreature*  sub;      // +0x04
    void  OnActivate();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x008621A0 ?OnActivate@CModeStandBase@@UAEXXZ (was ?BaseActivate@CModeHitByEpicSpell@@QAEXXZ)
class CModeStandBase { public: virtual void OnActivate(); };
void CModeHitByEpicSpell::OnActivate()
{
    reinterpret_cast<CModeStandBase*>(this)->CModeStandBase::OnActivate();
    CThingAICreature* s = this->sub;
    if (s->flag == 1)
        s->SuspendAI(1, this);
}