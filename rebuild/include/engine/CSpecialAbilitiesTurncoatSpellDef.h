#ifndef FABLE_ENGINE_CSPECIALABILITIESTURNCOATSPELLDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESTURNCOATSPELLDEF_H
// NEW shared header (w22 agent2027, row 0x007C6E50 CTCTurncoatSpell::FrameUpdate): the retail CSpecialAbilitiesTurncoatSpellDef, laid
// out as the sibling CSpecialAbilities*Def headers (CParentDefClassBase base, the first member at +0x3C): the 5 Ego_r.pdb members in
// order and spelling (ghidra_out/struct_layouts_egor.tsv, 0x90 there with 16-byte vectors and a 0x40 base) with 12-byte STLport
// vectors: StaminaCostPerSec +0x3C, ResistanceDamagePerSec +0x48, TurncoatTime +0x54, TurncoatRange +0x60, TurncoatXPUpEveryNSecs
// +0x6C (sizeof 0x78). Retail evidence: FrameUpdate reads TurncoatRange's start at +0x60 (`mov ecx,[eax+60h] ; fld dword ptr
// [ecx+edx*4]` with edx = LevelOfSpell, 0x007C6F7E / 0x007C6F9C); the landed SetPTarget 0x007C6940 reads TurncoatTime at +0x54.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesTurncoatSpellDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_TRANSFER
// Opt-in (NF40, 2026-11-16, row 0x00465662 CSpecialAbilitiesTurncoatSpellDef::Transfer): Ego_r.pdb / FableWin.pdb ?Transfer@CSpecialAbilitiesTurncoatSpellDef@@UAEXAAVCPersistContext@@@Z, the def's
// override of CDefClassBase::Transfer (virtual slot 18). Declaration only (the elaborated class needs no include); no layout change.
public: virtual void Transfer(class CPersistContext&);
public:
#endif
    std::vector<float,std::allocator<float> > StaminaCostPerSec;                 // +0x3C
    std::vector<float,std::allocator<float> > ResistanceDamagePerSec;            // +0x48
    std::vector<float,std::allocator<float> > TurncoatTime;                      // +0x54
    std::vector<float,std::allocator<float> > TurncoatRange;                     // +0x60
    std::vector<float,std::allocator<float> > TurncoatXPUpEveryNSecs;            // +0x6C
#ifdef FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CSpecialAbilitiesTurncoatSpellDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x00457AC4: ?GetSizeofClass@CSpecialAbilitiesTurncoatSpellDef@@UAEKXZ (Ego_r.pdb + FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
#endif
