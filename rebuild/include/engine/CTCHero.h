#ifndef FABLE_ENGINE_CTCHERO_H
#define FABLE_ENGINE_CTCHERO_H
// CTCHero (new, chunk 765): RTTI .?AVCTCHero@@, base CTCBase at +0; retail vtable 0x0125EB54. Ego_r.pdb members
// (struct_layouts_egor.tsv, sizeof 0x38): WeaponsUsable +0x0C .. WillUseOverriddenByScripts +0x0F, Title +0x10,
// HeroTitleObjectDef +0x14, PHeroTitleSubDef +0x18, OnBoastingPodium +0x1C, LastWeaponEquipped +0x20,
// LastWeaponEquippedID +0x28, PTeleportParticle +0x30. Only the methods rows use are modelled (no members yet).
// Ego_r.pdb order: ?ProcessPendingTutorials@CTCHero@@QAEXXZ 0x00700980 then ?FrameUpdate@CTCHero@@UAEXXZ 0x00700990;
// retail 0x006C9FC0 / 0x006C9FD0 (vtable slot 9) carry the same `call Get ; mov ecx,eax ; jmp` body.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#ifdef FABLE_RETAIL_CTCHERO_TITLE
#include <stddef.h>
#include "engine/ETCInterfaceType.h"
#endif
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCHERO_TUTORIAL
#include "engine/fable_nscript_forward.h"
#endif
class CTCHero : public CTCBase {
public:
    void ProcessPendingTutorials();                                      // 0x006C9FC0
    virtual void FrameUpdate();                                          // slot 9 0x006C9FD0
#ifdef FABLE_RETAIL_CTCHERO_TUTORIAL
    // Opt-in (chunk 1010, row 0x00896390 CGameScriptInterface::OpenChest): FableWin.pdb ?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z,
    // out of line in retail at 0x006C9FA0 (`push 0 ; push 21h ; mov ecx,eax ; call 0x006C9FA0`). The enum comes from
    // engine/fable_nscript_forward.h (included here). Declaration only: no layout change.
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
#endif
#ifdef FABLE_RETAIL_CTCHERO_TITLE
    // Retail opt-in (chunk 867, row 0x005AF64E CTCInventoryTrade::BuyItem): the hero component as the trade screen reaches it.
    // Ego_r.pdb ?StatGetInterfaceType@CTCHero@@SI?AW4ETCInterfaceType@@XZ (inline: TCI_HERO = 41, the `push 29h` of the inline
    // CThing::GetTC), ?SetTitle@CTCHero@@QAEXJ@Z 0x006CA400 (FableWin.pdb 0x024742B0; the bought title's def index), and the
    // Ego_r.pdb members at their retail offsets (struct_layouts_egor.tsv, sizeof 0x38): the row reads WeaponsUsable +0x0C
    // (`cmp byte ptr [eax+0Ch],0`) to raise the weapon-unusable warning after buying a weapon. Only the first word of members is
    // typed; the rest keep opaque storage (Title +0x10 EHeroTitle, HeroTitleObjectDef +0x14, PHeroTitleSubDef +0x18,
    // OnBoastingPodium +0x1C, LastWeaponEquipped +0x1D.., LastWeaponEquippedID +0x28, PTeleportParticle +0x30).
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO; }
    void SetTitle(long titleDefIndex);                                   // 0x006CA400
    bool WeaponsUsable;                                                  // +0x0c
    bool WillUsable;                                                     // +0x0d
    bool WeaponUseOverriddenByScripts;                                   // +0x0e
    bool WillUseOverriddenByScripts;                                     // +0x0f
    unsigned char _tail[0x28];                                           // +0x10 Title .. PTeleportParticle (opaque)
#ifdef FABLE_RETAIL_CTCHERO_WEAPONUSE
    // Nested opt-in (chunk 1004, row 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier): FableWin.pdb
    // ?IsWeaponUseAllowed@CTCHero@@QBE_NXZ (a 5-byte body there), inline in retail (`cmp byte ptr [eax+0Ch],0`, 0x005CA6D5).
    bool IsWeaponUseAllowed() const { return WeaponsUsable; }
#endif
#endif
#ifdef FABLE_RETAIL_CTCHERO_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x006CA720
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x006CA730
public:
#endif
};
#ifdef FABLE_RETAIL_CTCHERO_TITLE
FABLE_STATIC_ASSERT(sizeof(CTCHero) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCHero, WeaponsUsable) == 0x0c);
#endif
#endif
