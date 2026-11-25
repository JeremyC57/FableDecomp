// Leaf: ?CObstacleNavigatorExternalInterface_PeekNavigatorMaxSpeed@@YGMPAX@Z
// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_terrainHeight (table ?g_terrainHeight@@3MA -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].

float __stdcall CObstacleNavigatorExternalInterface_PeekNavigatorMaxSpeed(void* pos)
{
    (void)pos;
    return 0.0f;
}
// Stand-in (owner rule 2026-11-05): CObstacleNavigatorExternalInterface_PeekNavigatorMaxSpeed for CObstacleNavigatorExternalInterface::PeekNavigatorMaxSpeed; RTTI vtable slot ?PeekNavigatorMaxSpeed@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@@Z + PDB name set (fablewin).
