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
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#include "engine/CIntelligentPointer.h"
#endif
#include "engine/EHeroAbility.h"
#include "stlport/fable_containers.h"
class CThingCreatureBase;class C3DVector;
class CTCSpecialAbilities:public CTCBase {
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
public: void SetUsingBed(bool value) { UsingBed=value; }
#endif

#ifdef FABLE_RETAIL_UNDEAD_FOLLOWER_LIMBO_PASS30
public:
    virtual void OnSetInLimbo(bool in_limbo);
    void ReclaimUndeadFollower(CThingGameObject& thing);
#endif
public:static CThing* FABLE_FASTCALL CreateParticleEffect(long,const C3DVector&);
#ifdef FABLE_RETAIL_DIVINE_WRATH_CTOR_PASS28
 // FableWin 0267D580, with both member stores inlined in retail at 0078EE1A.
 void LockAbilitiyMutex(EHeroAbility ability) { AbilityMutexLock = true; CurrentAbility = ability; }
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_ISCHARGINGUPABILITY
 // NF118 (PRIORITY q6369 / q6388, rows 0x0072A860 / 0x0068F2A0 / 0x00690030): Ego_r.pdb 0x00741700 / FableWin.pdb 0x0254F790
 // ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ, out of line in retail at 0x0072A860 (`mov al,[ecx+0Ch] ; test al,al ; setne al`;
 // the Ego_r donor run IsAbleToCastRaiseDead / AddHarvestedSoul / SetupRaiseDeadSpell / IsChargingUpAbility is retail's TU head
 // 0x0072A830 / 0x0072A840 / 0x0072A850 / 0x0072A860; Ego_d.pdb carves it as a tc_special_abilities.obj member). Declaration only.
public: bool IsChargingUpAbility() const;                                  // 0x0072A860
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_GETCURRENTABILITYSTRAFEANIM
 // NF118 (PRIORITY q6389, row 0x00690030): Ego_r.pdb 0x00741D90 / FableWin.pdb 0x0254F6A0
 // ?GetCurrentAbilityStrafeAnim@CTCSpecialAbilities@@QBE?AVCCharString@@XZ, out of line in retail at 0x0072B870 (landed). Declaration only.
public: CCharString GetCurrentAbilityStrafeAnim() const;                   // 0x0072B870
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_GETCURRENTABILITYBEINGUSED
 // NF118 (row 0x00690030): FableWin.pdb ?GetCurrentAbilityBeingUsed@CTCSpecialAbilities@@QBE?AW4EHeroAbility@@XZ, inline (Ego_d.pdb carves
 // its COMDAT under input_process_special_abilities.obj, the 0x00690030 TU; retail reads CurrentAbility +0x10 in place). No layout change.
public: EHeroAbility GetCurrentAbilityBeingUsed() const { return CurrentAbility; }
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_ISABLETOUSEANYABILITY
// Opt-in (hand4b w24, row 0x0072C430 IsAbilityAvailable): Ego_r.pdb / FableWin.pdb ?IsAbleToUseAnyAbility@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z,
// out of line in retail at 0x0072AF30 (`push ebx ; mov esi,ecx ; call 0x0072AF30`, 0x0072C437). Declaration only; no layout change.
public: bool IsAbleToUseAnyAbility(EHeroAbility type) const;              // 0x0072AF30
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_CANCELUSINGABILITY
// Opt-in (NF435, row 0x0089D750 CGameScriptInterface::CancelUsingAbility): Ego_r.pdb / FableWin.pdb
// ?CancelUsingAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z, out of line in retail at 0x0072ACF0 (the row's call target). Declaration
// only; no layout change.
public: bool CancelUsingAbility(EHeroAbility);                            // 0x0072ACF0
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
 // Both original ctor/dtor and exact static interface API, full branch only.
 CTCSpecialAbilities(CThing&);virtual ~CTCSpecialAbilities();
#if !defined(FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE) && !defined(FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT)
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_SPECIAL_ABILITIES;}
#endif
#endif
#ifdef FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_SPECIAL_ABILITIES;}
 void UnLockAbilitiyMutex(){AbilityMutexLock=false;CurrentAbility=(EHeroAbility)0;}
 static void FABLE_FASTCALL FadeOrKillEffect(CIntelligentPointer<CThing>&);
#endif
#if defined(FABLE_RETAIL_DIVINE_FINALCLEANUP) && !defined(FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE)
 // (chunk Q17, row 0x00795090: the guard lets one TU define both opt-ins; the CANCELFORCUTSCENE branch above declares the same member.)
 static void FABLE_FASTCALL FadeOrKillEffect(CIntelligentPointer<CThing>&);
#endif
#if defined(FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT) && !defined(FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE)
 // Nested opt-in (w22 agent2027, row 0x007C6E50 CTCTurncoatSpell::FrameUpdate): FableWin.pdb ?StatGetInterfaceType@CTCSpecialAbilities@@ (the
 // GetTC<CTCSpecialAbilities> key, interface 0xA7 tested as the sign bit of the +0x34 word, 0x007C6EC2) and the inline
 // ?UnLockAbilitiyMutex@CTCSpecialAbilities@@QAEXXZ (FableWin.pdb only; retail stores `mov byte ptr [eax+0Ch],0 ; mov dword ptr
 // [eax+10h],0` in place, 0x007C6EF4) -- the same two bodies as the CANCELFORCUTSCENE block above.
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_SPECIAL_ABILITIES;}
 void UnLockAbilitiyMutex(){AbilityMutexLock=false;CurrentAbility=(EHeroAbility)0;}
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT
 // Ego_r.pdb / FableWin.pdb ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z (public static __fastcall, the bound spelling of retail
 // 0x0072AC40: `mov eax,ecx ; test eax,eax` then the dead bit +0x91 and the particle-emitter fade, right after the
 // CIntelligentPointer& overload 0x0072ABD0 as in the Ego_r donor order). Declaration only.
 static void FABLE_FASTCALL FadeOrKillEffect(CThing*);                  // 0x0072AC40
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_USE
// Retail opt-in (agentC25, row 0x0072CF40 UseSpecialAbility): Ego_r.pdb ?UseSpecialAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z,
// ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z (0x0072C430) and the private ?Setup<Ability>@CTCSpecialAbilities@@AAE_NXZ members
// at the retail addresses of their landed rows (SetupMultiStrike 0x0072AAF0: HERO_ABILITY_DOUBLE_STRIKE's case). Declarations only.
public: bool UseSpecialAbility(EHeroAbility ability);                      // 0x0072CF40
public: bool IsAbilityAvailable(EHeroAbility ability) const;               // 0x0072C430
private:
 bool SetupForceMove();   // 0x0072A9B0
 bool SetupBulletTime();   // 0x0072A970
 bool SetupEnflameSpell();   // 0x0072C790
 bool SetupPhysicalShieldSpell();   // 0x0072B0D0
 bool SetupTurncoatSpell();   // 0x0072B160
 bool SetupDrainLifeSpell();   // 0x0072B260
 bool SetupBerserk();   // 0x0072CBA0
 bool SetupMultiStrike();   // 0x0072AAF0
 bool SetupSummonSpell();   // 0x0072B330
 bool SetupLightningSpell();   // 0x0072C680
 bool SetupBattleCharge();   // 0x0072C9C0
 bool SetupAssassinRush();   // 0x0072CAA0
 bool SetupHealLifeSpell();   // 0x0072AA00
 bool SetupGhostSwordSpell();   // 0x0072AA50
 bool SetupFireballSpell();   // 0x0072C930
 bool SetupMultiArrow();   // 0x0072AAA0
 bool SetupDivineWrath();   // 0x0072CD00
 bool SetupUnholyPower();   // 0x0072CE20
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
private:
 // Both original complete nested class records:20bytes, generated copy/destructor.
 class CUndeadFollower {public:
  long Health;CIntelligentPointer<CThing> PThing;bool Active;unsigned long AppearanceSeed;
  CUndeadFollower();CUndeadFollower(long,unsigned long);
 };
 bool AbilityMutexLock,UsingBed;EHeroAbility CurrentAbility;
#else
private:class CUndeadFollower;bool AbilityMutexLock,UsingBed;EHeroAbility CurrentAbility;
#endif
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
FABLE_STATIC_ASSERT_AT(52, sizeof(CTCSpecialAbilities)==0x28);
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
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_STRAFE_CALLS_PASS24
    bool GetAbilityMutexLock() const;
    CCharString GetCurrentAbilityStrafeAnim() const;
#else
    bool GetAbilityMutexLock() const { return AbilityMutexLock; }
#endif
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES_ISABILITYAVAILABLE
    // Retail opt-in (bwf1 / agentC2, row 0x00651B04): FableWin.pdb ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z (0x0072C430).
    bool IsAbilityAvailable(EHeroAbility ability) const;                 // 0x0072C430
#endif
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
