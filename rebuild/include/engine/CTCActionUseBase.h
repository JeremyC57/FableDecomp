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
#ifdef FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
// agentFAM05 (2026-11-11): the return type of GetWhenToActivate (mangled W4EWhenToActivate@@, a global enum; the enumerators are not in the name
// sets, so the rows write the retail values as casts).
enum EWhenToActivate { };
#endif // FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
#pragma pack(push, 1)
class CTCActionUseBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ON_ACTION_USE; }
    bool IsUsable() const { return Usable; }                             // PLACEHOLDER accessor name (inline member read in the row)
#ifdef FABLE_RETAIL_CTCACTIONUSEBASE_ANIMGROUPNAME
    // Retail opt-in (wf7, row 0x00847DF0 CCreatureAction_UseLamp::Initialise): Ego_r.pdb / FableWin.pdb
    // ?GetActionUseAnimationGroupName@CTCActionUseBase@@QBE?AVCCharString@@XZ (public const, by-value CCharString), out of line in
    // retail at 0x007EABA0 (landed). Declaration only; needs fable_string.h (FABLE_RETAIL_CCHARSTRING) first.
    CCharString GetActionUseAnimationGroupName() const;                  // 0x007EABA0
#endif
#ifdef FABLE_RETAIL_CTCACTIONUSEBASE_DTOR
    // Retail opt-in (agentNF20, 2026-11-11, the CTCActionUse* / CTCCarriedActionUse* scalar deleting destructors 0x004DBF76..0x004DCF8B):
    // Ego_r.pdb ??0CTCActionUseBase@@QAE@AAVCThing@@@Z and FableWin.pdb ??1CTCActionUseBase@@UAE@XZ. The destructor is 0x004DA967 (it
    // releases PActionUseDef +0x0C, then tail-jumps ~CTCBase 0x00686830); every derived class's implicit destructor reaches it directly.
    // Declarations only: no layout change.
    CTCActionUseBase(CThing& thing);
    virtual ~CTCActionUseBase();                                         // slot 0 0x004DA967
#endif
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
#ifdef FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtuals this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC vtable
// 0x0123F09C (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
public:
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21, 0x004DA959
public:
    virtual EWhenToActivate GetWhenToActivate() const;  // slot 29, 0x004DA95D
public:
#endif // FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCActionUseBase) == 0x14);
#endif
