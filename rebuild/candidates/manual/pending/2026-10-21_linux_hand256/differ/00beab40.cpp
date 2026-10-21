// Grade: DIFFER(325v345) -- register allocation: retail keeps this/x/y/theme0 in edi/ebx/esi/ebp and spills theme1/theme2; the lane spills x and this
// CWaterPatchMesh::GetWaterType() const -- scan the patch's 17x17 landscape vertices; for each vertex look up the
// three blended theme ids, map them to landscape-theme indices and take the first non-zero theme water type while
// the result is still none (0) or the overridable type 7
// Leaf: GetWaterType
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEAB40, 345 bytes, `ret` (PDB ?GetWaterType@CWaterPatchMesh@@ABE?AW4EWaterType@@XZ).
// Callees: CEngineMap::PeekThemeId(long x, long y, long layer) const 0x00B43720 (labelled PeekLandscapeHeight; the
//   three-argument call and integer result match ?PeekThemeId@CEngineMap@@QBEJJJJ@Z), CDefinitionManager::
//   GetDefClassIndexFromGlobalIndex 0x009ACCA0 on the manager global 0x01436E28.
// Layout: rebuild/include/engine/CWaterPatchMesh.h (Map +0x04, Offset_X +0x08, Offset_Y +0x0c); CEngineLandscapeMap
//   ::Map +0x14 (Ego_r.pdb); CLandscapeTheme is 0x38 bytes with WaterType +0x30 (Ego_r.pdb). The theme array is the
//   CArray data pointer at retail +0x4B8 of the landscape renderer global 0x01436EA8 (PDB +0xD14; the renderer is
//   smaller in retail, as the landed CLandscapeBackgroundPatch rows note).
#define CWaterPatchMesh CWaterPatchMesh_PDB
#include "engine/CWaterPatchMesh.h"
#undef CWaterPatchMesh
enum EWaterType { WATER_TYPE_NONE = 0, WATER_TYPE_OVERRIDABLE = 7 };
struct CEngineMap {
    long PeekThemeId(long x, long y, long layer) const;           // 0x00B43720
};
struct CEngineLandscapeMap {
    long        _padding_;                                       // +0x00
    void*       PatchTree;                                       // +0x04
    void*       Patches;                                         // +0x08
    long        PatchGridWidth;                                  // +0x0c
    long        PatchGridHeight;                                 // +0x10
    CEngineMap* Map;                                             // +0x14
};
struct CDefinitionManager {
    long GetDefClassIndexFromGlobalIndex(long globalIndex) const; // 0x009ACCA0
};
extern CDefinitionManager* g_pDefinitionManager;                 // 0x01436E28
struct CLandscapeTheme {
    long       Textures[12];                                     // +0x00 .. +0x2c
    EWaterType WaterType;                                        // +0x30
    float      WaterHeight;                                      // +0x34
};
struct CEngineLandscapeRenderer {
    unsigned char    _pad_0x0[0x4b8];
    CLandscapeTheme* LandscapeThemes;                            // +0x4B8 CArray<CLandscapeTheme> data
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;          // 0x01436EA8
struct CWaterPatchMesh : CWaterPatchMesh_PDB {
    EWaterType GetWaterType() const;
};
static inline bool IsUnsetWaterType(EWaterType type)
{
    return type == WATER_TYPE_NONE || type == WATER_TYPE_OVERRIDABLE;
}
EWaterType CWaterPatchMesh::GetWaterType() const
{
    EWaterType waterType = WATER_TYPE_NONE;
    long startX = Offset_X;
    long startY = Offset_Y;
    long endX = startX + 16;
    for (long x = startX; x <= endX; ++x) {
        for (long y = startY; y <= startY + 16; ++y) {
            long theme0 = ((CEngineLandscapeMap*)Map)->Map->PeekThemeId(x, y, 0);
            long theme1 = ((CEngineLandscapeMap*)Map)->Map->PeekThemeId(x, y, 1);
            long theme2 = ((CEngineLandscapeMap*)Map)->Map->PeekThemeId(x, y, 2);
            long index0 = g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(theme0);
            long index1 = g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(theme1);
            long index2 = g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(theme2);
            if (waterType == WATER_TYPE_NONE || waterType == WATER_TYPE_OVERRIDABLE) {
                EWaterType themeWater = g_pLandscapeRenderer->LandscapeThemes[index0].WaterType;
                if (themeWater)
                    waterType = themeWater;
                if (index1 && g_pLandscapeRenderer->LandscapeThemes[index1].WaterType) {
                    if (waterType == WATER_TYPE_NONE || waterType == WATER_TYPE_OVERRIDABLE)
                        waterType = g_pLandscapeRenderer->LandscapeThemes[index1].WaterType;
                    if (index2 && g_pLandscapeRenderer->LandscapeThemes[index2].WaterType) {
                        if (waterType == WATER_TYPE_NONE || waterType == WATER_TYPE_OVERRIDABLE)
                            waterType = g_pLandscapeRenderer->LandscapeThemes[index2].WaterType;
                    }
                }
            }
        }
    }
    return waterType;
}
