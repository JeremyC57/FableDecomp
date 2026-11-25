// CObstacleNavigatorExternalInterface::FindPosZAt @ 0x004df3b4
// Leaf: ?FindPosZAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@_N@Z
// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_obstacleNavPosZ (table ?g_obstacleNavPosZ@@3MA -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
// Retail: fld dword ptr [g_obstacleNavPosZ]; ret 8

class CObstacleNavigatorExternalInterface { public: virtual float FindPosZAt(const class C3DVector&, bool) const; };
float CObstacleNavigatorExternalInterface::FindPosZAt(const class C3DVector& pos, bool b) const
{
    return 0.0f;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_FindPosZAt -> CObstacleNavigatorExternalInterface::FindPosZAt, the RTTI-placed PDB member; class model: the row's local class model.
