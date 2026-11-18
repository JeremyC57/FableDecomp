#ifndef FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
#define FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
#ifdef FABLE_RETAIL_NCAMERAMODES_CPROJECTILEWEAPON_HEIGHT
#include "engine/NCameraModes_CBase.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CGameCameraManager;
namespace NCameraModes {
class CProjectileWeapon : public CBase {
public:
 CProjectileWeapon(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,float,float);
private:
 C3DVector GetPosFromParams(float,float)const;
 C3DVector GetLookDirectionFromParams(float,float)const;
 float GetHeightParamForPostionWithPhi(const C3DVector&,float)const;
 long ShotFiredTimer; // +a4
 bool Finished; // +a8
 // Original C3DVector members have byte alignment (raw PDB +cd,+d9,+e5;
 // retail +a9,+b5,+c1). Opaque original storage avoids introducing packing.
 C3DVector XAxis;
 C3DVector YAxis;
 C3DVector ZAxis;
 float Phi,HeightParam,DeltaPhi,DeltaHeightParam,LockDeltaPhi,LockDeltaHeightParam,DeltaLockDeltaPhi,DeltaLockDeltaHeightParam;
 std::vector<C3DVector> Attractors; // vector<C3DVector> +f0
 const float MaxZoomInFOV,MinZoomInFOV;
 float ZoomInFOV;
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CProjectileWeapon)==0x108);
#endif // FABLE_RETAIL_NCAMERAMODES_CPROJECTILEWEAPON_HEIGHT
#endif
