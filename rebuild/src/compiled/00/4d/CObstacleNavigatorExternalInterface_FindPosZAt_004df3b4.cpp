// CObstacleNavigatorExternalInterface::FindPosZAt @ 0x004df3b4
// Leaf: ?FindPosZAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@_N@Z
// Retail: fld dword ptr [g_obstacleNavPosZ]; ret 8
extern float g_obstacleNavPosZ;

class CObstacleNavigatorExternalInterface { public: virtual float FindPosZAt(const class C3DVector&, bool) const; };
float CObstacleNavigatorExternalInterface::FindPosZAt(const class C3DVector& pos, bool b) const
{
    return g_obstacleNavPosZ;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_FindPosZAt -> CObstacleNavigatorExternalInterface::FindPosZAt, the RTTI-placed PDB member; class model: the row's local class model.
