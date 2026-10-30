// Grade: IMPROVED(251v246) retry1 -- retail_asm helper in place; residue: span colour eax v esi and the late push esi
// CEngineSceneMap::Update() -- size the grid hierarchy to the scene: side = sqrt(primitives in the top grid / the scene's primitives-per-cell target), levels = floor(log2(max(2*side - 1, 1))) + 1 clamped to [1, MaxLevels]; rebuild the grid levels when the count changes
// Retail-asm: 0x00B94813 `fld dword ptr [esp+8]` + 0x00B94817 `fistp dword ptr [esp+0Ch]` -- the engine inline-assembly float-to-long helper; VC7.1 C++ (long)x calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B947E0, length 246 bytes (+ int3 padding).
// Callees: CEngineSceneGrid::GetCellTotalPrimitiveCount(unsigned long, unsigned long) @ 0x00B91D20 (on GridLevels[0], (0, 0)),
// CEngineSceneMap::ResizeGrid(unsigned long) @ 0x00B94540 (manifest label "CEngineScene::ResizeGrid", called on the map).
// Layout: as the landed CEngineSceneMap::Initialise 0x00B949A0 (MaxLevels +0x14, GridLevels +0x18 of 8-byte counted pointers,
// Scene +0x24); the scene's primitives-per-cell float is at +0x54 (descriptive).
// Shape: the unsigned count converts through `fild` + the 2^32 fix-up, `fsqrt` is the intrinsic, the float side is rounded to
// an integer with a BARE `fistp` (retail: an inline-asm float-to-long helper, the asm_bake FtoL family -- a C cast gives
// `__ftol2`); HighestSetBit is the landed 0x0099FDB0 helper inlined (constant-folded on the clamped branch).
#include <math.h>
#pragma intrinsic(sqrt)
// the engine's inline-assembly float-to-long helper: expanded in place, reads the homed value, writes a frame word
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

__forceinline long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}

struct CEngineSceneGrid {
    unsigned long GetCellTotalPrimitiveCount(unsigned long x, unsigned long y);   // 0x00B91D20
};

template <class T> struct CCountedPointer {
    T*    m_pObject;
    long* m_pCount;
    T* operator->() const { return m_pObject; }
};

template <class T> struct CRetailArray {           // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    unsigned long size() const { return last - first; }
    T& operator[](unsigned int n) { return first[n]; }
};

struct CEngineScene {
    unsigned char _pad[0x54];
    float         PrimitivesPerCell;               // +0x54
};

struct CEngineSceneMap {
    void*                                            _vfptr;       // +0x00
    float                                            Area[4];      // +0x04
    unsigned long                                    MaxLevels;    // +0x14
    CRetailArray<CCountedPointer<CEngineSceneGrid> > GridLevels;   // +0x18
    CEngineScene*                                    Scene;        // +0x24
    void ResizeGrid(unsigned long levels);         // 0x00B94540
    void Update();
};

void CEngineSceneMap::Update()
{
    unsigned long count = GridLevels[0]->GetCellTotalPrimitiveCount(0, 0);
    float side = (float)sqrt((float)count / Scene->PrimitivesPerCell);
    long cells = FloatToLong(side);
    long span = 2 * cells - 1;
    if (span <= 1)
        span = 1;
    unsigned long levels = HighestSetBit(span) + 1;
    if (levels < 1)
        levels = 1;
    else if (levels > MaxLevels)
        levels = MaxLevels;
    if (levels != GridLevels.size())
        ResizeGrid(levels);
}
