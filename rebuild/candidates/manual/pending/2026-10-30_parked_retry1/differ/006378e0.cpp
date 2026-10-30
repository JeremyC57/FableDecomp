// CThingMapManager::GetThingMapwhoLevel(float radius) -- picks the mapwho level for a thing of the given radius: the first level whose cell size exceeds ceil(2*radius), else the last level
// Grade: IMPROVED(108v106) retry1 -- retail_asm FloorToLong in place (lane gap disproved); residue: the unrotated, unbounded level-search loop with a dead count load
// exact-value fix-up `cmp eax,[ebp-10h] ; jne ; inc [ebp+8]` (the same inline round helper as CThingPhysical::ModifyHealth 0x006A68F0);
// the lane compiles (long) to an _ftol call (known lane gap: bare dword fistp)
// Retail-asm: 0x00637900 `fsub [ebp-8]` + 0x00637903 `fistp dword ptr [ebp+8]` -- the engine inline-assembly FloorToLong helper (landed 0x006A68F0); VC7.1 C++ (long)x calls _ftol.
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
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
struct CThingMapManager {
    long GetThingMapwhoLevel(float radius);
};

long CThingMapManager::GetThingMapwhoLevel(float radius)
{
    long size = FloorToLong(radius + radius);
    if (size < g_MaxMapwhoCellSize) {
        long count = g_NoMapwhoLevels;
        for (long level = 0; level < count; ++level)
            if (size < g_MapwhoLevels[level].CellSize)
                return level;
    }
    return g_NoMapwhoLevels - 1;
}
