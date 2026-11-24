// CObstacleNavigatorExternalInterface::GetMinPathCost @ 0x004DF492
// Leaf: ?GetMinPathCost@CObstacleNavigatorExternalInterface@@UBEMXZ
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

class CObstacleNavigatorExternalInterface { public: virtual float GetMinPathCost() const; };
float CObstacleNavigatorExternalInterface::GetMinPathCost() const
{
    return g_CombatMinDistance;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_GetMinPathCost -> CObstacleNavigatorExternalInterface::GetMinPathCost, the RTTI-placed PDB member; class model: the row's local class model.
