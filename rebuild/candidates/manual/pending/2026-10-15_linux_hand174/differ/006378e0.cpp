// CThingMapManager::GetThingMapwhoLevel(float radius) -- picks the mapwho level for a thing of the given radius: the first level whose cell size exceeds ceil(2*radius), else the last level
// Grade: DIFFER(105v106) lane gap -- retail's ceil is a bare dword `fistp [ebp+8]` / `fild` sequence with 0.5f/1.0f frame constants and the
// exact-value fix-up `cmp eax,[ebp-10h] ; jne ; inc [ebp+8]` (the same inline round helper as CThingPhysical::ModifyHealth 0x006A68F0);
// the lane compiles (long) to an _ftol call (known lane gap: bare dword fistp)
// Leaf: GetThingMapwhoLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006378E0, length 106 bytes (`ret 4`), frame pointer (`optimize("y", off)` TU).
// Callees: none. Globals: the mapwho level count @ 0x013BA848, the 16-byte level records @ 0x0137DAD8 (cell size first; the
// last-level threshold read through 0x013BA84C).
#pragma optimize("y", off)
struct SMapwhoLevel {                           // 16 bytes
    long CellSize;                              // +0x00
    long _pad[3];
};
extern long g_NoMapwhoLevels;                   // 0x013BA848
extern long g_MaxMapwhoCellSize;                // 0x013BA84C
extern SMapwhoLevel g_MapwhoLevels[];           // 0x0137DAD8
static inline long CeilToLong(float v)
{
    float half = 0.5f;
    float one = 1.0f;
    long r = (long)(v - half);
    float back = (float)r + one;
    if (*(long*)&v == *(long*)&back)
        ++r;
    return r;
}
struct CThingMapManager {
    long GetThingMapwhoLevel(float radius);
};

long CThingMapManager::GetThingMapwhoLevel(float radius)
{
    long size = CeilToLong(radius + radius);
    if (size >= g_MaxMapwhoCellSize)
        return g_NoMapwhoLevels - 1;
    long level = 0;
    for (SMapwhoLevel* info = g_MapwhoLevels; size >= info->CellSize; ++info)
        ++level;
    return level;
}
