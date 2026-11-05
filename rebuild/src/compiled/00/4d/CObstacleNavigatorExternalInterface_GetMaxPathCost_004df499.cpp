// CObstacleNavigatorExternalInterface::GetMaxPathCost @ 0x004DF499
// Leaf: ?CObstacleNavigatorExternalInterface_GetMaxPathCost@@YAMXZ
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

float CObstacleNavigatorExternalInterface_GetMaxPathCost()
{
    return g_CombatMinDistance;
}