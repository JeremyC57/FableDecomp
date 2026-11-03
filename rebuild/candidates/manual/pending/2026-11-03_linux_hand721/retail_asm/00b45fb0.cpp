// CEngineLightingManager::GetLightsInSphere(const C3DBoundingSphere& sphere, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme) -- clamp the light budget to 8; with local lights enabled, floor the sphere's XY extent into the 16x16 light lookup grid (1/16 world units per cell), walk every covered cell's link list and chain each not-yet-found light that is in one of the primitive's theme channels and whose radius reaches the sphere; then clear the found flags and clip/build the light array
// Grade: RELOCATION_MATCH (714 bytes)
// Leaf: ?GetLightsInSphere@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z
// Flags: none
// Retail-asm: 0x00B46011..0x00B4602B `fld [ebp-10h] ; fsub [ebp-1Ch] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-20h] ; fstp [ebp-0Ch] ; mov eax,[ebp-10h] ; cmp eax,[ebp-0Ch] ; jne ; inc dword ptr [ebp+8]` (and the same block at 0x00B46057, 0x00B4609C, 0x00B460E2) -- four expansions of the engine's inline-assembly floor helper GFFloatToLongFloor (standalone 0x0048D940; the same helper body as landed LookupSmallObjectLightCache 0x00B46A30). VC7.1 emits no bare dword fistp from C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B45FB0, length 714 bytes, ebp frame (0x28), `ret 0Ch`.
// Name: Ego_r.pdb + FableWin.pdb `?GetLightsInSphere@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z`;
//   called by GetLightsInSphereFast 0x00B46B90 (landed, binds this address).
// Callees: ResetAlreadyFoundFlags(long, long, long, long) 0x00B44CE0 (private, PDB AAEXJJJJ);
//   ClipLightCountAndBuildLightArray(CManagerLocalLight*, long, const C3DVector&, long, const CEnginePrimitiveThemeChannelDesc&) 0x00B44FE0 (private, PDB).
// Constants: 0.0625 (double) @ 0x012A2230 (literal).
// Levers: (1) the theme test is `bool inTheme = inChannel0 | (blend > 0 && mask & bit1)` -- the second && sits in an int
//   context, which is why retail materialises it with `mov eax,1 / xor eax,eax` and ORs bytes (`or al,bl`); a named
//   bool gives `mov al,1`. (2) C3DVector::GetSquaredMagnitude (X*X+Z*Z+Y*Y) gives retail's dz,dx,dy product order.
//   (3) `link->Light->AlreadyFoundFlag = true` before the re-link places the flag store where retail has it.
// Layout: engine/CEngineLightingManager.h (LookupGridWorldX/Y +0xC4/+0xC8, LocalLightsEnabled +0xCC, LightLookupGrid
//   +0xEC as CLocalLightElementLink*[16][16], EmptyLightArray +0x44EC); CManagerLocalLight / CLocalLightElementLink are
//   the PDB's nested types (Ego_r layouts: NextLocalLight +0x38, AlreadyFoundFlag +0x44; Light +4, Next +8).
#pragma optimize("y", off)
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
#include "engine/C3DVector.h"

class C3DBoundingSphere { public:
    C3DVector Center;
    float Radius;
};
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
    CEngineLocalLightArray* GetLightsInSphere(const C3DBoundingSphere& sphere, long maxLights,
                                              const CEnginePrimitiveThemeChannelDesc& theme);
private:
    void ResetAlreadyFoundFlags(long minX, long minY, long maxX, long maxY);
    CEngineLocalLightArray* ClipLightCountAndBuildLightArray(CManagerLocalLight* lights, long count,
        const C3DVector& centre, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme);
};

CEngineLocalLightArray* CEngineLightingManager::GetLightsInSphere(const C3DBoundingSphere& sphere, long maxLights,
                                                                  const CEnginePrimitiveThemeChannelDesc& theme)
{
    if (maxLights > 8)
        maxLights = 8;
    if (!LocalLightsEnabled)
        return reinterpret_cast<CEngineLocalLightArray*>(EmptyLightArray);
    long count = 0;
    CManagerLocalLight* lights = 0;
    long minX = GFFloatToLongFloor((sphere.Center.X - sphere.Radius - LookupGridWorldX) * 0.0625);
    long minY = GFFloatToLongFloor((sphere.Center.Y - sphere.Radius - LookupGridWorldY) * 0.0625);
    long maxX = GFFloatToLongFloor((sphere.Radius + sphere.Center.X - LookupGridWorldX) * 0.0625);
    long maxY = GFFloatToLongFloor((sphere.Radius + sphere.Center.Y - LookupGridWorldY) * 0.0625);
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
                    C3DVector offset = light->LightElement.WorldPos - sphere.Center;
                    float radius = light->LightElement.Radius + sphere.Radius;
                    if (offset.GetSquaredMagnitude() < radius * radius) {
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
    return ClipLightCountAndBuildLightArray(lights, count, sphere.Center, maxLights, theme);
}
