#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCEANIMATEDMESH_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCEANIMATEDMESH_H
// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh (agentC37, row 0x0077B350): the appearance component that draws a thing's main
// graphic as one animated mesh. Ego_r.pdb layout (struct_layouts_egor.tsv): 20 bytes, CTCBase + CAGraphicAppearanceComponent (+0x0C) +
// CPrimitive* PrimHandle +0x10. Virtuals named by Ego_r: OnAppearanceDraw (0x0077B350), OnAppearanceClear.
// Needs FABLE_RETAIL_CTCBASE.
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
class CPrimitive;
class CTCGraphicAppearance;
class CGraphicAppearanceDrawInfo;
class CTCGraphicAppearanceAnimatedMesh : public CTCBase, public CAGraphicAppearanceComponent {
public:
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& tc_appearance, const CGraphicAppearanceDrawInfo& draw_info);   // 0x0077B350
    virtual void OnAppearanceClear();
    CPrimitive* PrimHandle;                                 // +0x10
};
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh) == 0x14);
#endif // FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCEANIMATEDMESH_H
