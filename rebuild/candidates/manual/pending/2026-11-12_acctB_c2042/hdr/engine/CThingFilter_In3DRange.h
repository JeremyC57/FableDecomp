#ifndef FABLE_ENGINE_CTHINGFILTER_IN3DRANGE_H
#define FABLE_ENGINE_CTHINGFILTER_IN3DRANGE_H
// New shared header (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): the 3D range thing filter. Ego_r.pdb layout
// { C3DVector Pos +0; float Range +0xC } (16 bytes; struct_layouts_egor.tsv), FableWin.pdb ??0CThingFilter_In3DRange@@QAE@ABVC3DVector@@M@Z
// (inline: FrameUpdate stores the position words and the range straight into the filter, 0x007F2EEB..0x007F2F06) and
// ??RCThingFilter_In3DRange@@QBE_NABVCThing@@@Z (out of line at 0x006A7780, landed). The unary_function<CThing,bool> base follows the
// shared CThingFilter_HasTC / CThingFilter_EnemyCreature models (empty base, no layout change).
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
class CThing;
class CThingFilter_In3DRange : public std::unary_function<CThing,bool> {
public:
    CThingFilter_In3DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
    bool operator()(const CThing& thing) const;                  // 0x006A7780
private:
    C3DVector Pos;                                               // +0x00
    float     Range;                                             // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_In3DRange) == 0x10);
#endif
