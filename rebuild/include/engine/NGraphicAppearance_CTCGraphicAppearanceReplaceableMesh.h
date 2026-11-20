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
#ifdef FABLE_RETAIL_CTCREPLACEABLEMESH_REPLACEMESHBYINDEX
 // NF200 opt-in (2026-11-20, row 0x006412B0 CTCLight::SetActive): Ego_r.pdb 0x007AB780 + FableWin.pdb 0x026C4B30
 // ?ReplaceMeshByIndex@CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@QAEXK@Z, out of line in retail at 0x007A69F0 (landed).
 // Declaration only, no layout change.
 void ReplaceMeshByIndex(unsigned long index);                       // 0x007A69F0
#endif
};
}
FABLE_STATIC_ASSERT_AT(19, sizeof(NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh)==32);
#endif
#endif
// Retail opt-in (Codex pass22): both original TPI types,complete32-byte component with secondary base12.
