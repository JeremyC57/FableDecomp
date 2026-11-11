#ifndef FABLE_ENGINE_CTHINGFILTER_ISTARGETABLE_H
#define FABLE_ENGINE_CTHINGFILTER_ISTARGETABLE_H
// CThingFilter_IsTargetable (new, chunk 2015, row 0x007A0D90 CTCWillLightning::GetTargets). Layout: Ego_r.pdb struct_layouts_egor.tsv,
// 2 bytes (`bool ZTargeting` +0, `bool UsingLightning` +1). Names: FableWin.pdb ??0CThingFilter_IsTargetable@@QAE@_N0@Z (inline in
// retail: the caller stores 0 / 1 into the two bytes, 0x007A0F5A / 0x007A0F5F) and Ego_r.pdb / FableWin.pdb
// ??RCThingFilter_IsTargetable@@QBE_NABVCThing@@@Z (out of line at 0x006A84F0, called by the landed search body 0x007A3CA0). The
// unary_function base follows the shared CThingFilter_IsNot / _In2DRange models.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_IsTargetable : public std::unary_function<CThing,bool> {
private:
    bool ZTargeting;                                                     // +0x00
    bool UsingLightning;                                                 // +0x01
public:
    CThingFilter_IsTargetable(bool z_targeting, bool using_lightning) : ZTargeting(z_targeting), UsingLightning(using_lightning) {}
    bool operator()(const CThing& thing) const;                         // 0x006A84F0
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsTargetable)==2);
#endif
