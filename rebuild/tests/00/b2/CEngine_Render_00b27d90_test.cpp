// Runs the production Render and FlushRender bodies and the shared RAII shader guard.
// The device, scene loaders and shader backend record side effects instead of doing GPU work.
#include <stdio.h>
#include <string.h>
static char trace[80];
static int used;
static void event(char value) {if(used<78) {trace[used++]=value; trace[used]=0;}}
class RecordingDevice {
public:
    void SetStreamSource(int s,void* p,int o,int n) {event((s||p||o||n)?'!':'V');}
    void SetIndices(void* p) {event(p?'!':'I');}
    void SetTexture(long stage,void* p) {event(p?'!':char('0'+stage));}
};
class CRenderManagerCore {
public:
    RecordingDevice* PD3DDevice;
    long TextureStagesInUse;
    void* CurrentlySelectedTextures[8];
    void FlushRender();
};
class CRenderManager : public CRenderManagerCore {};
class CShaderRenderManager {
public:
    bool vertex,pixel;
    static CShaderRenderManager& Get() {static CShaderRenderManager state;return state;}
    void SetVertexShadersEnable(bool enable,bool* old) {
        if(old) *old=vertex;
        if(enable!=vertex) {vertex=enable;event(enable?'v':'w');}
    }
    void SetPixelShadersEnable(bool enable,bool* old) {
        if(old) *old=pixel;
        if(enable!=pixel) {pixel=enable;event(enable?'p':'q');}
    }
};
#define FABLE_ENGINE_CSHADERRENDERMANAGER_H
#include "engine/CScopedShadersEnable.h"
class CEngineInterpolationInfo {};
class CEngine {
public:
    bool EnableEngine,NewSceneFlag,CutscenePreloadFlag,CameraAngleChanged;
    bool changeDuringFrame;
    const CEngineInterpolationInfo* expected;
    void UpdateOffGameTickProcessInfo(bool enabled) {event(enabled?'T':'t');}
    void LoadNewScene(const CEngineInterpolationInfo& i) {event(&i==expected?'L':'!');}
    void RenderFrame(const CEngineInterpolationInfo& i) {
        CShaderRenderManager& shaders=CShaderRenderManager::Get();
        event(shaders.vertex && shaders.pixel && &i==expected?'R':'!');
        if(changeDuringFrame) shaders.vertex=shaders.pixel=false;
    }
    void Render(const CEngineInterpolationInfo& interpolation,unsigned long render_flags);
};
namespace NEngineInternal {
    extern CRenderManager* GPRenderManager;
}

// Inline-budget: plain inline leaves a call; the 00b27db6 device reset is expanded in the retail caller.
inline void CRenderManagerCore::FlushRender()
{
    PD3DDevice->SetStreamSource(0, 0, 0, 0);
    PD3DDevice->SetIndices(0);
    for (long c_stage = 0; c_stage < TextureStagesInUse; ++c_stage) {
        if (CurrentlySelectedTextures[c_stage]) {
            CurrentlySelectedTextures[c_stage] = 0;
            PD3DDevice->SetTexture(c_stage, 0);
        }
    }
    TextureStagesInUse = 0;
}

void CEngine::Render(const CEngineInterpolationInfo& interpolation, unsigned long render_flags)
{
    if (EnableEngine) {
        UpdateOffGameTickProcessInfo((render_flags & 1) != 0);
        NEngineInternal::GPRenderManager->FlushRender();
        CScopedShadersEnable shaders_enable(true, true);
        if (NewSceneFlag || CutscenePreloadFlag || CameraAngleChanged)
            LoadNewScene(interpolation);
        RenderFrame(interpolation);
    }
}

namespace NEngineInternal {CRenderManager* GPRenderManager;}
int main() {
    CEngine engine; CEngineInterpolationInfo info; engine.expected=&info;
    RecordingDevice device; CRenderManager render; render.PD3DDevice=&device;
    NEngineInternal::GPRenderManager=&render;
    CShaderRenderManager& shader=CShaderRenderManager::Get();
    int token=0;
    for(unsigned cfg=0;cfg<1024;++cfg) {
        engine.EnableEngine=(cfg&1)!=0; engine.NewSceneFlag=(cfg&2)!=0;
        engine.CutscenePreloadFlag=(cfg&4)!=0; engine.CameraAngleChanged=(cfg&8)!=0;
        bool v=(cfg&16)!=0,p=(cfg&32)!=0; shader.vertex=v; shader.pixel=p;
        unsigned long flags=(cfg&64)?3:2; engine.changeDuringFrame=(cfg&128)!=0;
        long n=(cfg&256)?3:0; bool holes=(cfg&512)!=0;
        render.TextureStagesInUse=n;
        for(int i=0;i<8;++i)render.CurrentlySelectedTextures[i]=(holes && i==1)?0:&token;
        used=0;trace[0]=0;engine.Render(info,flags);
        char want[80]="";
        if(engine.EnableEngine) {
            strcat(want,(flags&1)?"TVI":"tVI");
            if(n)strcat(want,holes?"02":"012");
            if(!v)strcat(want,"v");
            if(!p)strcat(want,"p");
            if(cfg&14)strcat(want,"L");
            strcat(want,"R");
            if(engine.changeDuringFrame) {if(v)strcat(want,"v");if(p)strcat(want,"p");}
            else {if(!v)strcat(want,"w");if(!p)strcat(want,"q");}
        }
        if(strcmp(trace,want) || shader.vertex!=v || shader.pixel!=p) return 1;
        if(render.TextureStagesInUse!=(engine.EnableEngine?0:n))return 2;
        for(int j=0;j<8;++j) {
            bool cleared=(engine.EnableEngine&&j<n)||(holes&&j==1);
            if(render.CurrentlySelectedTextures[j]!=(cleared?0:&token))return 3;
        }
    }
    puts("00b27d90_TEST PASS");return 0;
}