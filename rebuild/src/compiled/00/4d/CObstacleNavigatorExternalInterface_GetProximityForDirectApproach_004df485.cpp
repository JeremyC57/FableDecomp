// CObstacleNavigatorExternalInterface::GetProximityForDirectApproach @ 0x004DF485
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

float GetProximityForDirectApproach()
{
    return g_CombatMinDistance;
}