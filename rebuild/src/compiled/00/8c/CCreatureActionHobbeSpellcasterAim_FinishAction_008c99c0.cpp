// CCreatureAction_HobbeSpellcasterAim::FinishAction() -- base FinishAction, then if the aimed spell thing is still alive kill it and clear the pointer
// Leaf: ?FinishAction@CCreatureAction_HobbeSpellcasterAim@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C99C0, length 119 bytes (+ int3 padding).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the spell
// pointer at +0x114, three times, then on the temporary), CThing::Kill @ 0x004C9B80 (one argument, 1),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (temporary, then the member),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: IS_THING_ALIVE inlined; `Kill(true)` pushes its argument before the chained GetPItem; the clear is the landed
// "assign through a stack CIntelligentPointer" shape (0x0084B1C0): default-construct (inlined CIntelligentPointer ctor stores the
// vtable 0x01238C6C after the base ctor), SetPItem(0), read it back into the member, destroy -- the 8-byte frame is that temporary.
// Retail CCombatActionBase is 0x114 bytes, so the spell pointer is this class's first member.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
#include "engine/CThing.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
// CIntelligentPointer = the shared CIntelligentPointer<CThing> (engine/CIntelligentPointer.h).
// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[0x114 - 4];
};
class CCreatureAction_HobbeSpellcasterAim : public CCombatActionBase { public:
    CBaseIntelligentPointer PSpell;         // +0x114 the spell thing being aimed
    virtual void FinishAction();
};

void CCreatureAction_HobbeSpellcasterAim::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PSpell)) {
        ((CThing*)PSpell.GetPItem())->Kill(true);
        CIntelligentPointer<CThing> none;
        none.SetPItem(0);
        PSpell.SetPItem(reinterpret_cast<CBaseObject*>((((CThing*)none.GetPItem()))));
    }
}