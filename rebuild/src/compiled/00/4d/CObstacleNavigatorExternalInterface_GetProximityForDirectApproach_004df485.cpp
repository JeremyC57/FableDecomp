// CObstacleNavigatorExternalInterface::GetProximityForDirectApproach @ 0x004DF485
// Leaf: ?GetProximityForDirectApproach@CObstacleNavigatorExternalInterface@@UBEMXZ
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

class CObstacleNavigatorExternalInterface { public: virtual float GetProximityForDirectApproach() const; };
float CObstacleNavigatorExternalInterface::GetProximityForDirectApproach() const
{
    return g_CombatMinDistance;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_GetProximityForDirectApproach -> CObstacleNavigatorExternalInterface::GetProximityForDirectApproach, the RTTI-placed PDB member; class model: the row's local class model.
