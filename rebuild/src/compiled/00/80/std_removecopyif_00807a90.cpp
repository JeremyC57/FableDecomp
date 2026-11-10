// std::remove_copy_if -- stable filtering of player target points.
// Flags: /Gr
// Leaf: ??$remove_copy_if@PAVCTargetPoint@NPlayerTargeting@@PAV12@V?$CFilter_Bind3@VCThingFilter_IsInFOV@@VCTargetPointFilter_FurtherFromPosThan@?A0xdfc5f859@@V?$unary_negate@VCTargetPointFilter_IsLockedTarget@?A0xdfc5f859@@@std@@@@@std@@YIPAVCTargetPoint@NPlayerTargeting@@PAV12@00V?$CFilter_Bind3@VCThingFilter_IsInFOV@@VCTargetPointFilter_FurtherFromPosThan@?A0xdfc5f859@@V?$unary_negate@VCTargetPointFilter_IsLockedTarget@?A0xdfc5f859@@@std@@@@@Z
// Retail: 0x00807a90, 222 bytes, fastcall.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "rebuild_abi.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CTargetPointFilters.h"
#include "engine/CFilter_Bind3.h"
typedef CFilter_Bind3<CThingFilter_IsInFOV,CTargetPointFilter_FurtherFromPosThan,std::unary_negate<CTargetPointFilter_IsLockedTarget> > Pred;
template NPlayerTargeting::CTargetPoint* std::remove_copy_if(NPlayerTargeting::CTargetPoint*,NPlayerTargeting::CTargetPoint*,NPlayerTargeting::CTargetPoint*,Pred);