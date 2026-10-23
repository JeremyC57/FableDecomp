// CEngineLightingManager::ClipLightCountAndBuildLightArray(CManagerLocalLight* lights, long lightCount, const C2DVector& position, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme) -- weight each candidate light by the primitive's theme channels; when there are more lights than fit, score them by weight / max(1, XY distance squared), re-link them sorted by score and keep maxLights; copy the kept light elements (colour scaled by the weight when blending) into a frame-pool CEngineLocalLightArray, or return the shared empty array
// Leaf: ClipLightCountAndBuildLightArray
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B44FE0, 485 bytes (+int3 padding), `ret 14h`. Called from 0x00B4626C and 0x00B46648.
// Callees: CSimpleMemoryPool::Allocate 0x00A5C910 on the engine's frame pool (CEngine 0x01436EA4, pool at retail +0x130;
//   engine/CEngine.h puts FrameMemoryPool at +0x138, so the engine is a local model). Constants 1/255 0x01231724, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CEngineLightingManager::CManagerLocalLight (LightElement +4, NextLocalLight +0x38, ClippingScore +0x3c,
//   ThemeWeight +0x40), CEngineLocalLightElement (0x34 bytes: Colour +0x18, ChannelMask +0x30), CEngineLocalLightArray
//   (LightCount, Lights[]), CEnginePrimitiveThemeChannelDesc (ChannelIndex0/1, ChannelBlendFactor). EmptyLightArray is at
//   retail +0x44ec (PDB / engine/CEngineLightingManager.h +0x44f8: its three CArray members are 16 bytes where retail's
//   STLport vectors are 12), hence a local model of `this`.
// Shape: the clamp is a MAX macro over an inline XY distance helper, so the distance is computed twice (compared, popped,
//   recomputed) as in retail; the insertion sort walks the `lights` PARAMETER itself (`current = lights; lights =
//   lights->Next`) for retail's register use, and its inner test keeps the loop-invariant `maxLights > 0`; the copy loop is
//   `while (light && i < maxLights)` and the weight scaling an inline CRGBFloatColour::operator*= (retail's strength-reduced
//   element pointer biased to the colour's B word).
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
#define MAX(a, b) ((a) > (b) ? (a) : (b))
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour {
    float R, G, B, A;
    void operator*=(float f) { R *= f; G *= f; B *= f; A *= f; }
};

struct CEngineLocalLightElement {                  // 0x34 bytes
    C3DVector       WorldPos;                      // +0x00
    float           Radius;                        // +0x0c
    float           RadiusSquared;                 // +0x10
    float           SpotRadius;                    // +0x14
    CRGBFloatColour Colour;                        // +0x18
    float           AttenuationDistSquared;        // +0x28
    float           AttenuationConstant;           // +0x2c
    unsigned long   ChannelMask;                   // +0x30
};
struct CEngineLocalLightArray {
    unsigned long            LightCount;           // +0x00
    CEngineLocalLightElement Lights[8];            // +0x04
};
struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
};
static inline float DistanceSquaredXY(const C3DVector& a, const C2DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
struct CManagerLocalLight {
    virtual void v00();
    CEngineLocalLightElement LightElement;         // +0x04
    CManagerLocalLight*      NextLocalLight;       // +0x38
    float                    ClippingScore;        // +0x3c
    float                    ThemeWeight;          // +0x40
    bool                     AlreadyFoundFlag;     // +0x44
};
struct CSimpleMemoryPool {
    void* Allocate(unsigned long size);            // 0x00A5C910
};
struct CEngine {
    unsigned char     _pad[0x130];
    CSimpleMemoryPool FrameMemoryPool;             // +0x130
};
extern CEngine* g_pEngine_01436ea4;                         // 0x01436EA4

struct CEngineLightingManager {
    unsigned char          _pad[0x44ec];
    CEngineLocalLightArray EmptyLightArray;        // +0x44ec
    CEngineLocalLightArray* ClipLightCountAndBuildLightArray(CManagerLocalLight* lights, long lightCount, const C2DVector& position,
                                                             long maxLights, const CEnginePrimitiveThemeChannelDesc& theme);
};

CEngineLocalLightArray* CEngineLightingManager::ClipLightCountAndBuildLightArray(CManagerLocalLight* lights, long lightCount,
    const C2DVector& position, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme)
{
    if (theme.ChannelBlendFactor > 0 || lightCount + 1 > maxLights) {
        for (CManagerLocalLight* light = lights; light; light = light->NextLocalLight) {
            light->ThemeWeight = 0.0f;
            if (theme.ChannelBlendFactor < 255 && (light->LightElement.ChannelMask & (1 << theme.ChannelIndex0)))
                light->ThemeWeight = 1.0f - theme.ChannelBlendFactor * (1.0f / 255.0f);
            if (theme.ChannelBlendFactor > 0 && (light->LightElement.ChannelMask & (1 << theme.ChannelIndex1)))
                light->ThemeWeight += theme.ChannelBlendFactor * (1.0f / 255.0f);
        }
    }
    if (lightCount + 1 > maxLights) {
        for (CManagerLocalLight* light = lights; light; light = light->NextLocalLight) {
            light->ClippingScore = light->ThemeWeight
                / MAX(1.0f, DistanceSquaredXY(light->LightElement.WorldPos, position));
        }
        CManagerLocalLight* sorted = 0;
        while (lights) {
            CManagerLocalLight* current = lights;
            lights = lights->NextLocalLight;
            CManagerLocalLight* prev = 0;
            CManagerLocalLight* it = sorted;
            while (it && it->ClippingScore > current->ClippingScore && maxLights > 0) {
                prev = it;
                it = it->NextLocalLight;
            }
            if (prev)
                prev->NextLocalLight = current;
            else
                sorted = current;
            current->NextLocalLight = it;
        }
        lights = sorted;
        lightCount = maxLights;
    }
    if (lightCount == 0)
        return &EmptyLightArray;
    CEngineLocalLightArray* array = (CEngineLocalLightArray*)g_pEngine_01436ea4->FrameMemoryPool.Allocate(lightCount * sizeof(CEngineLocalLightElement) + 4);
    array->LightCount = lightCount;
    long i = 0;
    for (CManagerLocalLight* light = lights; light && i < maxLights; light = light->NextLocalLight, ++i) {
        array->Lights[i] = light->LightElement;
        if (theme.ChannelBlendFactor > 0)
            array->Lights[i].Colour *= light->ThemeWeight;
    }
    return array;
}