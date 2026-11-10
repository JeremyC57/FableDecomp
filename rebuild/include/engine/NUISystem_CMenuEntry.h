#ifndef FABLE_ENGINE_NUISYSTEM_CMENUENTRY_H
#define FABLE_ENGINE_NUISYSTEM_CMENUENTRY_H
// NUISystem::CMenuEntry (new, chunk 765): RTTI .?AVCMenuEntry@NUISystem@@, single direct base NUISystem::CClickable at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124662C (+0, slot 0 the ??_G 0x0053E8E0), 0x01246604 (+4, CObserver) and 0x012465FC (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CMenuEntry@NUISystem@@QAE@J@Z, ??0CMenuEntry@NUISystem@@QAE@ABV01@@Z, ??1CMenuEntry@NUISystem@@UAE@XZ.
// Ego_r.pdb members (492 bytes) LastInputTime +452, IsAvailable +456, PUIActionParams +460, PThingActionParams +464, PActions +468,
// PActionsOnSelected +472, PActionsOnBack +476, PPreActions +480, PActionsOnUnselected +484, PActionsOnVisible +488 sit 0x30 lower
// in retail (on the 0x194-byte retail CClickable): the constructor 0x0053DFE0 sets +0x198 to 1 and clears +0x19C..+0x1B8, then
// calls 0x0053DA30 (GetDataFromDef: the PDB name set has ?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ and every sibling
// component constructor ends with its GetDataFromDef; the catalog's "InitializeRumbleActions" is in neither PDB); sizeof 0x1BC.
// Overrides are not declared (no row calls them).
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CMenuEntry : public CClickable {
public:
    CMenuEntry(long);
    CMenuEntry(const CMenuEntry&);
    virtual ~CMenuEntry();
#ifdef FABLE_RETAIL_CMENUENTRY_UPDATE
    // Original PDB and native primary vtable slot1,0053d4b0.
    virtual void Update(float tick);
#endif
    void GetDataFromDef();                                          // 0x0053DA30
#ifdef FABLE_RETAIL_CMENUENTRY_ACTION
    // Retail opt-in (chunk 886, row 0x0041E6D3 CManager::Action): FableWin.pdb ?IsMenuEntryAvailable@CMenuEntry@NUISystem@@QBE_NXZ,
    // inline in retail (LOAD_WEAPON_MENU_ENTRY_NAME 0x004242A8 tests +0x198 in place), and ?GSetForceProcessing@CMenuEntry@NUISystem@@SIX_N@Z,
    // inline in retail (WIELD 0x0041E7A6 / SCROLL_PHOTO_UP 0x00426571 store the byte 0x013B8AA8 in place). The static's own name is
    // not in the name sets (GForceProcessing is descriptive). No layout change.
    bool IsMenuEntryAvailable() const { return IsAvailable; }
    static void FABLE_FASTCALL GSetForceProcessing(bool force) { GForceProcessing = force; }
    static bool GForceProcessing;                                   // 0x013B8AA8
#endif
#ifdef FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
    // Retail opt-in (chunk 890, row 0x0056AD40 CTCBoastUI::Activate): the first two virtuals CMenuEntry introduces after CClickable's
    // last slot (149 OnRightUnclicked), in vtable order (rtti-vtables CMenuEntry 0x0124662C: slot 150 0x0053DCD0, 151 0x0053DD50;
    // labels: AddThingActionParam / AddUIActionParam): Ego_r.pdb / FableWin.pdb ?AddThingActionParam@CMenuEntry@NUISystem@@UAEXJ@Z and
    // ?AddUIActionParam@CMenuEntry@NUISystem@@UAEXPAVCActionParam@2@@Z. Activate calls slot 151 as `call [edx+25Ch]` with the entry's
    // own CActionParam base (+0x18, null-checked). Declarations only: no layout change.
    virtual void AddThingActionParam(long param);                   // 150 0x0053DCD0
    virtual void AddUIActionParam(CActionParam* param);             // 151 0x0053DD50
#ifdef FABLE_RETAIL_CMENUENTRY_ACTIONLISTS
    // Nested opt-in (chunk 1002b, rows 0x005C3E91 / 0x005C47D5 CTCInventoryWeapons::FinalConstructWeaponItemEntries(PC)): the next two
    // new virtuals (rtti-vtables CMenuEntry 0x0124662C: slot 152 0x0053E2A0 appends to PActionsOnUnselected +0x1B4, slot 153
    // 0x0053E3C0 to PActionsOnVisible +0x1B8): Ego_r.pdb / FableWin.pdb
    // ?AddActionOnUnselected@CMenuEntry@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z and
    // ?AddActionOnVisible@CMenuEntry@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z; the rows call them as
    // `call [eax+260h]` / `call [eax+264h]`. Declarations only: no layout change.
    virtual void AddActionOnUnselected(CCountedPointer<CAction>& action);   // 152 0x0053E2A0
    virtual void AddActionOnVisible(CCountedPointer<CAction>& action);      // 153 0x0053E3C0
#endif
#ifdef FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
    // Retail opt-in (chunk 1003, row 0x005B6A66 CTCInventoryClothing::ConstructClothingListPC): the next two virtuals CMenuEntry
    // introduces, in vtable order (CMenuEntry 0x0124662C: slot 152 0x0053E2A0, 153 0x0053E3C0; labels AddActionOnUnselected /
    // AddActionOnVisible), Ego_r.pdb ?AddActionOnUnselected@CMenuEntry@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
    // (donor 0x005B7750) and ?AddActionOnVisible@CMenuEntry@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z (0x005B78D0),
    // the two new members in the PDB order AddAction / AddActionOnSelected / AddActionOnUnselected / AddActionOnBack /
    // AddActionOnVisible / AddPreAction (the base-class ones are overrides). The clothing lists call them as `call [eax+260h]` /
    // `call [eax+264h]` (0x005B6E6F / 0x005B6F02). Declarations only: no layout change. Needs FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS.
#ifndef FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#error "FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE needs FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS"
#endif
    virtual void AddActionOnUnselected(CCountedPointer<CAction>& action);   // 152 0x0053E2A0
    virtual void AddActionOnVisible(CCountedPointer<CAction>& action);      // 153 0x0053E3C0
#endif
#endif
protected:
    float LastInputTime;                                            // +0x194
    bool IsAvailable;                                               // +0x198
    std::list<CActionParam*>* PUIActionParams;                      // +0x19c
    std::list<long>* PThingActionParams;                            // +0x1a0
    std::list<CCountedPointer<CAction> >* PActions;                 // +0x1a4
    std::list<CCountedPointer<CAction> >* PActionsOnSelected;       // +0x1a8
    std::list<CCountedPointer<CAction> >* PActionsOnBack;           // +0x1ac
    std::list<CCountedPointer<CAction> >* PPreActions;              // +0x1b0
    std::list<CCountedPointer<CAction> >* PActionsOnUnselected;     // +0x1b4
    std::list<CCountedPointer<CAction> >* PActionsOnVisible;        // +0x1b8
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CMenuEntry) == 0x1bc);
#endif
