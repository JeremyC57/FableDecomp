// CObstacleNavigatorExternalInterface::GetMinPathCost @ 0x004DF492
// Leaf: ?CObstacleNavigatorExternalInterface_GetMinPathCost@@YAMXZ
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

float CObstacleNavigatorExternalInterface_GetMinPathCost()
{
    return g_CombatMinDistance;
}