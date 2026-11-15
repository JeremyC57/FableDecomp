// CEnginePrimitiveRenderer::SetDrawDistanceQuality(EEngineVideoOptionQuality,const CEngineVideoOptionsDef&) -- apply draw distances and invalidate scenes.
// Leaf: ?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z
// Retail: 0x00B32010,134 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_PASS22_DRAWDISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_PASS22_SETENABLE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CEnginePrimitiveRenderer.h"
void CEnginePrimitiveRenderer::SetDrawDistanceQuality(EEngineVideoOptionQuality, const CEngineVideoOptionsDef& def)
{
    DrawDistanceFactor = def.DrawDistanceMultiplier;
    DrawDistanceMinimum = def.DrawDistanceMinimum;
    DrawDistanceMaximum = def.DrawDistanceMaximum;
    RepeatedMeshDrawDistanceFactor = def.RepeatedMeshDrawDistanceFactor;
    ZSpriteDrawDistanceFactor = def.ZSpriteDrawDistanceMultiplier;
    MinimumZSpriteAsMeshDistance = def.MinimumZSpriteAsMeshDistance;
    MaximumZSpriteAsMeshDistance = def.MaximumZSpriteAsMeshDistance;
    DrawDistanceInvalid = true;
    for (CArray<CEngineScene*>::iterator scene = SceneList.begin(); scene != SceneList.end(); ++scene)
        (*scene)->InvalidateBoundingBoxesAndDrawDistances();
    RepeatedStaticMeshManager->SetEnable(def.EnableRepeatedMeshes);
}