#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define FABLE_ENGINE_CENGINESCREENEFFECTDISPLACEMENTRENDERER_H
// Fixed-width storage adapter for the host only; VC7.1 uses the actual shared header.
struct CEngineScreenEffectDisplacementRenderer {
    unsigned int vptr; unsigned char pad04[4], _pad_0x08[4];
    unsigned int back, disp; int dispHandle, DynamicTexturePageHandle;
    unsigned char prefix[0xed-0x1c]; unsigned char EffectSupported, Enable2DDisplacement, Enable3DDisplacement;
};
#endif
// CEngineScreenEffectDisplacementRenderer::SetEffectQuality(const CEngineVideoQualitySettings&) -- sets the 2D/3D feature flags, initializes textures if either is requested and supported, otherwise frees its scratch page; retail 0x00B86B00 length 0xa3. Uses shared renderer layout and the video-quality twin's four-byte option prefix; callees 0x00B86A70 and 0x00F37030.
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
struct CEngineVideoQualitySettings { unsigned char _pad[0x6c]; unsigned char Enable2D; unsigned char Enable3D; };
struct CGPUScratchBufferTextureManager { void FreeMemoryPage(long); /* 0x00F37030 */ };
struct CEngineGlobal_01436e98 { unsigned char _pad[0x3190]; CGPUScratchBufferTextureManager scratch; };
extern CEngineGlobal_01436e98 *g_pEngine_01436e98; // 0x01436E98
struct CEngineScreenEffectDisplacementRenderer_Quality : CEngineScreenEffectDisplacementRenderer {
    bool InitialiseTextures(); // 0x00B86A70
    void FreeTexturePage() { if (DynamicTexturePageHandle >= 0) g_pEngine_01436e98->scratch.FreeMemoryPage(DynamicTexturePageHandle); }
    bool SetEffectQuality(const CEngineVideoQualitySettings&);
};
bool CEngineScreenEffectDisplacementRenderer_Quality::SetEffectQuality(const CEngineVideoQualitySettings& quality)
{
    unsigned char *bytes = reinterpret_cast<unsigned char *>(this);
    unsigned char supported = bytes[0xed];
    int enabled2D;
    if (supported) {
        if (quality.Enable2D) { enabled2D = 1; goto store2D; }
    }
    enabled2D = 0;
store2D:
    bytes[0xee] = enabled2D;
    int enabled3D;
    if (supported) {
        if (quality.Enable3D) { enabled3D = 1; goto store3D; }
    }
    enabled3D = 0;
store3D:
    bytes[0xef] = enabled3D;
    if (!Enable2DDisplacement && !Enable3DDisplacement) {
        FreeTexturePage();
        _pad_0x08[0] = false;
        return true;
    }
    if (InitialiseTextures()) {
        _pad_0x08[0] = true;
        return true;
    }
    FreeTexturePage();
    _pad_0x08[0] = false;
    return false;
}

static int freeCalls, initCalls, expectedHandle, expectedInit, event;
static bool wrong;
static CEngineScreenEffectDisplacementRenderer_Quality* expectedRenderer;
static CEngineGlobal_01436e98 engine;
CEngineGlobal_01436e98* g_pEngine_01436e98=&engine;
void CGPUScratchBufferTextureManager::FreeMemoryPage(long handle) {
    if(this!=&engine.scratch || handle!=expectedHandle || event!=(expectedInit?1:0)) wrong=true;
    ++freeCalls; event=2;
}
static bool initializeResult;
bool CEngineScreenEffectDisplacementRenderer_Quality::InitialiseTextures() {
    if(this!=expectedRenderer || event!=0) wrong=true;
    ++initCalls;event=1;return initializeResult;
}
int main() {
    // All combinations of capability, requested modes, initialization outcome and page validity.
    for(int support=0;support<2;++support) for(int two=0;two<2;++two) for(int three=0;three<2;++three)
    for(int init=0;init<2;++init) for(int page=0;page<2;++page) {
        CEngineScreenEffectDisplacementRenderer_Quality r; std::memset(&r,0,sizeof(r));
        CEngineVideoQualitySettings q;std::memset(&q,0,sizeof(q));
        r.EffectSupported=(unsigned char)support;q.Enable2D=(unsigned char)two;q.Enable3D=(unsigned char)three;
        r.DynamicTexturePageHandle=page?17:-1;expectedHandle=r.DynamicTexturePageHandle;
        expectedRenderer=&r;expectedInit=support && (two||three);initializeResult=init!=0;
        r._pad_0x08[0]=7; r._pad_0x08[1]=0xa5;
        freeCalls=initCalls=event=0;wrong=false;
        bool result=r.SetEffectQuality(q);
        bool wantResult=!expectedInit || init;
        int wantFree=page && (!expectedInit || !init);
        if(wrong || result!=wantResult || initCalls!=expectedInit || freeCalls!=wantFree ||
           r.Enable2DDisplacement!=(support&&two) || r.Enable3DDisplacement!=(support&&three) ||
           r._pad_0x08[0]!=(expectedInit&&init) || r._pad_0x08[1]!=0xa5 ||
           r.DynamicTexturePageHandle!=expectedHandle) return 1;
    }
    std::printf("00b86b00_TEST PASS\n");return 0;
}