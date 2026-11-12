#ifndef FABLE_ENGINE_NUISYSTEM_CDYNAMICLIST_H
#define FABLE_ENGINE_NUISYSTEM_CDYNAMICLIST_H
// NUISystem::CDynamicList (new, chunk 765): RTTI .?AVCDynamicList@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01246E44 (+0, slot 0 the ??_G 0x00541870), 0x01246E20 (+4, CObserver) and 0x01246E18 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CDynamicList@NUISystem@@QAE@J@Z, ??0CDynamicList@NUISystem@@QAE@ABV01@@Z, ??1CDynamicList@NUISystem@@UAE@XZ.
// The destructor 0x00541350 stores the three vptrs, frees the 12-byte STLport vector at +0x174 and calls
// ~CChangingStateComponent 0x0052CCF0; its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
#ifdef FABLE_RETAIL_CDYNAMICLIST_MEMBERS
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#endif
namespace NUISystem {
class CDynamicList : public CChangingStateComponent {
public:
    CDynamicList(long);
    CDynamicList(const CDynamicList&);
    virtual ~CDynamicList();
protected:
    void GetDataFromDef();                                          // ?GetDataFromDef@CDynamicList@NUISystem@@IAEXXZ
#ifdef FABLE_RETAIL_CDYNAMICLIST_VSLOTS
// agentFAM04 (2026-11-11): retail virtuals of vtable 0x01246E44 that the declarations above lack (Ego_r / FableWin
// ?GetSelectedIndex@CDynamicList@NUISystem@@UAEKXZ slot 143, ?GetScrollingChildren@CDynamicList@NUISystem@@UAEAAV?$vector@PAVCComponent@NUISystem@@...@XZ
// slot 149; slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots, agentFAM03). Declarations only.
public:
    virtual unsigned long GetSelectedIndex();                       // slot 143, 0x00540E30
    virtual std::vector<CComponent*>& GetScrollingChildren();       // slot 149, 0x00540E40
#endif // FABLE_RETAIL_CDYNAMICLIST_VSLOTS
#ifdef FABLE_RETAIL_CDYNAMICLIST_MEMBERS
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 0x180 ; call operator new` before the
    // constructor 0x005415F0): the Ego_r.pdb data members (struct_layouts_egor.tsv) 0x30 lower in retail (STLport 4.6 container sizes),
    // so sizeof is retail's 0x180. Members only: no row reads them yet.
protected:
    int BInitialised;                            // +0x15c (Ego_r +396)
    C2DVector PositionOffset;                    // +0x160
    long TopIndex;                               // +0x168
    long SelectionIndex;                         // +0x16c
    long NumVisible;                             // +0x170
    std::vector<CComponent*> ScrollingChildren;  // +0x174 (STLport 12 bytes; Ego_r +420)
#endif
};
}
#ifdef FABLE_RETAIL_CDYNAMICLIST_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NUISystem::CDynamicList) == 0x180);
#endif
#endif
