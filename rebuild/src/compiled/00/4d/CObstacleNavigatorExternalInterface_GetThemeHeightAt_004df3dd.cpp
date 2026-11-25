// Leaf: ?GetThemeHeightAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@@Z
// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_themeHeight (table ?g_themeHeight@@3MA -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
class CObstacleNavigatorExternalInterface { public: virtual float GetThemeHeightAt(const class C3DVector&) const; };
float CObstacleNavigatorExternalInterface::GetThemeHeightAt(const class C3DVector& at) const
{
    return 0.0f;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_GetThemeHeightAt -> CObstacleNavigatorExternalInterface::GetThemeHeightAt, the RTTI-placed PDB member; class model: the row's local class model.
