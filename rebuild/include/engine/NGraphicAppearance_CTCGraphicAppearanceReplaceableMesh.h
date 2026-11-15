#ifndef FABLE_ENGINE_CTCGRAPHICAPPEARANCEREPLACEABLEMESH_H
#define FABLE_ENGINE_CTCGRAPHICAPPEARANCEREPLACEABLEMESH_H
#ifdef FABLE_RETAIL_CTCREPLACEABLEMESH_PASS22
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/CDefPointer.h"
#include "engine/CReplaceableMeshDef.h"
namespace NGraphicAppearance {
class CPrimitive;
class CTCGraphicAppearanceReplaceableMesh:public CTCBase,public CAGraphicAppearanceComponent {
public:
 bool Replaced;
 unsigned long MeshIndex;
 CPrimitive* PrimHandle;
 CDefPointer<const CReplaceableMeshDef> PDef;
 virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);
};
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh)==32);
#endif
#endif
// Retail opt-in (Codex pass22): both original TPI types,complete32-byte component with secondary base12.
