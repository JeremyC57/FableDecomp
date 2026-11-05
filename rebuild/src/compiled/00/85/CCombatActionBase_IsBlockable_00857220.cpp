// CCombatActionBase::IsBlockable() const -- false when the creature's flag 0x10 (+0x30) is set, else true only when the action has no melee ability
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857220, length 34 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8).
// PMeleeAbilityDef (+0xb8, Ego_r.pdb name) is tested as the raw pointer (`sete al`), no counted-pointer copy
// (compare the by-value getter of 0x00856EA0). Protected virtual (`MBE` in the decorated name); bool result:
// `xor al,al` false path, `test eax,eax ; sete al` for the named bool (a direct `return p == 0;` zeroes eax
// first and is 2 bytes longer). Layout modelled locally (retail base sizeof 0x114).
class CMeleeCombatAbilityDef;

class CBaseObject { public:              // engine/CThingCreatureBase.h exists but is not used: only the flags dword at +0x30 is read (retail offset)
    unsigned char _pad[0x30];
    unsigned long Flags30;                      // +0x30 (bit 0x10: cannot be blocked)
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xb8 - 0x10];          // +0x10
    CMeleeCombatAbilityDef* PMeleeAbilityDef;   // +0xb8 (CCountedPointer, raw pointer tested)
    unsigned char _padbc[0x114 - 0xbc];         // +0xbc
    bool IsBlockable() const;
};

bool CCombatActionBase::IsBlockable() const
{
    if (PCreature.GetPItem()->Flags30 & 0x10)
        return false;
    bool blockable = (PMeleeAbilityDef == 0);     // named bool: `test eax,eax ; sete al` without the eax zeroing
    return blockable;
}