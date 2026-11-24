#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCESTATICMESH_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCESTATICMESH_H
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
class CPrimitive;
// Original complete member set, both PDBs, size20; secondary component@12.
class CTCGraphicAppearanceStaticMesh:public CTCBase,public CAGraphicAppearanceComponent {
public:
 virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);
 virtual void OnAppearanceClear();
 virtual void GetMeshesUsed(std::vector<CMeshInfo>&)const;
private:
 CPrimitive* PrimHandle;
};
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearanceStaticMesh)==20);
#endif
