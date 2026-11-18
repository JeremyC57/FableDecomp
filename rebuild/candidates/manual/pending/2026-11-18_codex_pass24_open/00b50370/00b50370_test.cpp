#include <cassert>
#include <cstdio>

enum ETechnique { SHADOW_RENDERING_TECHNIQUE_NONE };

class C3DVector
{
public:
    float X,Y,Z;
    void Initialise(float x,float y,float z) { X=x; Y=y; Z=z; }
};

class CEngineShadowRenderer
{
public:
    struct { float ShadowFadeEnd, ShadowFadeStart; } SceneData;
    float MaxShadowDistance, ForegroundBufferSpan, ShadowProjectionZOffset, ShadowProjectionZSlopeOffset;
    float ColourBufferShadowTextureZBias, ColourBufferShadowMinWClamp, ShadowFadeRatio;
    float PerspectiveInterpStart, PerspectiveInterpEnd, UserVideoOptionShadowDistanceScale;
    bool TexturesAllocated, SceneOriginLocked, PerspectiveShadowsEnabled, EnableFrustrumDebugDisplay;
    long UserVideoOptionShadowBufferSize;
    ETechnique RenderingTechnique;
    C3DVector ShadowBufferTextureUVScaler;
    CEngineShadowRenderer();
    void InitialiseTextures()
    {
        assert(SceneData.ShadowFadeEnd==32.0f && MaxShadowDistance==32.0f);
        assert(SceneData.ShadowFadeStart==8.0f && ForegroundBufferSpan==8.0f);
        assert(ShadowProjectionZOffset==0.0001f && ShadowProjectionZSlopeOffset==2.0f);
        assert(ColourBufferShadowTextureZBias==-0.001f && ColourBufferShadowMinWClamp==0.1f);
        assert(!TexturesAllocated && !SceneOriginLocked && ShadowFadeRatio==0.5f);
        assert(PerspectiveInterpStart==0.1f && PerspectiveInterpEnd==0.2f);
        assert(UserVideoOptionShadowBufferSize==0x400 && UserVideoOptionShadowDistanceScale==1.0f);
        assert(RenderingTechnique==SHADOW_RENDERING_TECHNIQUE_NONE && PerspectiveShadowsEnabled && !EnableFrustrumDebugDisplay);
        assert(ShadowBufferTextureUVScaler.X==1.0f && ShadowBufferTextureUVScaler.Y==1.0f && ShadowBufferTextureUVScaler.Z==1.0f);
        TexturesAllocated=true;
    }
};

// The selected implementation is reproduced verbatim for this standalone fixture.
CEngineShadowRenderer::CEngineShadowRenderer()
{
    SceneData.ShadowFadeEnd = 32.0f;
    MaxShadowDistance = 32.0f;
    SceneData.ShadowFadeStart = 8.0f;
    ForegroundBufferSpan = 8.0f;
    ShadowProjectionZOffset = 0.0001f;
    ShadowProjectionZSlopeOffset = 2.0f;
    ColourBufferShadowTextureZBias = -0.001f;
    ColourBufferShadowMinWClamp = 0.1f;
    TexturesAllocated = false;
    SceneOriginLocked = false;
    ShadowFadeRatio = 0.5f;
    PerspectiveInterpStart = 0.1f;
    PerspectiveInterpEnd = 0.2f;
    UserVideoOptionShadowBufferSize = 0x400;
    UserVideoOptionShadowDistanceScale = 1.0f;
    RenderingTechnique = SHADOW_RENDERING_TECHNIQUE_NONE;
    PerspectiveShadowsEnabled = true;
    EnableFrustrumDebugDisplay = false;
    ShadowBufferTextureUVScaler.Initialise(1.0f, 1.0f, 1.0f);
    InitialiseTextures();
}


int main()
{
    CEngineShadowRenderer renderer;
    assert(renderer.TexturesAllocated);
    std::puts("00b50370_TEST PASS");
    return 0;
}
