// CTCInventoryAbilities::GetPrioritisedAbilityDefs(std::vector<CDefPointer<const CHeroAbilityDef> >& defs) const -- for an adult
// hero, lists the received abilities' defs in order, keeps them up to the last received one and pads with null defs to a whole
// number of rows of assignable special-move buttons.
// Retail 0x005F4B30, 277 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Name: Ego_r.pdb + FableWin.pdb ?GetPrioritisedAbilityDefs@CTCInventoryAbilities@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCHeroAbilityDef
// @@@@V?$allocator@V?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@@std@@@Z (engine/CTCInventoryAbilities.h PRIORITISED).
// Callees: CThingCreatureBase::IsChild() const 0x00661F70 (Ego_r.pdb ?IsChild@CThingCreatureBase@@QBE_NXZ, on ParentThing),
// GetAbilityPDef(EHeroAbility) const 0x005EDFD9 (CDefPointer by value), vector<CDefPointer<const CHeroAbilityDef> >::push_back
// 0x005FA3A5, GetNumAssignableSpecialMoveButtons() const 0x005EDCF0, vector::operator= 0x005F9F5A, ~vector 0x005F9F14 (all out
// of line in this TU; STLport's push_back body is visible here and VC7.1 still calls it, and with the body visible it knows the
// null-def padding temporary is never captured, so that temporary's release folds away as in retail).
// Shape: one appended def per ReceivedAbilityOrder slot (the by-value def temporary is released inside its null test and zeroed),
// remembering the last non-empty slot; a local vector copies the defs up to it and is padded with null-def temporaries while its
// size is not a multiple of the row width (the bound re-read every pass), then assigned back.
// Layout (retail; Ego_r.pdb names): ParentThing +0x04, ReceivedAbilityOrder vector<EHeroAbility> +0x154.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryAbilities.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThingCreatureBase { public:
    bool IsChild() const;                            // 0x00661F70
};

void CTCInventoryAbilities::GetPrioritisedAbilityDefs(std::vector<CDefPointer<const CHeroAbilityDef> >& defs) const
{
    if (((const CThingCreatureBase*)ParentThing)->IsChild())
        return;
    long lastReceived = -1;
    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
        defs.push_back(GetAbilityPDef(ReceivedAbilityOrder[i]));
        if (ReceivedAbilityOrder[i] != HERO_ABILITY_NULL)
            lastReceived = i;
    }
    std::vector<CDefPointer<const CHeroAbilityDef> > prioritised;
    long j;
    for (j = 0; j <= lastReceived; ++j)
        prioritised.push_back(defs[j]);
    for (unsigned long k = 0; k < prioritised.size() % GetNumAssignableSpecialMoveButtons(); ++k)
        prioritised.push_back(CDefPointer<const CHeroAbilityDef>());
    defs = prioritised;
}
