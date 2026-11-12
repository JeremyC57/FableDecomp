#ifndef FABLE_ENGINE_CTCSTEALTH_H
#define FABLE_ENGINE_CTCSTEALTH_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CStealthDef; class CCreatureActionBase;
namespace NStealth {
#if defined(_MSC_VER)||!defined(FABLE_RETAIL_STEALTH_DRAW)
enum EDangerStatus;
#else
enum EDangerStatus{};
#endif
class CDangerStatus {
public:
 CDangerStatus(NStealth::EDangerStatus p0);
 NStealth::EDangerStatus DangerStatus;
 long Timer;
};
C3DVector FABLE_FASTCALL GetStabPosOnThing(CThing&);
}
// Full original Ego_r0x28491, native vector12 yields76bytes.
class CTCStealth:public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCSTEALTH_SETSNEAKING
 // Retail opt-in (chunk Q15, row 0x00713D50): FableWin.pdb ?StatGetInterfaceType@CTCStealth@@SI?AW4ETCInterfaceType@@XZ and
 // ?SetSneaking@CTCStealth@@QAEX_N@Z (debug copies only; no Ego_r.pdb copy, expanded in retail: 0x00713DC9 `mov byte ptr [eax+0Fh],1`,
 // the bitset test of type 54 = bit 22 of +24h). Bodies only, no layout change.
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_STEALTH; }
#else
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
#endif
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCStealth(const CTCStealth & p0);
 CTCStealth(CThing & p0);
 virtual void OnCreate();
 virtual void FrameUpdate();
 virtual bool HasFrameUpdate() const;
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0);
 virtual void OnSetInLimbo(bool p0);
 bool IsAbleToSneak() const;
 bool IsAvailableToPerformStealthAttack(const CThing & p0) const;
#ifdef FABLE_RETAIL_STEALTH_DRAW
 bool IsViewable() const{return Viewable;}
#else
 bool IsViewable() const;
#endif
#ifdef FABLE_RETAIL_STEALTH_DRAW
 bool IsSmellable() const{return Smellable;}
#else
 bool IsSmellable() const;
#endif
#ifdef FABLE_RETAIL_STEALTH_DRAW
 bool IsHearable() const{return Hearable;}
#else
 bool IsHearable() const;
#endif
 bool IsPerceptible() const;
 #if defined(FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE) || defined(FABLE_RETAIL_STEALTH_DRAW)
 // Retail opt-in (agent1022, row 0x00691D70 CInputProcessCreatureMovement::ProcessInput): Ego_r.pdb / FableWin.pdb
 // ?IsSneaking@CTCStealth@@QBE_NXZ and ?SetSneakingToggleSet@CTCStealth@@QAEX_N@Z expanded in place in retail
 // (0x00691F36 `mov cl,[eax+0Fh] ; test cl,cl ; sete cl ; mov [eax+10h],cl`). Bodies only, no layout change.
 bool IsSneaking() const { return Sneaking; }
 #else
 bool IsSneaking() const;
 #endif
 bool IsSneakToggleSet() const;
 float GetSneakingSpeed() const;
#ifdef FABLE_RETAIL_CTCSTEALTH_SETSNEAKING
 void SetSneaking(bool p0) { Sneaking = p0; }
#else
 void SetSneaking(bool p0);
#endif
 #ifdef FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE
 void SetSneakingToggleSet(bool p0) { SneakingToggleSet = p0; }
 #else
 void SetSneakingToggleSet(bool p0);
 #endif
 float GetVisibilityMultiplier() const;
 float GetAudibilityMultiplier() const;
 float GetDistanceCanBeHeardFrom() const;
 C3DVector GetPositionToMoveToForStabOn(CThing & p0, float & p1) const;
 bool SetupStealthAttackAction(CThing & p0);
 void StartSneaking();
 void SetActionPermittedToSneak(const CCreatureActionBase * p0);
 void SetAsRunningAwayFromCombat();
 void SetAsInDanger(const CThing & p0, NStealth::EDangerStatus p1);
 void SetMaxNumberOfDangerThings(long p0);
 bool IsDangerThing(const CThing & p0) const;
 bool IsRunningAwayFromCombat() const;
 bool IsInDanger() const;
 bool IsAbleToFlourishDangerThings() const;
 bool IsAvailableForAttack() const;
private:
 void ResetStealthVariables();
 void UpdateViewability();
 void UpdateSmellability();
 void UpdateHearability();
 void UpdateDangerInfo();
 void InitialiseSneakSpeed();
 long GetNumberOfAttackersOverMaximum(std::vector<CThing const *,std::allocator<CThing const *> > * p0) const;
 float GetRunningAwayFromCombatHangtimeFrames() const;
 float GetDangerFeedbackHangtimeFrames() const;
 void ClearDangerStatus();
 bool Viewable;
 bool Smellable;
 bool Hearable;
 bool Sneaking;
 bool SneakingToggleSet;
 float DistanceCanBeHeardFrom;
 float MaxSneakingSpeed;
 CDefPointer<CStealthDef const > PDef;
 long FrameStartedWalking;
 long FrameStartedSneakingAndMoving;
 long SneakingAndMovingDeedHandle;
 long ActionPermittedToSneakID;
 long RunningAwayFromCombatTimer;
 long InDangerCoolOffTimer;
 std::vector<std::pair<CIntelligentPointer<CThing const >,NStealth::CDangerStatus>,std::allocator<std::pair<CIntelligentPointer<CThing const >,NStealth::CDangerStatus> > > DangerThings;
 long MaxNumberOfDangerThings;
 long RegionBasedMaxNumberOfDangerThings;
public:
 virtual ~CTCStealth();
 CTCStealth & operator=(const CTCStealth & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCStealth)==76);
#endif
