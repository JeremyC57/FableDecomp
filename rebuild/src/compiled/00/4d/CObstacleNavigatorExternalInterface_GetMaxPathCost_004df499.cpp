// CObstacleNavigatorExternalInterface::GetMaxPathCost @ 0x004DF499
// Leaf: ?GetMaxPathCost@CObstacleNavigatorExternalInterface@@UBEMXZ
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

class CObstacleNavigatorExternalInterface { public: virtual float GetMaxPathCost() const; };
float CObstacleNavigatorExternalInterface::GetMaxPathCost() const
{
    return g_CombatMinDistance;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_GetMaxPathCost -> CObstacleNavigatorExternalInterface::GetMaxPathCost, the RTTI-placed PDB member; class model: the row's local class model.
