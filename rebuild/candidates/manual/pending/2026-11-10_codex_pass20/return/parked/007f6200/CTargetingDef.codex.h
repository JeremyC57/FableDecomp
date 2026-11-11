#ifndef FABLE_ENGINE_CTARGETINGDEF_H
#define FABLE_ENGINE_CTARGETINGDEF_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "engine/ETargetingType.h"
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_map.h"
class CTargetingDef : public CSubDefClassBase {
public:
 virtual void Copy(CDefClassBase const * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 unsigned long Type;
 bool UseGlowTargeting;
 float TargetArc;
 float TargetArcPCThirdPersonAimingMode;
 bool TargetingBasePosFromCamera;
 bool TargetingFacingDirFromCamera;
 bool TargetingPCFacingDirFromCamera;
 bool RejectTargetsBehindIfRunning;
 bool AcceptOnlyEnemiesBehind;
 float EnemyBehindTargetingRange;
 float FirstPersonTargetingAngleFalloff;
 float FirstPersonTargetingDistanceFalloff;
 float ThirdPersonTargetingAngleFalloff;
 float ThirdPersonTargetingDistanceFalloff;
 float ZLockTargetSelectAngleFalloff;
 float ZLockTargetSelectDistanceFalloff;
 float FirstPersonTargetingOverrideShortRangeFactor;
 float FirstPersonTargetingOverrideShortRangeFalloff;
 float ThirdPersonTargetingOverrideShortRangeFactor;
 float ThirdPersonTargetingOverrideShortRangeFalloff;
 float FirstPersonTargetingEnemyPreferenceFactor;
 float ThirdPersonTargetingEnemyPreferenceFactor;
 std::map<enum ETargetingType,float,std::less<enum ETargetingType>,std::allocator<std::pair<enum ETargetingType const ,float> > > TargetingRanges;
 float PreferPlayerWeight;
 float PreferNonCombatantsWeight;
 float PreferLastAttackerWeight;
 float PreferCurrentBestEnemyWeight;
 float GetTargetingRange(ETargetingType p0) const;
 virtual void Transfer(CPersistContext & p0);
 CTargetingDef(CTargetingDef const & p0);
 CTargetingDef();
 virtual ~CTargetingDef();
 CTargetingDef & operator=(CTargetingDef const & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTargetingDef)==144);
#endif
