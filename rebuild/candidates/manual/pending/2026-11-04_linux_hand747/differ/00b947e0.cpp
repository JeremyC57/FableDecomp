// CEngineSceneMap::Update() -- size the grid hierarchy to the scene: side = sqrt(primitives in the top grid / the scene's primitives-per-cell target), levels = HighestSetBit(max(2*round(side) - 1, 1)) + 1 clamped to [1, MaxLevels]; resize the grid levels when the count changes
// Grade: DIFFER(251v246) -- best c dl=16 (251v246): retail keeps the span in esi (pushed at entry) and increments it in place for the inlined HighestSetBit; ours rounds into eax and copies to a lazily pushed esi (best try e; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?Update@CEngineSceneMap@@QAEXXZ
// Flags: none
// Retail-asm: 0x00B94817 `fistp dword ptr [esp+0Ch]` -- sqrt(...) stored to a frame float, reloaded and stored with a bare dword fistp:
//   the engine's inline-assembly float-to-long GFFloatToLongNear (shared engine/GFFloatToLongNear.h). VC7.1 emits no bare dword fistp
//   from C++ (NOTES.md "C++ attempts before helper"). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B947E0, 246 bytes (+ int3 padding), thiscall `ret`.
// Identity: Ego_r.pdb ?Update@CEngineSceneMap@@QAEXXZ; the level computation is Ego_r's private inline
//   ?CalculateLevel@CEngineSceneMap@@ABEKM@Z, the resize the private ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z 0x00B94540 (manifest label
//   "CEngineScene::ResizeGrid" is propagated; NAME_FIXES.tsv).
// Callees: CEngineSceneGrid::GetCellTotalPrimitiveCount(unsigned long, unsigned long) 0x00B91D20 (on GridLevels[0], (0, 0)),
//   ResizeLevels 0x00B94540; HighestSetBit (the landed 0x0099FDB0 body) inlined.
// Layout: as the landed CEngineSceneMap::Initialise 0x00B949A0 (MaxLevels +0x14, GridLevels +0x18 = CArray of 8-byte counted pointers,
//   Scene +0x24); the scene's primitives-per-cell float +0x54 (descriptive).
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/GFFloatToLongNear.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
__forceinline long __fastcall HighestSetBit(unsigned long value)
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
class CEngineSceneGrid { public:
    unsigned long GetCellTotalPrimitiveCount(unsigned long x, unsigned long y);   // 0x00B91D20
};
class CEngineScene { public:
    unsigned char _pad[0x54];
    float         PrimitivesPerCell;               // +0x54
};
class C2DBoxF { public: float MinX, MinY, MaxX, MaxY; };
class CEngineSceneMap { public:
    void*                                    _vfptr;       // +0x00
    C2DBoxF                                  Area;         // +0x04
    unsigned long                            MaxLevels;    // +0x14
    CArray<CCountedPointer<CEngineSceneGrid> > GridLevels; // +0x18
    CEngineScene*                            Scene;        // +0x24
    void Update();
private:
    __forceinline unsigned long CalculateLevel(float side) const
    {
        unsigned long span = 2 * GFFloatToLongNear(side) - 1;
        if ((long)span <= 1)
            span = 1;
        unsigned long levels = HighestSetBit(span) + 1;
        if (levels < 1)
            levels = 1;
        else if (levels > MaxLevels)
            levels = MaxLevels;
        return levels;
    }
    void ResizeLevels(long levels);                // 0x00B94540
};
void CEngineSceneMap::Update()
{
    unsigned long count = GridLevels[0]->GetCellTotalPrimitiveCount(0, 0);
    float side = (float)sqrt((float)count / Scene->PrimitivesPerCell);
    unsigned long span = 2 * GFFloatToLongNear(side) - 1;
    if ((long)span <= 1)
        span = 1;
    unsigned long levels = HighestSetBit(span) + 1;
    if (levels < 1)
        levels = 1;
    else if (levels > MaxLevels)
        levels = MaxLevels;
    if (levels != GridLevels.size())
        ResizeLevels(levels);
}
