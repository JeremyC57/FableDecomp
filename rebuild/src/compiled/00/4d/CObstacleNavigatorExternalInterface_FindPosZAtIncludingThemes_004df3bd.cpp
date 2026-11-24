// Leaf: ?FindPosZAtIncludingThemes@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@_N@Z
extern float g_findPosZTheme;

class CObstacleNavigatorExternalInterface { public: virtual float FindPosZAtIncludingThemes(const class C3DVector&, bool) const; };
float CObstacleNavigatorExternalInterface::FindPosZAtIncludingThemes(const class C3DVector& pos, bool b) const
{
    return g_findPosZTheme;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_FindPosZAtIncludingThemes -> CObstacleNavigatorExternalInterface::FindPosZAtIncludingThemes, the RTTI-placed PDB member; class model: the row's local class model.
