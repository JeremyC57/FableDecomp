// Leaf: ?CObstacleNavigatorExternalInterface_GetTerrainHeightAt@@YGMPAX@Z
extern float g_terrainHeight;

float __stdcall CObstacleNavigatorExternalInterface_GetTerrainHeightAt(void* pos)
{
    (void)pos;
    return g_terrainHeight;
}