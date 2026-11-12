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
    virtual ~CEngineSubPrimitiveBoundingBox();
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveBoundingBox) == 0x18);
#endif
