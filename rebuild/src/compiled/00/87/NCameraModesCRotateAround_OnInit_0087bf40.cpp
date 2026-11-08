// NCameraModes::CRotateAround::OnInit() -- start the rotate-around camera: string length in XY and height offset to the observed thing, X/Y axes from the camera-to-focus line, FOV/Z offset/def copied from the previous camera mode
// Leaf: ?OnInit@CRotateAround@NCameraModes@@MAEXXZ
// Grade: RELOCATION_MATCH against the TRUE extent (own oracle: Fable.exe 0x0087BF40..0x0087C066, 295 bytes, `ret` at
//   0x0087C066 then int3 to 0x0087C070); the catalog/oracle row is a defective 6-byte span (83ec0c568bf1 = the prologue only).
//   Report for a manifest split/recut to 295 bytes; graded with QLEN=295 (bin/qx.py).
// Retail: Fable.exe @ 0x0087BF40; vtable NCameraModes::CRotateAround 0x0125C674 slot 21 (rtti-vtables.tsv) = protected virtual
//   OnInit (Ego_r.pdb ?OnInit@CRotateAround@NCameraModes@@MAEXXZ). Callees: CBase::GetObservedThing 0x0084F1B0,
//   CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise 0x00A14440 (x2), previous mode's GetDefaultFOV (slot 0x48).
// Shape: the observed position is copied as a C2DVector (only X/Y move through eax/ecx); `GetPosition() - foc_pos` is
//   the PDB's C3DVector::operator-(const C2DVector&) built in a named local, which keeps both differences on the FPU
//   (`fld; fsub; fld; fsub; fxch st(1); fstp; fstp`) and lets the 12-byte result share the 8-byte foc_pos frame slot
//   (frame 0xC). ChatGPT pass 10's field-by-field / ctor-argument forms stored X before computing Y (293v295).
// Layout: as ChatGPT pass 10 modelled it (Ego_r CBase members; retail CPhysicsState 0x30, CBase 0xA4, CRotateAround 0xC8).
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CGameCameraManager CGameCameraManagerFields
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
class C3DVector;
class C2DVector { public:                                         // FableWin.pdb C2DVector members used here
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}                    // ??0C2DVector@@QAE@MM@Z
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}              // ??0C2DVector@@QAE@ABV0@@Z
    inline C2DVector(const C3DVector& v);                          // ??0C2DVector@@QAE@ABVC3DVector@@@Z (copies X, Y)
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}     // ??0C3DVector@@QAE@MMM@Z
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}      // user copy ctor, no operator= (GOTCHAS chunk 185: lea copy)
    C3DVector& AccurateNormalise();                                // ?AccurateNormalise@C3DVector@@QAEAAV1@XZ 0x00A14440
    // ??GC3DVector@@QBE?AV0@ABVC2DVector@@@Z: built field by field in a named local -> both differences stay on the FPU
    C3DVector operator-(const C2DVector& v) const { C3DVector r; r.X = X - v.X; r.Y = Y - v.Y; r.Z = Z; return r; }
};
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}

class CThing:public CThingFields{public:const C3DVector&GetPos()const;};
// Layout: shared CThingGameObject storage extends CThing98 throughd0; no derived fields read.
class CThingGameObject:public CThing{unsigned char _pad_0x98[0x38];};
class CCameraModeDef;
// Layout: use shared handle storage and original same-type assignment; native reloads the source after Release.
template<> FABLE_FORCEINLINE CDefPointer<const CCameraModeDef>&CDefPointer<const CCameraModeDef>::operator=(const CDefPointer<const CCameraModeDef>&other){if(Object!=other.Object){Release();Object=other.Object;if(Object)Object->AddRef();}return *this;}
class CEnginePrimitivePrepareInfo;
namespace NCameraModes{class CBase;enum EMode{};}
class CGameCameraManager:public CGameCameraManagerFields{public:NCameraModes::CBase*GetCameraMode(){return reinterpret_cast<NCameraModes::CBase*>(CurrentMode_Data);}const NCameraModes::CBase*GetCameraMode()const{return reinterpret_cast<const NCameraModes::CBase*>(CurrentMode_Data);}};
namespace NCameraModes{
// Layout: native physics state Position0, Velocityc, Acceleration18, two4-byte lists24/28, flag2c.
// Only Position is read; the untouched suffix is retained as storage, not invented field types.
class CPhysicsState{public:C3DVector Position;unsigned char _pad_0x0c[0x24];};
class CBase{
public: virtual ~CBase();
public: virtual void Update()=0;
public: virtual NCameraModes::EMode GetModeType() const=0;
public: virtual void OnCameraInsideObservedThing();
public: virtual bool IsUseRightStickLookAround() const;
public: virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) const;
public: virtual bool IsShortenOnOcclusion() const;
public: virtual bool IsRaiseUpOnHeroNearby() const;
public: virtual bool IsStretchAroundHero() const;
public: virtual bool IsAvoidLandscape() const;
public: virtual bool IsFOVChange() const;
public: virtual bool IsTrackMeleeOpponents() const;
public: virtual bool IsTrackMeleeOpponentsAutoFOVChange() const;
public: virtual bool IsFinished() const;
public: virtual bool IsStringAndCage() const;
public: virtual float GetObservedThingCurrentMeshHeight() const;
public: virtual void Recentre();
public: virtual void UpdateSubComponents();
public: virtual float GetDefaultFOV() const;
public: virtual float GetDefaultRollAngle() const;
public: virtual float GetDefaultObservedPositionZOffset() const;
protected: virtual void OnInit()=0;
protected: virtual void OnDie()=0;
protected: virtual void OnReset();
protected:float DefaultFOV,DefaultRollAngle,DefaultObservedPositionZOffset;CPhysicsState LookAt;
private:CGameCameraManager&CameraManager;CDefPointer<const CCameraModeDef>PCameraModeDef;unsigned char SubComponents[12];CPhysicsState GoTo;float CameraFOV,RollAngle,ObservedPositionZOffset;C2DVector RightStickVector,LeftStickVector;bool ViewOccluded;
public:const CThingGameObject*GetObservedThing()const;const C3DVector&GetPosition()const{return GoTo.Position;}const C3DVector&GetObservedPos()const{return LookAt.Position;}float GetObservedPositionZOffset()const{return ObservedPositionZOffset;}void SetObservedPositionZOffset(float f){ObservedPositionZOffset=f;}const CDefPointer<const CCameraModeDef>&GetCameraModeDef()const{return PCameraModeDef;}
protected:CGameCameraManager&GetCameraManager(){return CameraManager;}const CGameCameraManager&GetCameraManager()const{return CameraManager;}void SetCameraModeDef(const CDefPointer<const CCameraModeDef>&p){PCameraModeDef=p;}
};
class CRotateAround:public CBase{
public:virtual EMode GetModeType()const;virtual void Update();virtual bool IsShortenOnOcclusion()const;virtual bool IsFOVChange()const;virtual bool IsAvoidLandscape()const;virtual float GetDefaultFOV()const;virtual float GetDefaultRollAngle()const;virtual float GetDefaultObservedPositionZOffset()const;
protected:virtual void OnInit();virtual void OnDie();
private:float StringLengthXY,HeightOffset;C3DVector XAxis,YAxis;float Theta;
};
FABLE_STATIC_ASSERT(sizeof(CBase)==0xa4);
FABLE_STATIC_ASSERT(sizeof(CRotateAround)==0xc8);

void CRotateAround::OnInit()
{
    C2DVector foc_pos(GetObservedPos());
    float dx = foc_pos.X - GetPosition().X;
    float dy = foc_pos.Y - GetPosition().Y;
    StringLengthXY = (float)sqrt(dx * dx + dy * dy);
    HeightOffset = GetPosition().Z - GetObservedThing()->GetPos().Z;
    C3DVector line = GetPosition() - foc_pos;
    line.Z = 0.0f;
    C3DVector& xAxis = XAxis;
    xAxis = line;
    xAxis.AccurateNormalise();
    line.X = xAxis.Y;
    line.Y = -xAxis.X;
    line.Z = 0.0f;
    YAxis = line;
    YAxis.AccurateNormalise();
    Theta = 0.0f;
    CBase* previous = GetCameraManager().GetCameraMode();
    DefaultFOV = previous->GetDefaultFOV();
    SetObservedPositionZOffset(previous->GetObservedPositionZOffset());
    SetCameraModeDef(previous->GetCameraModeDef());
}
}