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
    void  BaseActivate();
    void  OnActivate();
};

void CModeHitByEpicSpell::OnActivate()
{
    BaseActivate();
    CThingAICreature* s = this->sub;
    if (s->flag == 1)
        s->SuspendAI(1, this);
}