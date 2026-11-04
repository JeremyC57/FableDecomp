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
// Retail opt-in addition (chunk 750, rows 0x0077B290 / 0x0077B9F0 / 0x0077BE80 / 0x0077C440, the appearance-component destructors):
// define FABLE_RETAIL_CTCGRAPHICAPPEARANCE for the public non-virtual UnregisterComponent 0x004C0000 (the only spelling bound there,
// ?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z, Ego_r.pdb / FableWin.pdb;
// every component destructor passes its CAGraphicAppearanceComponent subobject, `push edi` = this+0xC). A declaration only.
namespace NGraphicAppearance {
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE
class CAGraphicAppearanceComponent;
#endif
class CTCGraphicAppearance : public CTCBase {
public:
    C3DVector GetFocalPos(float time) const;                 // 0x004C1060
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE
    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000
#endif
};
}
#endif
