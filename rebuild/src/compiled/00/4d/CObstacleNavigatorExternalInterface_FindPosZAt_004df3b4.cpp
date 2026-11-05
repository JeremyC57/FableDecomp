// CObstacleNavigatorExternalInterface::FindPosZAt @ 0x004df3b4
// Leaf: ?CObstacleNavigatorExternalInterface_FindPosZAt@@YGMMM@Z
// Retail: fld dword ptr [g_obstacleNavPosZ]; ret 8
extern float g_obstacleNavPosZ;

float __stdcall CObstacleNavigatorExternalInterface_FindPosZAt(float pos, float b)
{
    return g_obstacleNavPosZ;
}