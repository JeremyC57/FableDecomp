#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEBOUNDINGBOX_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEBOUNDINGBOX_H
// New header (chunk 2028, row 0x00B9A4A0): CEngineSubPrimitiveBoundingBox : CEngineSubPrimitiveBase (RTTI vtable 0x012A3E84, no own
// member: the 0x18-byte pool allocation). Ego_r.pdb / FableWin.pdb ??0CEngineSubPrimitiveBoundingBox@@QAE@PAVCEngineInternalPrimitiveOcclusionBase@@@Z
// is inline: this TU keeps the out-of-line COMDAT 0x00B98B10 (called at 0x00B9A5BE) and expands it at 0x00B9A694 (base constructor
// 0x0099A2F0, Parent +0x0C, NextSubPrimitive +0x10 = 0, vtable); ??1CEngineSubPrimitiveBoundingBox@@UAE@XZ virtual; ?GetParent@..@@QAEPAVCEngineInternalPrimitiveOcclusionBase@@XZ.
#include "engine/CEngineSubPrimitiveBase.h"
class CEngineInternalPrimitiveOcclusionBase;
class CEngineSubPrimitiveBoundingBox : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase* parent)
        : CEngineSubPrimitiveBase(reinterpret_cast<CEngineInternalPrimitiveBase*>(parent)) { }
    #ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
 // Original FableWin module904 dtor has no own work; only the real base dtor call.
 // Evidence: SUBPRIMITIVE_DTOR_LOCALS.json; donor02E831E0/02E83240; retail deleting dtor00B9C130.
 // Implicit virtual destructor: native00B9C130 forwards straight to the real base.
#else
 virtual ~CEngineSubPrimitiveBoundingBox();
#endif
};
FABLE_STATIC_ASSERT_AT(15,sizeof(CEngineSubPrimitiveBoundingBox) == 0x18);
#endif
