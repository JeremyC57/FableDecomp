// CWaterPatchMesh::GetWaterType() const -- scan the patch's 17x17 landscape vertices; for each vertex map the three blended theme ids to landscape-theme indices and, while the result is still none (0) or the overridable type 7, take the first theme water type: layer 0 whenever it has water, layers 1 and 2 only while the previous layer had water
// Leaf: ?GetWaterType@CWaterPatchMesh@@ABE?BW4EWaterType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEAB40, 345 bytes, thiscall, plain `ret`; private const, returns const EWaterType
//   (Ego_r.pdb + FableWin.pdb ?GetWaterType@CWaterPatchMesh@@ABE?BW4EWaterType@@XZ). No callers in retail.
// Callees: CEngineMap::PeekThemeId(long x, long y, long layer) const 0x00B43720 (three calls, layer 0/1/2),
//   CDefinitionManager::GetDefClassIndexFromGlobalIndex(long) const 0x009ACCA0 on g_pEngineDefinitionManager 0x01436E28
//   (three calls); inline CEngineLandscapeMap::PeekEngineMap (Map +0x14) and CEngineLandscapeRenderer::PeekTheme
//   (retail's out-of-line copy is 0x00B43CE0; LandscapeThemes +0x4B8 of the renderer global 0x01436EA8, 0x38-byte
//   CLandscapeTheme, WaterType +0x30).
// Shape: Offset.X / Offset.Y are read once and both loop ends are named locals declared X first, then Y (retail's
//   spill-store order at 0x00BEAB55). endY has no frame slot: VC7.1 recomputes `startY + 16` at the inner latch
//   (0x00BEAC63), but as a named local it is one more allocation candidate in the entry block, which is what ranks the
//   outer index (ebx) above the theme ids (ebp); written inline (`y <= startY + 16`) the same code comes out with x
//   and theme0 swapped. Each theme id variable is reassigned with its theme index (retail shares one frame slot per
//   layer). Every layer re-reads the theme's water type for the store instead of reusing the tested value
//   (0x00BEAC35 / 0x00BEAC5B; in layer 0 the re-read is folded to `mov eax,ebp` at 0x00BEAC13 after the index was
//   copied to edx at 0x00BEAC06): only a by-value inline helper around the conditional assignment reproduces that.
//   Layer 0 combines with the vertex's own type, which starts as none like in CEngineMap::PeekWaterType 0x00B40670, so
//   its unset check folds late and leaves the plain second load. CombineWaterTypes and vertexType are descriptive
//   names (found in no PDB); EWaterType enumerator names are not recovered (0 = none, 7 = overridable).
// Descriptive (made-up) globals, no PDB data symbol: g_pEngineDefinitionManager 0x01436E28 (the only landed spelling,
//   CEngine::Initialise 0x00B26360, also marked descriptive there) and g_pLandscapeRenderer 0x01436EA8 (the most
//   common typed, non-placeholder landed spelling: 8 rows declare it as CEngineLandscapeRenderer*, 44 older rows still use the
//   placeholder g_pEngine_01436ea8, queued in NAME_FIXES; the type is backed by retail's read of LandscapeThemes at +0x4B8,
//   0x00BEAC00).
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
#include "engine/CWaterPatchMesh.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineMap.h"
#include "engine/CDefinitionManager.h"
enum EWaterType { WATER_TYPE_NONE = 0, WATER_TYPE_OVERRIDABLE = 7 };
class CEngineLandscapeMap {
public:
    const CEngineMap* PeekEngineMap() const { return Map; }
    long        _padding_;
    void*       PatchTree;
    void*       Patches;
    long        PatchGridWidth;
    long        PatchGridHeight;
    CEngineMap* Map;                                             // +0x14
};
extern CDefinitionManager*       g_pEngineDefinitionManager;     // 0x01436E28 (descriptive)
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;           // 0x01436EA8 (descriptive)
static inline EWaterType CombineWaterTypes(EWaterType current, EWaterType themeType)
{
    return (current == WATER_TYPE_NONE || current == WATER_TYPE_OVERRIDABLE) ? themeType : current;
}
const EWaterType CWaterPatchMesh::GetWaterType() const
{
    long startX = Offset.X;
    long endX = startX + 16;
    long startY = Offset.Y;
    long endY = startY + 16;
    EWaterType waterType = WATER_TYPE_NONE;
    for (long x = startX; x <= endX; ++x) {
        for (long y = startY; y <= endY; ++y) {
            long theme0 = Map->PeekEngineMap()->PeekThemeId(x, y, 0);
            long theme1 = Map->PeekEngineMap()->PeekThemeId(x, y, 1);
            long theme2 = Map->PeekEngineMap()->PeekThemeId(x, y, 2);
            theme0 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme0);
            theme1 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme1);
            theme2 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme2);
            if (waterType == WATER_TYPE_NONE || waterType == WATER_TYPE_OVERRIDABLE) {
                EWaterType vertexType = WATER_TYPE_NONE;
                if (g_pLandscapeRenderer->PeekTheme(theme0).WaterType)
                    waterType = CombineWaterTypes(vertexType, g_pLandscapeRenderer->PeekTheme(theme0).WaterType);
                if (theme1 && g_pLandscapeRenderer->PeekTheme(theme1).WaterType) {
                    waterType = CombineWaterTypes(waterType, g_pLandscapeRenderer->PeekTheme(theme1).WaterType);
                    if (theme2 && g_pLandscapeRenderer->PeekTheme(theme2).WaterType) {
                        waterType = CombineWaterTypes(waterType, g_pLandscapeRenderer->PeekTheme(theme2).WaterType);
                    }
                }
            }
        }
    }
    return waterType;
}