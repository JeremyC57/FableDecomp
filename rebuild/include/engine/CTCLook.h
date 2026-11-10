#ifndef FABLE_ENGINE_CTCLOOK_H
#define FABLE_ENGINE_CTCLOOK_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CLookDef.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/GFAngles.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
class CLookDef; class CAppearanceDef; class CThingCreatureBase; class CAnimationEntry; class CAnimateThing;
enum ECreatureAnim;
namespace NAnimationEntryBodge { class CAnimEntryLite; }
namespace NGraphicAppearance { class CPrimitive; }
// Full original Ego_r 0x2cf37 owner; all data members retained.
class CTCLook : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCLook(const CTCLook & p0);
 CTCLook(CThing & p0);
 virtual ~CTCLook();
 static long FABLE_FASTCALL GetDefaultLookFlags();
 void LookAtPos(const C3DVector & p0, long p1);
 void StopLooking(long p0);
 bool TrackNearestInterestingThing(bool p0);
 bool IsPosInViewRange(const C3DVector & p0, float p1, bool p2, bool p3) const;
 bool IsTrackingNearestInterestingThing() const;
 float GetLookAngleRelToBodyXY() const;
 bool IsInsideAnimationRange() const;
 bool IsOverriddenByParent() const;
 bool IsCurrentlyTurningViaAnimation() const;
 void SetAnimationTurningDisable(bool p0);
 void UpdateTurningAnims();
 float GetAnimationTurningSpeedMultiplier() const;
 virtual void OnCreate();
 virtual void OnInitialActivate();
 virtual void FrameUpdate();
 virtual bool HasFrameUpdate() const;
 virtual void OnAppearanceClear();
 virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance & p0, const NGraphicAppearance::CGraphicAppearanceDrawInfo & p1);
private:
 void GetAnglesFromBodyToPos(const C3DVector & p0, bool p1, float & p2, float & p3) const;
 void ImplementHeadSettings();
 void ImplementBodySettings();
 bool IsLookTimeOver() const;
 bool IsAbleToTurnViaAnimation() const;
 bool IsAbleToTurnDaBody() const;
 void UpdateHeadCreatureAnim(const CCharString & p0, ECreatureAnim p1, CAnimateThing * p2);
 float GetHeadCreatureAnimFrame(ECreatureAnim p0) const;
 float GetHeadTurnRangeXY() const { return (PDef->HeadTurnRangeXY * 0.0027777778f); }
 float GetHeadTurnRangeYZ() const;
 float GetHeadTurnRangeYZUp() const;
 float GetHeadTurnRangeYZDown() const;
 float GetEyesTurnRangeXY() const;
 float GetEyesTurnRangeYZ() const;
 void SetRecoveryAngles(const C3DVector & p0);
 float GetHeadMoveSpeedFromLookFlags() const;
 void SetHeadLooking(float p0, bool p1, bool p2, float p3, float p4);
 void SetEyesLooking(const C3DVector & p0);
 void SetBodyMoving();
 void SetBodyMovingViaAnimation();
 void InitialiseTurningViaAnimation(const C3DVector & p0, long p1);
 void CentreEyes();
 void CentreHead();
 void SetHeadTurnState(long p0);
 void SetBodyTurnState(long state) { if(BodyTurnState!=state) BodyTurnState=state; }
 void ChangeHeadAngleXY(float p0, float p1);
 void ChangeHeadAngleYZ(float p0, float p1);
 void ChangeEyeAngles(float p0, float p1);
 void GetHeadSettingsSlowInFastOut(float & p0, float & p1, float p2, float p3, float p4, float p5, float p6) const;
 void GetBodySettingsSlowInFastOut(float & p0, float & p1, float p2, float p3, float p4) const;
 bool IsLookTypeBodyOnly() const { return (LookFlags&1) && !(LookFlags&2) && !(LookFlags&4); }
 static void FABLE_FASTCALL InitialiseConsoleVariables();
 void FrameUpdateHeadInterested();
 void FrameUpdateHeadLookAtPos(const C3DVector & p0, bool p1);
 void FrameUpdateOverriddenByParent();
 void UpdateLookTimer();
 bool IsTurningBody();

#ifdef _MSC_VER
 __declspec(noinline)
#endif
 bool GetTurningAnglesAndCheckForTurningBody(float& a,float& b) {
 if(!GetTurningAngles(a,b)) return false;
 float range;
 if(!IsLookTypeBodyOnly() && !(LookFlags&0x80)) range=GetHeadTurnRangeXY();
 else range=DELTA_NOTIONAL_ZERO;
 return !GFIsSignedAngleWithinRange(b,range);
 }

#ifdef _MSC_VER
 __declspec(noinline)
#endif
 bool GetTurningAngles(float& a,float& b) {
 if((BodyTurnState&2) && IsAbleToTurnDaBody() && !(BodyTurnState&8)) {
  CTCPhysicsBase* physics=GetParentThing().GetPhysicsTC();
  float current_angle=physics->GetFacingAngleXY();
  a=RecoveryAngleToLookAtXY;
  if(LookFlags&0x40) a=DirectionToLookIn.GetAngleXY();
  else if(physics->GetPosition()!=PosToLookAt) a=GFGetAngleXYTo(physics->GetPosition(),PosToLookAt);
  b=GFGetSignedAngleDifference(current_angle,a);
  return true;
 }
 return false;
 }
 bool IsWithinYZHeadRange(float p0);
 float GetHeadAngularVelocityMaxXY();
 float GetHeadAngularVelocityMaxYZ();
 void InitialiseHeadAnims();
 C3DVector GetHeadPos() const;
 void UpdateHeadPos();
 CThingCreatureBase * PParentCreature;
 CDefPointer<CLookDef const > PDef;
 CDefPointer<CAppearanceDef const > PAppearanceDef;
 float RecoveryAngleToLookAtXY;
 float RecoveryAngleToLookAtYZ;
 float BodyAngleXY;
 float BodyAngleYZ;
 float HeadAngleRelToBodyXY;
 float HeadAngleRelToBodyYZ;
 float EyeAnglesRelToHeadXY;
 float EyeAnglesRelToHeadYZ;
 long LookTimer;
 bool UsingOldStyleTurning;
 bool TurningViaAnimationDisable;
 float FractionOfTurningToAcceptEachFrame;
 long HeadTurnState;
 long BodyTurnState;
 bool CanTurnBody;
 float BodyVelocityXY;
 float BodyVelocityYZ;
 float HeadVelocityXY;
 float HeadVelocityYZ;
 long LookFlags;
 C3DVector PosToLookAt;
 C3DVector DirectionToLookIn;
 bool EyeAnglesChanged;
 NGraphicAppearance::CPrimitive * LeftEyePrimitive;
 NGraphicAppearance::CPrimitive * RightEyePrimitive;
 std::vector<std::pair<float,CAnimationEntry const *>,std::allocator<std::pair<float,CAnimationEntry const *> > > TurningAnims;
 float TurningViaAnimMinAngle;
 const CAnimationEntry * PCurrentTurningAnim;
 bool TargetAcquired;
 C3DVector CachedHeadPos;
 long HeadCacheTimer;
 long TimeToStopLooking;
 bool OneFrameDelay;
 CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PHeadTurnAnimH;
 CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PHeadTurnAnimV;
 bool FirstTimeThrough;
public:
 CTCLook & operator=(const CTCLook & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCLook)==208);
#endif
