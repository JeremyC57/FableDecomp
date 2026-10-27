// CTCCombat::OnInitialActivate() -- on first activation of a creature's combat component, build its usable ability list
// Leaf: ?OnInitialActivate@CTCCombat@@UAEXXZ
// Retail: Fable.exe @ 0x00736AD0, 15 bytes: `mov eax,[ecx+4] ; cmp byte ptr [eax+10h],2 ; jne ; jmp 0x00736590 ; ret`
// (CTCCombat vtable 0x0126724C slot 6).
// Identity (probable): CTCBase vtable slot 6 is OnInitialActivate (GOTCHAS wave 25 chunk 160: the "vfunc_6" rows are
// CTCBase::OnInitialActivate); neither PDB name set lists CTCCombat's override. The callee is the landed
// CTCCombat::UpdateUsableMeleeCombatAbilities 0x00736590, entered with a tail jump.
// Layout: CTCBase::ParentThing +0x04; the thing's CClass::Type byte +0x10 (engine/CClass.h; 2 = creature, as the landed
// CTCCombat::SetBlocked 0x00734050 tests it).
// Header debt: engine/CTCCombat.h flattens CTCBase's vptr into a data member, so the virtual override needs this local
// polymorphic model (no members past ParentThing are read).
#define CClass DonorCClass
#include "engine/CClass.h"
#undef CClass
class CClass : public DonorCClass { public:          // engine/CClass.h: Type +0x10
    unsigned long GetType() const { return Type; }
};
class CThing : public CClass { public:
};
class CTCBase { public:                             // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnInitialActivate();               // slot 6
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCCombat : public CTCBase { public:
    void OnInitialActivate();
    void UpdateUsableMeleeCombatAbilities();        // 0x00736590
};

void CTCCombat::OnInitialActivate()
{
    if (ParentThing->GetType() == 2)
        UpdateUsableMeleeCombatAbilities();
}