#ifndef FABLE_ENGINE_CSPECIALABILITIESFIREBALLSPELLDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESFIREBALLSPELLDEF_H
// NEW shared header (chunk 1008b, row 0x005EE49A CTCInventoryAbilities::IsAbleToUseAbility): the retail
// CSpecialAbilitiesFireballSpellDef, laid out as the sibling CSpecialAbilities*Def headers (CParentDefClassBase base, packed 0x39, the
// first member at +0x3C): the 4 Ego_r.pdb members in order and spelling (ghidra_out/struct_layouts_egor.tsv, size 92 there) with a
// 12-byte STLport vector (retail 0x58). The row reads StaminaCost's start at +0x44 (`mov eax,[ecx+44h] ; fcomp dword ptr [eax]`,
// 0x005EE54E) under the def name "SPECIAL_ABILITIES_FIREBALL_SPELL_DEF".
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CSPECIALABILITIESFIREBALLSPELLDEF_MEMBERS
class CPersistContext;
#endif
class CSpecialAbilitiesFireballSpellDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CSPECIALABILITIESFIREBALLSPELLDEF_MEMBERS
    // Retail opt-in (agentNF4, 2026-11-10, row 0x00457B35): the out-of-line constructor 0x00457B35 (Ego_r.pdb / FableWin.pdb
    // ??0CSpecialAbilitiesFireballSpellDef@@QAE@XZ: CParentDefClassBase() 0x00430370, vtable 0x012345A4, the empty StaminaCost vector),
    // and the def's own overrides (both PDBs ?Transfer@, ?Copy@,
    // ?GetSizeofClass@ and the static ?Alloc@..@@SIPAVCDefClassBase@@XZ). Declarations only. Needs FABLE_RETAIL_DEF_CLASS_BASE.
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSpecialAbilitiesFireballSpellDef();                                         // 0x00457B35
    virtual void Transfer(CPersistContext&);                                     // slot 18
    virtual void Copy(const CDefClassBase*);                                     // slot 19
    virtual unsigned long GetSizeofClass();                                      // slot 20
#endif
    float ReleaseDelay;                                                          // +0x3C
    float Inclination;                                                           // +0x40
    std::vector<float,std::allocator<float> > StaminaCost;                       // +0x44
    float TargettingArcAngle;                                                    // +0x50
};
#endif
