#ifndef FABLE_ENGINE_CTCSPECIALABILITIES_H
#define FABLE_ENGINE_CTCSPECIALABILITIES_H
// New (chunk 1012, row 0x0062BEF0 CWorld::EAUseProjectileWeapon): the hero special-abilities component, interface TCI_SPECIAL_ABILITIES
// (167; RTTI .?AVCTCSpecialAbilities@@ : CTCBase). Ego_r.pdb CTCSpecialAbilities (0x38 bytes there): AbilityMutexLock +0x0C, UsingBed
// +0x0D, CurrentAbility +0x10, UndeadFollowers (STLport list, 4 bytes in retail) +0x14, Creature ...; only the first byte is read by
// the rows that use this header, through FableWin.pdb's inline ?GetAbilityMutexLock@CTCSpecialAbilities@@QBE_NXZ (retail
// `mov cl,[eax+0Ch] ; test cl,cl`, 0x0062C001). The rest of the class stays unmodelled. Needs FABLE_RETAIL_CTCBASE.
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_FULL
// Full-layout opt-in (Codex pass 17, row 0x0078C150 CTCBerserk::PlayHitEffectAtPos): the retail 0x28-byte layout (STLport list 4 and
// vector<bool> sized members) and the public static ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z.
#include "engine/CTCBase.h"
#include "engine/EHeroAbility.h"
#include "stlport/fable_containers.h"
class CThingCreatureBase;class C3DVector;
class CTCSpecialAbilities:public CTCBase {
public:static CThing* FABLE_FASTCALL CreateParticleEffect(long,const C3DVector&);
#ifdef FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_SPECIAL_ABILITIES;}
 void UnLockAbilitiyMutex(){AbilityMutexLock=false;CurrentAbility=(EHeroAbility)0;}
 static void FABLE_FASTCALL FadeOrKillEffect(CIntelligentPointer<CThing>&);
#endif
#ifdef FABLE_RETAIL_DIVINE_FINALCLEANUP
 static void FABLE_FASTCALL FadeOrKillEffect(CIntelligentPointer<CThing>&);
#endif
private:class CUndeadFollower;bool AbilityMutexLock,UsingBed;EHeroAbility CurrentAbility;
 std::list<CUndeadFollower> UndeadFollowers;CThingCreatureBase& Creature;std::vector<bool> AbilityAvailability;
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0072DA20
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x0072DA30
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCSpecialAbilities)==0x28);
#elif defined(FABLE_RETAIL_CTCSPECIALABILITIES)
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCSpecialAbilities : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_SPECIAL_ABILITIES; }
    bool GetAbilityMutexLock() const { return AbilityMutexLock; }
private:
    bool AbilityMutexLock;                                   // +0x0C
    bool UsingBed;                                           // +0x0D
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0072DA20
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x0072DA30
public:
#endif
};
#else
class CTCSpecialAbilities;
#endif
#endif // FABLE_ENGINE_CTCSPECIALABILITIES_H
