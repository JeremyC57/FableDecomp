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
private:class CUndeadFollower;bool AbilityMutexLock,UsingBed;EHeroAbility CurrentAbility;
 std::list<CUndeadFollower> UndeadFollowers;CThingCreatureBase& Creature;std::vector<bool> AbilityAvailability;
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
};
#else
class CTCSpecialAbilities;
#endif
#endif // FABLE_ENGINE_CTCSPECIALABILITIES_H
