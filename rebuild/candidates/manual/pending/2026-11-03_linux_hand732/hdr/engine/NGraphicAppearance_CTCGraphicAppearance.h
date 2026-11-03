#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCE_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCE_H
// New shared header (chunk 732, row 0x006AA2E0 CThingPlayerCreature::DrawPreparePrimitivesForRendering): the retail
// NGraphicAppearance::CTCGraphicAppearance over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCGraphicAppearance@NGraphicAppearance@@ : CTCBase@0,
// ghidra_out/rtti_bases.tsv). Members are not modelled; only the out-of-line accessor the rows call:
// Ego_r.pdb / FableWin.pdb ?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z (retail 0x004C1060; the
// float is the CEnginePrimitivePrepareInfo interpolation time, +0x04).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase {
public:
    C3DVector GetFocalPos(float time) const;                 // 0x004C1060
};
}
#endif
