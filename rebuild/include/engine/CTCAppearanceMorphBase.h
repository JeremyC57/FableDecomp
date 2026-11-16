#ifndef FABLE_ENGINE_CTCAPPEARANCEMORPHBASE_H
#define FABLE_ENGINE_CTCAPPEARANCEMORPHBASE_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CTCAPPEARANCEMORPHBASE.
// Class: Ego_r.pdb CTCAppearanceMorphBase : CTCBase; interface TCI_APPEARANCE_MORPH (3: the hero's component looked up at 0x004A4FFF..0x004A502D).
// Method: Ego_r.pdb ?UploadTextureMorphsToEngine@CTCAppearanceMorphBase@@QAEXAAVCIEngine@@PBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@_NM@Z
// (0x006CA7F0, called with Get3DEngine(), 0, true, 3.0f). Members are not modelled.
#ifdef FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
class CIEngine;
class CTCAppearanceMorphBase : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
    void UploadTextureMorphsToEngine(CIEngine& engine, const std::set<long>* morphs, bool, float);   // 0x006CA7F0
};
#endif
#endif // FABLE_ENGINE_CTCAPPEARANCEMORPHBASE_H
