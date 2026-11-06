#ifndef FABLE_ENGINE_NUISYSTEM_CSPELLCONTAINER_H
#define FABLE_ENGINE_NUISYSTEM_CSPELLCONTAINER_H
// NUISystem::CSpellContainer (new, chunk 765): RTTI .?AVCSpellContainer@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124786C (+0, slot 0 the ??_G 0x005460A0), 0x01247848 (+4, CObserver) and 0x01247840 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CSpellContainer@NUISystem@@QAE@J@Z, ??0CSpellContainer@NUISystem@@QAE@ABV01@@Z, ??1CSpellContainer@NUISystem@@UAE@XZ.
// The destructor 0x00545840 removes the observer from CManager, then the compiler destroys the BaseSpells vector.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CSpellContainer : public CChangingStateComponent {
public:
    CSpellContainer(long);
    CSpellContainer(const CSpellContainer&);
    virtual ~CSpellContainer();
    void GetDataFromDef();
#ifdef FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS
    // Retail opt-in (chunk 800, row 0x00544D20 CSpellContainerList::ProcessEvent): the new virtuals of the primary vtable
    // 0x0124786C (rebuild/manifest/rtti-vtables.tsv slots 142..148), names Ego_r.pdb / FableWin.pdb ?..@CSpellContainer@NUISystem@@
    // (AddChildDynamically UAEXAAV?$CCountedPointer..@K@Z, AssignSpell UAEJ..K@Z, UnAssignSpell UAEXK@Z, SetEmptySlotDef UAEXK@Z,
    // AcceptAssignments UAEXXZ, AddChildAndSetOffsets UAEXAAV?$CCountedPointer..@Z, GetBaseSpells UAEAAV?$vector..XZ); slot 145 is the
    // only remaining void(unsigned long) name (its body stores EmptySlot +0x170). Declarations only.
    virtual void AddChildDynamically(CCountedPointer<CComponent>&, unsigned long);               // slot 142 0x00545F00
    virtual long AssignSpell(CCountedPointer<CComponent>&, unsigned long);                       // slot 143 0x005458A0
    virtual void UnAssignSpell(unsigned long);                                                    // slot 144 0x00545B10
    virtual void SetEmptySlotDef(unsigned long);                                                  // slot 145 0x00546060
    virtual void AcceptAssignments();                                                             // slot 146 0x00545670
    virtual void AddChildAndSetOffsets(CCountedPointer<CComponent>&);                            // slot 147 0x00545DF0
    virtual std::vector<CCountedPointer<CComponent> >& GetBaseSpells();                          // slot 148 0x00546070
#endif
};
}
#endif
