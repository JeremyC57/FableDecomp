// CThingMapManager::GetThingMapwhoLevel(float radius) -- picks the mapwho level for a thing of the given radius: the first level whose cell size exceeds ceil(2*radius), else the last level
// exact-value fix-up `cmp eax,[ebp-10h] ; jne ; inc [ebp+8]` (the same inline round helper as CThingPhysical::ModifyHealth 0x006A68F0);
// the lane compiles (long) to an _ftol call (known lane gap: bare dword fistp)
// Leaf: GetThingMapwhoLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006378E0, length 106 bytes (`ret 4`), frame pointer (`optimize("y", off)` TU).
// Callees: none. Globals: the mapwho level count @ 0x013BA848, the 16-byte level records @ 0x0137DAD8 (cell size first; the
// last-level threshold read through 0x013BA84C).
// Retail-asm: 0x006378FD..0x00637917 `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-4] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp+8]` (the result homed in the dead `radius` argument slot) -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
#pragma optimize("y", off)
struct SMapwhoLevel {                           // 16 bytes
    long CellSize;                              // +0x00
    long _pad[3];
};
extern long g_NoMapwhoLevels;                   // 0x013BA848
extern long g_MaxMapwhoCellSize;                // 0x013BA84C
extern SMapwhoLevel g_MapwhoLevels[];           // 0x0137DAD8
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
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
    long cellSize = FloorToLong(radius + radius);     // `size` is a MASM operator: not a name the asm block may see
    long level = g_NoMapwhoLevels - 1;
    if (cellSize < g_MaxMapwhoCellSize) {
        level = 0;
        while (cellSize >= g_MapwhoLevels[level].CellSize)
            ++level;
    }
    return level;
}
