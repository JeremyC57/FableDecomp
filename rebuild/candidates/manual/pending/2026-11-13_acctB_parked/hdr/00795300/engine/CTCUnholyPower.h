#ifndef FABLE_ENGINE_CTCUNHOLYPOWER_H
#define FABLE_ENGINE_CTCUNHOLYPOWER_H
// Retail opt-in FABLE_RETAIL_CTCUNHOLYPOWER_IMPLICIT_INFO_DTORS (row 0x00795300): PerCreatureInfo / PerEffectInfo destructors are the
// implicit ones in retail (??_GPerEffectInfo 0x007939A0 runs only ~CIntelligentPointer; FrameUpdate expands both in place).
// Complete original owner: Ego_r0x2ef4e/FableWin0x31a89; native retail size208.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CWillButtonHelperTimer.h"
#include "engine/C3DVector.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
#include "engine/CTCSoundPlayer.h"
class CSpecialAbilitiesUnholyPowerDef;class CThingCreatureBase;
class CTCUnholyPower:public CTCBase {
private:
 struct PerCreatureInfo {
public:
 CIntelligentPointer<CThingCreatureBase> ActualCreature;
 long FramesToGetHit;
 CIntelligentPointer<CThing> Effect;
 CEnginePrimitiveThemeChannelDesc DefaultChannel;
 PerCreatureInfo(const PerCreatureInfo & p0);
 PerCreatureInfo();
#ifndef FABLE_RETAIL_CTCUNHOLYPOWER_IMPLICIT_INFO_DTORS
 ~PerCreatureInfo();
#endif
 PerCreatureInfo & operator=(const PerCreatureInfo & p0);
 };
 struct PerEffectInfo {
public:
 C3DVector Position;
 CIntelligentPointer<CThing> Effect;
 PerEffectInfo(const PerEffectInfo & p0);
 PerEffectInfo();
#ifndef FABLE_RETAIL_CTCUNHOLYPOWER_IMPLICIT_INFO_DTORS
 ~PerEffectInfo();
#endif
 PerEffectInfo & operator=(const PerEffectInfo & p0);
 };
 typedef std::map<CThingCreatureBase*,PerCreatureInfo> MapOfCreaturesToPerCreatureInfo;
 typedef std::vector<PerEffectInfo> VectorOfCreaturesToPerEffectInfo;
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCUnholyPower(const CTCUnholyPower & p0);
 CTCUnholyPower(CThing & p0);
 virtual ~CTCUnholyPower();
 virtual bool HasFrameUpdate() const;
 virtual void OnCreate();
 virtual void FrameUpdate();
 virtual void OnKill();
 virtual void OnRemove();
 void FinishMove();
 void CancelMove();
 void CancelForCutscene();
#ifdef FABLE_RETAIL_CTCUNHOLYPOWER_INLINE_ACCESSORS
 // Opt-in (row 0x00795300): FableWin.pdb header-inline accessors (debug copies 0x02A217E0 / 0x02211510 / 0x0280F2F0, far from the
 // TU's own functions 0x0268E050..0x02694960; no Ego_r.pdb out-of-line copy).
 void SetUnholyPowerActionInitialised(bool initialised){UnholyPowerActionInitialised=initialised;}
 bool HasUnholyPowerActionInitialised() const{return UnholyPowerActionInitialised;}
#else
 void SetUnholyPowerActionInitialised(bool p0);
 bool HasUnholyPowerActionInitialised() const;
#endif
 void SetPItemRemoved(CIntelligentPointer<CThing> p0);
 float GetDamageMultiplier() const;
#ifdef FABLE_RETAIL_CTCUNHOLYPOWER_INLINE_ACCESSORS
 bool IsCharging() const{return CurFramesForEnvChange<NumFramesForEnvChange;}
#else
 bool IsCharging() const;
#endif
private:
 void FinalCleanup();
 bool Attackable(CThingCreatureBase * p0);
 void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> p0);
 void UnsheatheWeapon();
 unsigned long PlaySoundW(const CCharString & p0, bool p1);
 void StopSound(unsigned long sound) {CTCSoundPlayer* sound_player=0;if(GetParentThing().GetTC(TCI_SOUND_PLAYER,&sound_player))sound_player->StopAttachedSound(sound);}
 void SetPTarget();
 CDefPointer<CSpecialAbilitiesUnholyPowerDef const > PDef;
 CIntelligentPointer<CThing> PItemRemoved;
 bool UnholyPowerActionInitialised;
 CWillButtonHelperTimer Timer;
 long SpellLevel;
 long NumFramesBetweenDamageHit;
 long DurationOfSpellFrames;
 long NumFramesForEnvChange;
 long CurFramesForEnvChange;
 bool Finished;
 long NumFramesToWaitForFadeOut;
 bool CutSceneCancel;
 CIntelligentPointer<CThingCreatureBase> PTarget;
 bool IsZTargetting;
 bool PerformedReleaseAction;
 std::map<CThingCreatureBase *,CTCUnholyPower::PerCreatureInfo,std::less<CThingCreatureBase *>,std::allocator<std::pair<CThingCreatureBase * const,CTCUnholyPower::PerCreatureInfo> > > LastFrameCreaturesHit;
 std::vector<CTCUnholyPower::PerEffectInfo,std::allocator<CTCUnholyPower::PerEffectInfo> > HitEffects;
 float PerFrameStaminaModify;
 float AccModifyStamina;
 long StaminaModify;
 CIntelligentPointer<CThing> BuildUpEffect;
 CIntelligentPointer<CThing> DarkFacesEffect;
 CEnginePrimitiveThemeChannelDesc HeroDefaultChannel;
 CEnginePrimitiveThemeChannelDesc SpellExclusionChannel;
 CIntelligentPointer<CThing> HandEffect1;
 CIntelligentPointer<CThing> HandEffect2;
 unsigned long LoopingSound;
 CIntelligentPointer<CThing> PObstruction;
public:
 CTCUnholyPower & operator=(const CTCUnholyPower & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCUnholyPower)==208);
#endif
