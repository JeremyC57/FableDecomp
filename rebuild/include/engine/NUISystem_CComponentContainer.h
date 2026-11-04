#ifndef FABLE_ENGINE_NUISYSTEM_CCOMPONENTCONTAINER_H
#define FABLE_ENGINE_NUISYSTEM_CCOMPONENTCONTAINER_H
#ifndef FABLE_RETAIL_CCOMPONENTCONTAINER
#error "engine/NUISystem_CComponentContainer.h is a retail model: define FABLE_RETAIL_CCOMPONENTCONTAINER"
#endif
// Retail model (new, chunk 735, rows 0x00570332 / 0x00571280 CTCInventoryExperience::AddAbilityToList(PC)): the PDB class
// NUISystem::CComponentContainer (RTTI .?AVCComponentContainer@NUISystem@@, single base NUISystem::CChangingStateComponent at
// +0, rtti_bases.tsv; retail vtable 0x01247AF4 adds slots 142 = 0x00546140 and 143 = 0x00546320 after the base's 141). Its two
// Ego_r.pdb virtuals are the AddTextLine overloads (Ego_r 0x005BFDC0 / 0x005BFFD0); VC7.1 lays overloads out in reverse
// declaration order, and the rows call slot 142 (+0x238) with a CCharString argument and slot 143 (+0x23C) with an
// unsigned long, so the (CCharString) overload is declared second. Data members are not modelled (Ego_r has no layout record
// for the class; struct_layouts_egor.tsv), hence no sizeof assertion.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CRGBColour.h"
namespace NUISystem {
class CComponentContainer : public CChangingStateComponent {
public:
#ifdef FABLE_RETAIL_CCOMPONENTCONTAINER_CTORS
    // Retail opt-in (chunk 765): the PDB constructors ??0CComponentContainer@NUISystem@@QAE@J@Z (retail 0x005460C0: the base
    // call and the three vptrs) and ??0..QAE@ABV01@@Z, so the constructor's TU can be written (it emits the vtables, the ??_G
    // 0x005465D0 and its two adjustor thunks 0x005465B0 / 0x005465C0). The destructor stays implicit (the ??_G inlines it).
    CComponentContainer(long);
    CComponentContainer(const CComponentContainer&);
#endif
    virtual void AddTextLine(const CWideString&, CRGBColour, unsigned long, long);    // slot 143
    virtual void AddTextLine(const CWideString&, CRGBColour, CCharString, long);      // slot 142
};
}
#endif
