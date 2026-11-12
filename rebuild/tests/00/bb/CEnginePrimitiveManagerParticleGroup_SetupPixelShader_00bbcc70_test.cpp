// Behaviour test for CEnginePrimitiveManagerParticleGroup::SetupPixelShader (0x00BBCC70): a copy of the logic over small models
// of the render-state cache, the texture-stage cache and the shader manager.
#include <cstdio>

struct CStateInfo { unsigned long CurrentState, DesiredState, BookmarkMask; bool DirtyListFlag; };
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
class CTexture { public: void* PD3DTexture; };
class CPixelShader { public: int id; };

static unsigned long D3DTextureUVModeTable[4] = { 1, 2, 3, 4 };
enum TEXTURE_UV_MODE { TEXTURE_UV_WRAP = 0 };

class CRenderManager { public:
    CStateStackElement StateStack[16];
    CStateInfo* StateUpdateList[16];
    long StateStackSize, StateUpdateListSize;
    unsigned long CurrentBookmarkMask;
    CStateInfo TextureUVModeU[8], TextureUVModeV[8];
    long TextureStagesInUse;
    void* CurrentlySelectedTextures[8];
    int DeviceSetTextureCalls; long LastStage; void* LastTexture;

    void AddStateToUpdateList(CStateInfo& state)
    {
        if (!state.DirtyListFlag) {
            state.DirtyListFlag = true;
            StateUpdateList[StateUpdateListSize] = &state;
            ++StateUpdateListSize;
        }
    }
    void SetRenderState(unsigned long value, CStateInfo& state)
    {
        if (state.DesiredState != value) {
            if ((CurrentBookmarkMask & state.BookmarkMask) == 0) {
                state.BookmarkMask |= CurrentBookmarkMask;
                StateStack[StateStackSize].State = &state;
                StateStack[StateStackSize].Value = state.DesiredState;
                ++StateStackSize;
            }
            state.DesiredState = value;
            AddStateToUpdateList(state);
        }
    }
    void SetTextureUVModeU(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], TextureUVModeU[stage]); }
    void SetTextureUVModeV(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], TextureUVModeV[stage]); }
    void AttachTextureToStage(long stage, const CTexture* texture)
    {
        void* d3d = texture ? texture->PD3DTexture : 0;
        if (CurrentlySelectedTextures[stage] != d3d) {
            CurrentlySelectedTextures[stage] = d3d;
            ++DeviceSetTextureCalls; LastStage = stage; LastTexture = d3d;
        }
        if (TextureStagesInUse < stage + 1)
            TextureStagesInUse = stage + 1;
    }
};
class CShaderRenderManager { public:
    unsigned long Reg; float X, Y, Z, W; int ConstantCalls;
    const CPixelShader* Applied; int ApplyCalls;
    void SetUserVertexShaderConstant(unsigned long reg, float x, float y, float z, float w) { Reg = reg; X = x; Y = y; Z = z; W = w; ++ConstantCalls; }
    void ApplyPixelShader(const CPixelShader& shader) { Applied = &shader; ++ApplyCalls; }
};
class CEngineResourceManager { public:
    CTexture StipleFadeTexture;
    CTexture& GetStipleFadeTexture() { return StipleFadeTexture; }
};

static CRenderManager* GPRenderManager;
static CShaderRenderManager* GPShaderRenderManager;
static CEngineResourceManager* GPResourceManager;

class CEnginePrimitiveManagerParticleGroup { public:
    bool WriteZBuffer;
    void SetupPixelShader(unsigned long reg, const CPixelShader& stipple_shader, const CPixelShader& normal_shader)
    {
        if (WriteZBuffer) {
            GPShaderRenderManager->SetUserVertexShaderConstant(reg, 1.0f / 32.0f, 1.0f / 32.0f, 320.0f, 240.0f);
            GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
            GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
            GPRenderManager->AttachTextureToStage(1, &GPResourceManager->GetStipleFadeTexture());
            GPShaderRenderManager->ApplyPixelShader(stipple_shader);
        } else {
            GPShaderRenderManager->SetUserVertexShaderConstant(reg, 0.0f, 0.0f, 0.0f, 0.0f);
            GPShaderRenderManager->ApplyPixelShader(normal_shader);
        }
    }
};

static CRenderManager rm;
static CShaderRenderManager sm;
static CEngineResourceManager res;

int main()
{
    int tag = 0;
    bool ok = true;
    GPRenderManager = &rm; GPShaderRenderManager = &sm; GPResourceManager = &res;
    res.StipleFadeTexture.PD3DTexture = &tag;
    CPixelShader stipple, normal; stipple.id = 1; normal.id = 2;
    CEnginePrimitiveManagerParticleGroup group;

    // 1. Non z-writing group: zero constant, normal shader, no render state or texture touched.
    rm.CurrentBookmarkMask = 4;
    rm.TextureUVModeU[1].DesiredState = 3;       // clamp
    rm.TextureUVModeV[1].DesiredState = 1;       // already wrap
    rm.TextureUVModeV[1].BookmarkMask = 0;
    group.WriteZBuffer = false;
    group.SetupPixelShader(7, stipple, normal);
    ok = ok && sm.ConstantCalls == 1 && sm.Reg == 7 && sm.X == 0.0f && sm.Y == 0.0f && sm.Z == 0.0f && sm.W == 0.0f;
    ok = ok && sm.ApplyCalls == 1 && sm.Applied == &normal;
    ok = ok && rm.StateStackSize == 0 && rm.StateUpdateListSize == 0 && rm.DeviceSetTextureCalls == 0 && rm.TextureStagesInUse == 0;
    ok = ok && rm.TextureUVModeU[1].DesiredState == 3;

    // 2. Z-writing group: stipple constants, U changes (bookmarked + queued), V unchanged (skipped), texture attached, stipple shader.
    group.WriteZBuffer = true;
    group.SetupPixelShader(0, stipple, normal);
    ok = ok && sm.ConstantCalls == 2 && sm.Reg == 0 && sm.X == 0.03125f && sm.Y == 0.03125f && sm.Z == 320.0f && sm.W == 240.0f;
    ok = ok && sm.ApplyCalls == 2 && sm.Applied == &stipple;
    ok = ok && rm.TextureUVModeU[1].DesiredState == 1 && rm.TextureUVModeU[1].BookmarkMask == 4 && rm.TextureUVModeU[1].DirtyListFlag;
    ok = ok && rm.StateStackSize == 1 && rm.StateStack[0].State == &rm.TextureUVModeU[1] && rm.StateStack[0].Value == 3;
    ok = ok && rm.StateUpdateListSize == 1 && rm.StateUpdateList[0] == &rm.TextureUVModeU[1];
    ok = ok && rm.TextureUVModeV[1].BookmarkMask == 0 && !rm.TextureUVModeV[1].DirtyListFlag;
    ok = ok && rm.TextureUVModeU[0].DesiredState == 0 && rm.TextureUVModeV[0].DesiredState == 0;
    ok = ok && rm.DeviceSetTextureCalls == 1 && rm.LastStage == 1 && rm.LastTexture == &tag && rm.CurrentlySelectedTextures[1] == &tag;
    ok = ok && rm.TextureStagesInUse == 2;

    // 3. Again with V dirtied by someone else and a larger stage count: U skipped, V bookmarked once (already-set mask bit is not
    //    pushed again), texture cache hit, stage count kept.
    rm.TextureUVModeV[1].DesiredState = 2;
    rm.TextureUVModeV[1].BookmarkMask = 4;
    rm.TextureStagesInUse = 5;
    group.SetupPixelShader(3, stipple, normal);
    ok = ok && rm.StateStackSize == 1 && rm.StateUpdateListSize == 2 && rm.StateUpdateList[1] == &rm.TextureUVModeV[1];
    ok = ok && rm.TextureUVModeV[1].DesiredState == 1 && rm.TextureUVModeV[1].DirtyListFlag;
    ok = ok && rm.DeviceSetTextureCalls == 1 && rm.TextureStagesInUse == 5;
    ok = ok && sm.ConstantCalls == 3 && sm.Reg == 3 && sm.ApplyCalls == 3 && sm.Applied == &stipple;

    if (!ok) { std::printf("00bbcc70_TEST FAIL\n"); return 1; }
    std::printf("00bbcc70_TEST PASS\n");
    return 0;
}