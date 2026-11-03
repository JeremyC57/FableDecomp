// CEngineLightingManager::GetLightsInArea(const C3DBoundingBox& box, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme) -- clamp the light budget to 8; with local lights enabled, floor the box's XY extent into the 16x16 light lookup grid, walk every covered cell's link list and chain each not-yet-found light in one of the primitive's theme channels whose radius reaches the box (shortest box-to-light distance squared below its RadiusSquared); then clear the found flags and clip/build the light array around the box centre
// Grade: RELOCATION_MATCH (982 bytes)
// Leaf: ?GetLightsInArea@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingBox@@JABVCEnginePrimitiveThemeChannelDesc@@@Z
// Flags: none
// Retail-asm: 0x00B462DE..0x00B462F8 `fld [ebp-10h] ; fsub [ebp-20h] ; fistp dword ptr [ebp-14h] ; fild [ebp-14h] ; fadd [ebp-24h] ; fstp [ebp-0Ch] ; mov eax,[ebp-10h] ; cmp eax,[ebp-0Ch] ; jne ; inc dword ptr [ebp-14h]` (x4: GFFloatToLongFloor, standalone 0x0048D940) and 0x00B464A2..0x00B464E2 `mov ecx,[ebp+8] ; mov edx,[ebp-2Ch] ; movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [ebp-30h],xmm2` -- C3DBoundingBox::CalcShortestDistanceToPointSquared's hand SSE kernel behind g_SSEAvailable (0x013D2880), the same kernel as landed 0x00BF7DF0; VC7.1 emits neither a bare dword fistp nor packed SSE from C++. The x87 branch (0x00B464EF..) is the C++ fallback.
// Portable (2026-11-03 asm twins): the floor block is the shared engine/GFFloatToLongFloor.h; the C3DBoundingBox::CalcShortestDistanceToPointSquared SSE kernel is this TU's own inline member (the landed 0x00B49810 / 0x00BF7DF0 rows carry it the same way), kept here in two-path form: under FABLE_PORTABLE the #ifndef drops the SSE branch and the member runs retail's own non-SSE C++ path.
// Helper (2026-11-03 asm twins): the floor block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); the SSE kernel stays in this row (see Portable).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B46280, length 982 bytes, ebp frame (0x48), `ret 0Ch`.
// Name: Ego_r.pdb + FableWin.pdb `?GetLightsInArea@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingBox@@JABVCEnginePrimitiveThemeChannelDesc@@@Z`;
//   called by LookupSmallObjectLightCache 0x00B46A30 (landed, `call 0xb46280`).
// Callees: ResetAlreadyFoundFlags 0x00B44CE0 and ClipLightCountAndBuildLightArray 0x00B44FE0 (both private in the PDB).
// Constants: 0.0625 (double) @ 0x012A2230, 0.5f @ 0x0122F59C (literals). Globals: g_SSEAvailable 0x013D2880.
// Inlines: C3DBoundingBox::CalcShortestDistanceToPointSquared and C3DBoundingBox::GetCenter (PDB names; engine/C3DBoundingBox.h
//   FABLE_RETAIL_C3DBOUNDINGBOX branch), C3DVector::GetSquaredMagnitude (engine/C3DVector.h).
// Shape: the grid walk is GetLightsInSphere's (0x00B45FB0); here the flag store goes through link->Light (retail reloads it).
#pragma optimize("y", off)
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
#include "engine/C3DVector.h"

extern bool g_SSEAvailable;                             // 0x013D2880
#define FABLE_RETAIL_C3DBOUNDINGBOX
#include "engine/C3DBoundingBox.h"

// the engine's C3DBoundingBox::CalcShortestDistanceToPointSquared (Ego_r QBEMABVC3DVector@@@Z), expanded in place
__forceinline float C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const
{
#ifndef FABLE_PORTABLE   // portable build: the retail non-SSE C++ path below (the twin of the SSE asm kernel)
    if (g_SSEAvailable) {
        float result;
        __asm {
            mov     ecx, this
            mov     edx, point
            movss   xmm0, [ecx+8]
            movhps  xmm0, [ecx]
            movss   xmm1, [ecx+14h]
            movhps  xmm1, [ecx+0Ch]
            movss   xmm2, [edx+8]
            movhps  xmm2, [edx]
            movaps  xmm3, xmm2
            maxps   xmm2, xmm0
            minps   xmm2, xmm1
            subps   xmm2, xmm3
            mulps   xmm2, xmm2
            movaps  xmm3, xmm2
            shufps  xmm2, xmm2, 0B0h
            addps   xmm2, xmm3
            shufps  xmm2, xmm2, 3
            addss   xmm2, xmm3
            movss   result, xmm2
        }
        return result;
    }
#endif
    C3DVector closest(point);
    if (closest.X < MinCoord.X)
        closest.X = MinCoord.X;
    else if (closest.X > MaxCoord.X)
        closest.X = MaxCoord.X;
    if (closest.Y < MinCoord.Y)
        closest.Y = MinCoord.Y;
    else if (closest.Y > MaxCoord.Y)
        closest.Y = MaxCoord.Y;
    if (closest.Z < MinCoord.Z)
        closest.Z = MinCoord.Z;
    else if (closest.Z > MaxCoord.Z)
        closest.Z = MaxCoord.Z;
    return (point - closest).GetSquaredMagnitude();
}

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
};
class CRGBFloatColour { public: float R, G, B, A; };
class CEngineLocalLightElement { public:
    C3DVector WorldPos;
    float Radius;
    float RadiusSquared;
    float SpotRadius;
    CRGBFloatColour Colour;
    float AttenuationDistSquared;
    float AttenuationConstant;
    unsigned long ChannelMask;
};

#include "engine/GFFloatToLongFloor.h"

class CEngineLocalLightArray;

class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    struct CManagerLocalLight {
        virtual ~CManagerLocalLight();
        CEngineLocalLightElement LightElement;
        CManagerLocalLight* NextLocalLight;
        float ClippingScore;
        float ThemeWeight;
        bool AlreadyFoundFlag;
    };
    struct CLocalLightElementLink {
        int _padding_;
        CManagerLocalLight* Light;
        CLocalLightElementLink* Next;
    };
    CEngineLocalLightArray* GetLightsInArea(const C3DBoundingBox& box, long maxLights,
                                              const CEnginePrimitiveThemeChannelDesc& theme);
private:
    void ResetAlreadyFoundFlags(long minX, long minY, long maxX, long maxY);
    CEngineLocalLightArray* ClipLightCountAndBuildLightArray(CManagerLocalLight* lights, long count,
        const C3DVector& centre, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme);
};

CEngineLocalLightArray* CEngineLightingManager::GetLightsInArea(const C3DBoundingBox& box, long maxLights,
                                                                  const CEnginePrimitiveThemeChannelDesc& theme)
{
    if (maxLights > 8)
        maxLights = 8;
    if (!LocalLightsEnabled)
        return reinterpret_cast<CEngineLocalLightArray*>(EmptyLightArray);
    long count = 0;
    CManagerLocalLight* lights = 0;
    long minX = GFFloatToLongFloor((box.MinCoord.X - LookupGridWorldX) * 0.0625);
    long minY = GFFloatToLongFloor((box.MinCoord.Y - LookupGridWorldY) * 0.0625);
    long maxX = GFFloatToLongFloor((box.MaxCoord.X - LookupGridWorldX) * 0.0625);
    long maxY = GFFloatToLongFloor((box.MaxCoord.Y - LookupGridWorldY) * 0.0625);
    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX > 15) maxX = 15;
    if (maxY > 15) maxY = 15;
    CLocalLightElementLink* (*grid)[16] = reinterpret_cast<CLocalLightElementLink* (*)[16]>(LightLookupGrid);
    for (long y = minY; y <= maxY; ++y) {
        for (long x = minX; x <= maxX; ++x) {
            for (CLocalLightElementLink* link = grid[x][y]; link; link = link->Next) {
                CManagerLocalLight* light = link->Light;
                bool inChannel0 = theme.ChannelBlendFactor < 0xFF && (light->LightElement.ChannelMask & (1 << theme.ChannelIndex0));
                bool inTheme = inChannel0 | (theme.ChannelBlendFactor > 0 && (light->LightElement.ChannelMask & (1 << theme.ChannelIndex1)));
                if (!light->AlreadyFoundFlag && inTheme) {
                    if (box.CalcShortestDistanceToPointSquared(light->LightElement.WorldPos) < light->LightElement.RadiusSquared) {
                        link->Light->AlreadyFoundFlag = true;
                        light->NextLocalLight = lights;
                        lights = light;
                        ++count;
                    }
                }
            }
        }
    }
    ResetAlreadyFoundFlags(minX, minY, maxX, maxY);
    return ClipLightCountAndBuildLightArray(lights, count, box.GetCenter(), maxLights, theme);
}
