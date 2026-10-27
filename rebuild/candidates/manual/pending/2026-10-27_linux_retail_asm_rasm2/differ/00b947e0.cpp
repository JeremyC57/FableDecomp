// CEngineSceneMap::Update() -- size the grid hierarchy to the scene: side = sqrt(primitives in the top grid / the scene's primitives-per-cell target), levels = floor(log2(max(2*side - 1, 1))) + 1 clamped to [1, MaxLevels]; rebuild the grid levels when the count changes
// Retail-asm: 0x00B94813 `fld dword ptr [esp + 8]` ; 0x00B94817 `fistp dword ptr [esp + 0xc]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B947E0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The cell side is rounded by the helper.
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
#include <math.h>
#pragma intrinsic(sqrt)

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
