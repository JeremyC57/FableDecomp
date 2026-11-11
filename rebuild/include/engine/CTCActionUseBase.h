#ifndef FABLE_ENGINE_CTCACTIONUSEBASE_H
#define FABLE_ENGINE_CTCACTIONUSEBASE_H
// CTCActionUseBase (new, chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the base of the
// action-use components (TCI_ON_ACTION_USE 13: FableWin.pdb ?StatGetInterfaceType@CTCActionUseBase@@SI?AW4ETCInterfaceType@@XZ).
// Ego_r.pdb (struct_layouts_egor.tsv, 20 bytes, no STL member): PActionUseDef CDefPointer<const CActionUseDef> +0x0C, Usable +0x10
// (the row tests `[eax+10h]`: an unusable action-use thing gets no "use" cursor).
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/ETCInterfaceType.h"
class CActionUseDef;
#pragma pack(push, 1)
class CTCActionUseBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ON_ACTION_USE; }
    bool IsUsable() const { return Usable; }                             // PLACEHOLDER accessor name (inline member read in the row)
#ifdef FABLE_RETAIL_CTCACTIONUSEBASE_ONCREATE
    // Retail opt-in (agentNF10, row 0x007EAC10): Ego_r.pdb / FableWin.pdb ?OnCreate@CTCActionUseBase@@UAEXXZ (CTCBase slot 4; the body
    // calls CTCBase::OnCreate 0x00686850, then GetDef<CThingBaseDef> 0x0042AF3C and GetSubDef<CActionUseDef> 0x007EAE40 into
    // PActionUseDef, `add esi,0Ch ; push esi`). Declaration only: no layout change.
    virtual void OnCreate();                                             // slot 4 0x007EAC10
#endif
private:
    CDefPointer<const CActionUseDef> PActionUseDef;                      // +0x0c
    bool Usable;                                                         // +0x10
    unsigned char _pad_0x11[3];                                          // +0x11
#ifdef FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual void OnActionUse(CThing&);  // slot 28, 0x007EAB90
public:
    virtual bool IsUsableWhileThingPerformingAction() const;  // slot 30, 0x004DA961
public:
    virtual bool IsToActuallyUseActionUseAsIntendedGoddammitWeReallyHaveGotOurselvesInAMess() const;  // slot 31, 0x004DA964
public:
#endif
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCActionUseBase) == 0x14);
#endif
