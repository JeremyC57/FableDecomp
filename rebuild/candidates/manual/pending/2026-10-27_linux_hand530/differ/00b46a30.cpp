// CEngineLightingManager::LookupSmallObjectLightCache(float x, float y, long layerMask) -- the cached light array of the 4x4 world cell under a small object: floor the cell indices, reject cells outside the 64x64 grid, and fill an empty cell once with the lights in the cell's 6-unit-square column
// Grade: DIFFER(322v350) -- retail_asm candidate; everything up to the box build matches; retail keeps BOTH C3DVector temporaries (frame 0x40, min at [ebp-1Ch], max at [ebp-28h], built max-first) and copies them member-wise into the box through edx/eax, ours (frame 0x34) builds the min vector straight into the box | tried: inline / forceinline box + vector ctors, temporaries vs named locals, asm-in-helper vs asm-in-body, C compare | next: non-inline (declared-only or noinline) C3DVector copy ctor with forceinline box ctor, or a C3DBoundingBox(const C3DVector&, const C3DVector&) body that assigns members
// Leaf: ?LookupSmallObjectLightCache@CEngineLightingManager@@AAEPAVCEngineLocalLightArray@@MMJ@Z
// Retail-asm: 0x00B46A56..0x00B46A70 and 0x00B46A93..0x00B46AAD `fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-4] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp+8]` -- two expansions of the engine's inline-assembly floor helper GFFloatToLongFloor (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in one 10-instruction block; standalone copy landed at 0x0048D940). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si); the results are read back by C++ after each block.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B46A30, length 350 bytes, ebp frame, `ret 0Ch`.
// Name: Ego_r.pdb `?LookupSmallObjectLightCache@CEngineLightingManager@@AAEPAVCEngineLocalLightArray@@MMJ@Z` (private) -- donor
//   order puts it right before GetLightsInSphereFast, its only caller (0x00B46B90 twice: `call 0xb46a30`).
// Callees: CEngineLightingManager::GetLightsInArea(const C3DBoundingBox&, long, const CEnginePrimitiveThemeChannelDesc&) @ 0x00B46280.
// Constants: 0.25 (double) @ 0x01238010, 6.0f @ 0x0125A2A0.
// Shape: each floored cell index is homed in its dead float argument slot; the theme descriptor (three zero bytes, the
//   CEnginePrimitiveThemeChannelDesc(uchar, uchar, uchar) constructor) reuses x's slot; the box is built from two C3DVector
//   temporaries through C3DVector's memberwise copy constructor.
// Layout: engine/CEngineLightingManager.h (retail offsets: LookupGridWorldX +0xC4, LookupGridWorldY +0xC8,
//   SmallObjectLookupGrid +0x4EC as CEngineLocalLightArray*[64][64]).
#pragma optimize("y", off)
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager

class C3DVector { public:
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C3DBoundingBox { public:
    C3DVector MinCoord;
    C3DVector MaxCoord;
    __forceinline C3DBoundingBox(const C3DVector& minCoord, const C3DVector& maxCoord) : MinCoord(minCoord), MaxCoord(maxCoord) { }
};
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
    CEnginePrimitiveThemeChannelDesc(unsigned char index0, unsigned char index1, unsigned char blend)
        : ChannelIndex0(index0), ChannelIndex1(index1), ChannelBlendFactor(blend) { }
};

// the engine's inline-assembly FPU floor helper (standalone copy 0x0048D940, ?GFFloatToLongFloor@@YIJM@Z): expanded in place
__forceinline long GFFloatToLongFloor(float value)
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

class CEngineLocalLightArray;

class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    CEngineLocalLightArray* GetLightsInArea(const C3DBoundingBox& box, long layerMask,
                                            const CEnginePrimitiveThemeChannelDesc& theme);   // 0x00B46280
private:
    CEngineLocalLightArray* LookupSmallObjectLightCache(float x, float y, long layerMask);
};

CEngineLocalLightArray* CEngineLightingManager::LookupSmallObjectLightCache(float x, float y, long layerMask)
{
    long cellX = GFFloatToLongFloor((x - LookupGridWorldX) * 0.25);
    long cellY = GFFloatToLongFloor((y - LookupGridWorldY) * 0.25);
    if (cellX < 0 || cellY < 0 || cellX >= 64 || cellY >= 64)
        return 0;
    CEngineLocalLightArray** cell = &reinterpret_cast<CEngineLocalLightArray* (*)[64]>(SmallObjectLookupGrid)[cellX][cellY];
    if (*cell == 0) {
        float minX = (float)(cellX * 4) + LookupGridWorldX;
        float minY = (float)(cellY * 4) + LookupGridWorldY;
        *cell = GetLightsInArea(C3DBoundingBox(C3DVector(minX, minY, -1e30f), C3DVector(minX + 6.0f, minY + 6.0f, 1e30f)),
                                layerMask, CEnginePrimitiveThemeChannelDesc(0, 0, 0));
    }
    return *cell;
}
