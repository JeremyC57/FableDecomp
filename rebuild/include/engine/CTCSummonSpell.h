#ifndef FABLE_ENGINE_CTCSUMMONSPELL_H
#define FABLE_ENGINE_CTCSUMMONSPELL_H
// NEW (chunk 1008b, row 0x005EE49A CTCInventoryAbilities::IsAbleToUseAbility). Without the opt-in the header only names the class.
#ifdef FABLE_RETAIL_CTCSUMMONSPELL
// Retail opt-in: RTTI .?AVCTCSummonSpell@@ : CTCBase (needs FABLE_RETAIL_CTCBASE, forced below). Members:
// FableWin.pdb ?StatGetInterfaceType@CTCSummonSpell@@SI?AW4ETCInterfaceType@@XZ (inline in retail: IsAbleToUseAbility looks up
// TC 145 = TCI_SUMMON_SPELL, `mov ebx,91h` at 0x005EE66A) and Ego_r.pdb / FableWin.pdb ?GetResetCreatureManaCost@CTCSummonSpell@@QBEJXZ
// (the class's only QBEJXZ member in both name sets), out of line in retail at 0x0079A610 between FinalCleanup 0x0079A600 and the
// ctor 0x0079A620: `mov eax,[ecx+0Ch] ; mov eax,[eax+54h] ; ret` = PDef (Ego_r +0x0C CDefPointer<const CSpecialAbilitiesSummonSpellDef>)
// -> ResetCreatureManaCost (Ego_r +0x5C; retail +0x54: CParentDefClassBase 0x3C, TimeOfSummon a 12-byte STLport vector). The data
// members are not modelled (no row reads them), so the class is used through pointers only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
#include "engine/C3DVector.h"
class CWorldMap;
#endif
#ifdef FABLE_RETAIL_CTCSUMMONSPELL_MINION
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#endif
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CSpecialAbilitiesSummonSpellDef; class CThingAICreature;
#endif
class CTCSummonSpell : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_SUMMON_SPELL; }
    long GetResetCreatureManaCost() const;                               // 0x0079A610
#ifdef FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
    // Retail opt-in (chunk 1021, row 0x0079AA90): Ego_r.pdb ?IsValidTeleportPos@CTCSummonSpell@@AAE_NABVC3DVector@@ABVCWorldMap@@PBVCThing@@@Z
    // (Ego_r 0x007A0360, retail 0x0079A890) and ?GetValidTeleportPos@CTCSummonSpell@@AAE?AVC3DVector@@PBVCThing@@@Z (Ego_r 0x007A05C0,
    // retail 0x0079AA90; called by CreateMinion 0x0079ACC0), both private and out of line. Declarations only, no layout change.
private:
    bool IsValidTeleportPos(const C3DVector& pos, const CWorldMap& world_map, const CThing* thing);   // 0x0079A890
    C3DVector GetValidTeleportPos(const CThing* thing);                                              // 0x0079AA90
public:
#endif
#ifdef FABLE_RETAIL_CTCSUMMONSPELL_RESETMINIONPOS
    // Q19 opt-in (row 0x0079B6F0): Ego_r.pdb ?ResetMinionPos@CTCSummonSpell@@QAEXXZ (public, out of line). Declaration only.
    void ResetMinionPos();                                                                           // 0x0079B6F0
#endif
#ifdef FABLE_RETAIL_CTCSUMMONSPELL_MINION
    // Retail opt-in (chunk 1012, row 0x008A8260 CGameScriptInterface::GetHeroSummonedCreaturesList): Ego_r.pdb / FableWin.pdb
    // ?GetSummonedCreature@CTCSummonSpell@@QBE?AV?$CIntelligentPointer@VCThingAICreature@@@@XZ, inline in retail: the caller's local is
    // copy-constructed in place from the member (0x008A82CE base ctor 0x00A01B10, the vptr 0x0123E7E0, GetPItem on [comp+11h], SetPItem;
    // needs FABLE_RETAIL_CINTELLIGENTPOINTER_COPY). Members from Ego_r.pdb CTCSummonSpell (PDef +0x0C, JustReleased +0x10, Minion +0x11:
    // CBaseIntelligentPointer is pack(1)); retail agrees at +0x0C (GetResetCreatureManaCost `mov eax,[ecx+0Ch]`) and +0x11 (0x008A82D3
    // `lea ecx,[edi+11h]`). The members after Minion are not modelled (pointer use only).
    CIntelligentPointer<CThingAICreature> GetSummonedCreature() const { return Minion; }
private:
    CDefPointer<const CSpecialAbilitiesSummonSpellDef> PDef;             // +0x0c
    bool                                               JustReleased;     // +0x10
    CIntelligentPointer<CThingAICreature>              Minion;           // +0x11
#endif
#ifdef FABLE_RETAIL_CTCSUMMONSPELL_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0079BD00
public:
    virtual void OnKill();  // slot 14, 0x0079A5F0
public:
#endif
};
#else
class CTCSummonSpell;
#endif
#endif // FABLE_ENGINE_CTCSUMMONSPELL_H
