#ifndef FABLE_ENGINE_CTCSTEALTH_H
#define FABLE_ENGINE_CTCSTEALTH_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CStealthDef; class CCreatureActionBase;
namespace NStealth { enum EDangerStatus; class CDangerStatus {
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
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
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
 bool IsViewable() const;
 bool IsSmellable() const;
 bool IsHearable() const;
 bool IsPerceptible() const;
 #ifdef FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE
 // Retail opt-in (agent1022, row 0x00691D70 CInputProcessCreatureMovement::ProcessInput): Ego_r.pdb / FableWin.pdb
 // ?IsSneaking@CTCStealth@@QBE_NXZ and ?SetSneakingToggleSet@CTCStealth@@QAEX_N@Z expanded in place in retail
 // (0x00691F36 `mov cl,[eax+0Fh] ; test cl,cl ; sete cl ; mov [eax+10h],cl`). Bodies only, no layout change.
 bool IsSneaking() const { return Sneaking; }
 #else
 bool IsSneaking() const;
 #endif
 bool IsSneakToggleSet() const;
 float GetSneakingSpeed() const;
 void SetSneaking(bool p0);
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
