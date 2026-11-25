#ifndef FABLE_ENGINE_CSPECIALABILITIESUNHOLYPOWERDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESUNHOLYPOWERDEF_H
#ifdef FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_DTOR_DECL_ONLY
#if defined(FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_ALLOC) || defined(FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_RTTI_DECLS) || defined(FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_COPY_DECLS)
#error "FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_DTOR_DECL_ONLY cannot be combined with the full view's opt-ins"
#endif
// Add-only per-TU opt-in (NF451, 2026-11-25; row 0x004582BA, the scalar deleting destructor, which calls the out-of-line
// ~CSpecialAbilitiesUnholyPowerDef 0x004582D6): the class with its retail base and the destructor declared (Ego_r.pdb / FableWin.pdb
// ??1CSpecialAbilitiesUnholyPowerDef@@UAE@XZ; virtual through ~CParentDefClassBase), WITHOUT its members: the first member (+0x3C) is
// already an STLport vector and stlport/fable_containers.h adds /DEFAULTLIB:"libcp" to the object's .drectve, which the caller's object
// never had. Use it only to name the class or call the destructor (a qualified call); never for its layout. Declarations only.
#include "engine/CParentDefClassBase.h"
class CSpecialAbilitiesUnholyPowerDef : public CParentDefClassBase {
public:
    virtual ~CSpecialAbilitiesUnholyPowerDef();                          // 0x004582D6
};
#else
// New shared header (chunk 782, row 0x007941D0 CTCUnholyPower ctor): the retail CSpecialAbilitiesUnholyPowerDef, laid out as the
// sibling CSpecialAbilities*Def headers (CParentDefClassBase base, packed 0x39, the first member at +0x3C): the 22 Ego_r.pdb members in
// order and spelling (ghidra_out/struct_layouts_egor.tsv, size 312 there) with 12-byte STLport vectors (retail 0x100). The ctor reads
// StartPowerupSound +0x3C, LoopingPowerupSound +0x48, NumSecsBetweenDamageHit +0x88, DurationOfSpellSecs +0xA0, RadiusOfEffect +0xAC,
// EnvChangeSecs +0xB8, StaminaCostPerSec +0xDC, SmallRingRadius +0xF4 and LargeRingRadius +0xF8 (all matched by the row).
// Needs FABLE_RETAIL_CDEFSTRINGTABLE and FABLE_RETAIL_CCHARSTRING (engine/CDefString.h).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesUnholyPowerDef : public CParentDefClassBase {
public:
    std::vector<CDefString,std::allocator<CDefString> > StartPowerupSound;       // +0x3C
    std::vector<CDefString,std::allocator<CDefString> > LoopingPowerupSound;     // +0x48
    std::vector<CDefString,std::allocator<CDefString> > ReleaseSound;            // +0x54
    std::vector<CDefString,std::allocator<CDefString> > ReleaseLoopSound;        // +0x60
    std::vector<CDefString,std::allocator<CDefString> > FinishSound;             // +0x6C
    std::vector<CDefString,std::allocator<CDefString> > CancelSound;             // +0x78
    float RandomFactorForNumSecsBetweenDamageHit;                                  // +0x84
    std::vector<float,std::allocator<float> > NumSecsBetweenDamageHit;           // +0x88
    std::vector<float,std::allocator<float> > DamagePerHit;                      // +0x94
    std::vector<float,std::allocator<float> > DurationOfSpellSecs;               // +0xA0
    std::vector<float,std::allocator<float> > RadiusOfEffect;                    // +0xAC
    std::vector<float,std::allocator<float> > EnvChangeSecs;                     // +0xB8
    std::vector<float,std::allocator<float> > EnvReturnSecs;                     // +0xC4
    bool DoLifting;                                                              // +0xD0
    float VelocityOfLift;                                                        // +0xD4
    float DistanceToLift;                                                        // +0xD8
    std::vector<float,std::allocator<float> > StaminaCostPerSec;                 // +0xDC
    std::vector<float,std::allocator<float> > DamageTakenMultiplier;             // +0xE8
    float SmallRingRadius;                                                       // +0xF4
    float LargeRingRadius;                                                       // +0xF8
#ifdef FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CSpecialAbilitiesUnholyPowerDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004582B4: ?GetSizeofClass@CSpecialAbilitiesUnholyPowerDef@@UAEKXZ (Ego_r.pdb + FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
#ifdef FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_COPY_DECLS
    // Opt-in (NF386, 2026-11-24; NF337 form): declarations only, no layout change, for CSpecialAbilitiesUnholyPowerDef::Copy 0x004608C1, which tail-calls the
    // out-of-line operator= (retail `jmp` 0x004608C6). ?Copy@CSpecialAbilitiesUnholyPowerDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable
    // placement (ghidra_out/labels_rtti_port.tsv) names at 0x004608C1 (it overrides CDefClassBase::Copy: clang override probe,
    // made-up-name control fails); ??4CSpecialAbilitiesUnholyPowerDef@@QAEAAV0@ABV0@@Z (PDB name sets) is the landed operator= row at 0x004608C6.
public:
    virtual void Copy(const CDefClassBase*);
    CSpecialAbilitiesUnholyPowerDef& operator=(const CSpecialAbilitiesUnholyPowerDef&);
#endif
};
#endif // FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_DTOR_DECL_ONLY
#endif
