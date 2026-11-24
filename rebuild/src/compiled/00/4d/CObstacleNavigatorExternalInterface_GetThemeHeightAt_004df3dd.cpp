// Leaf: ?GetThemeHeightAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@@Z
extern float g_themeHeight;
class CObstacleNavigatorExternalInterface { public: virtual float GetThemeHeightAt(const class C3DVector&) const; };
float CObstacleNavigatorExternalInterface::GetThemeHeightAt(const class C3DVector& at) const
{
    return g_themeHeight;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_GetThemeHeightAt -> CObstacleNavigatorExternalInterface::GetThemeHeightAt, the RTTI-placed PDB member; class model: the row's local class model.
