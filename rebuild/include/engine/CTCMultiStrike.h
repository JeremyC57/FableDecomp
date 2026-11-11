#ifndef FABLE_ENGINE_CTCMULTISTRIKE_H
#define FABLE_ENGINE_CTCMULTISTRIKE_H
// CTCMultiStrike (new shared header, chunk 1004, row 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier): the multi-strike
// spell component over FABLE_RETAIL_CTCBASE (Ego_r.pdb sizeof 0x5C: PDef +0x0C, SpellLevel +0x10, StrikeStarted +0x14 .. ParticleEffects
// +0x4C; members not modelled). StatGetInterfaceType: TCI_MULTI_STRIKE (135; the `mov edi,87h` of the caller's inline GetTC, 0x005CA6F3).
// Out-of-line methods the rows call (Ego_r.pdb + FableWin.pdb spellings): ?ApplyEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z
// (retail 0x007CCF50) and ?RemoveEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z (0x007CC9E0). Declarations only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
#ifdef FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
// Retail opt-in (chunk 1024, row 0x007CD390 the constructor ??0CTCMultiStrike@@QAE@AAVCThing@@@Z): the Ego_r.pdb members in order and
// spelling at the retail offsets (the ctor's stores: PDef +0x0C .. PHitLocation +0x20, the two CIntelligentPointer<CThing> +0x24 / +0x44
// built with vtable 0x01238C6C, the 12-byte STLport ParticleEffects +0x4C; retail sizeof 0x58, Ego_r 0x5C with its 16-byte vector).
// Members stay public (the landed rows reach them through local models). Needs FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesMultiStrikeDef;
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
namespace NHitLocations {class CHitLocation;}
#else
class CHitLocation;
#endif
#endif
class CTCMultiStrike : public CTCBase {
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:
bool IsStrikeStarted()const{return StrikeStarted;}
void StartMultiHit(const CHitParameters&,CThing*);
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_MULTI_STRIKE; }
    void ApplyEffectOnWeapon(CThing* weapon);                // 0x007CCF50
    void RemoveEffectOnWeapon(CThing* weapon);               // 0x007CC9E0
#ifdef FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
    CTCMultiStrike(CThing& thing);                           // 0x007CD390
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
private:
#endif
    CDefPointer<const CSpecialAbilitiesMultiStrikeDef> PDef;                 // +0x0C
    long SpellLevel;                                                         // +0x10
    bool StrikeStarted;                                                      // +0x14
    long DelayTimer;                                                         // +0x18
    long NumHits;                                                            // +0x1C
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
    NHitLocations::CHitLocation* PHitLocation;
#else
    CHitLocation* PHitLocation;
#endif                                              // +0x20
    CIntelligentPointer<CThing> PWeapon;                                     // +0x24
    C3DVector PointOfHit;                                                    // +0x2C
    C3DVector HitDirection;                                                  // +0x38
    CIntelligentPointer<CThing> PTarget;                                     // +0x44
    std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > ParticleEffects;   // +0x4C
#endif
#ifdef FABLE_RETAIL_CTCMULTISTRIKE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007CD800
public:
#endif
#ifdef FABLE_RETAIL_CTCMULTISTRIKE_VSLOTS2
// agentFAM04 (2026-11-11): retail virtuals and the non-virtual they call (Ego_r.pdb / FableWin.pdb spellings; TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
public:
    virtual void OnKill();                                               // slot 14, 0x007CCE00 (tail call to FinalCleanup)
private:
    void FinalCleanup();                                                 // 0x007CCD70 ?FinalCleanup@CTCMultiStrike@@AAEXXZ
public:
#endif // FABLE_RETAIL_CTCMULTISTRIKE_VSLOTS2
};
#endif // FABLE_ENGINE_CTCMULTISTRIKE_H
