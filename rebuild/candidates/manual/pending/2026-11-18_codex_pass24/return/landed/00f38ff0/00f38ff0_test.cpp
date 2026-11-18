#include <stdio.h>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
enum _D3DRENDERSTATETYPE { DummyRender };
enum _D3DTEXTURESTAGESTATETYPE { DummyStage };
enum _D3DSAMPLERSTATETYPE { DummySampler };
// Expose the current, saved and queued states to the fixture.
#define private public
#include "engine/CRenderStateManager.h"
#undef private

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CRenderStateManager::CStateInfo::CStateInfo()
    : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false) {}
CRenderStateManager::CDeviceState::CDeviceState() {}
CRenderStateManager::CRenderStateManager(unsigned long)
    : StateStackSize(0), StateUpdateListSize(0), CurrentBookmarkMask(1) {}
CRenderStateManager::~CRenderStateManager() {}
unsigned long CRenderStateManager::D3DTextureUVModeTable[4] = {1,2,3,4};
unsigned long CRenderStateManager::D3DPolyCullModeTable[3] = {3,2,1};
unsigned long CRenderStateManager::D3DPolyFillModeTable[3] = {3,2,1};
unsigned long CRenderStateManager::D3DPolyShadeModeTable[2] = {1,2};
unsigned long CRenderStateManager::D3DTextureArgumentModeTable[20] = {1,0,2,3,4};
unsigned long CRenderStateManager::D3DTextureOperationModeTable[24] = {1,2,3,4};
class CRenderManager : public CRenderStateManager {
public:
    CRenderManager() : CRenderStateManager(0) {}
};
class CDisplayManager {
public:
    CRenderManager renderer;
    CRenderManager& GetRenderManager() { return renderer; }
};
class CSystemManager {
public:
    CDisplayManager display;
    CDisplayManager* GetDisplayManager() { return &display; }
};
static CSystemManager systems;
static CSystemManager* GFGetSystemManager() { return &systems; }
class CSurfaceCompositor { public: void SetBaseBlendModes(); };

void CSurfaceCompositor::SetBaseBlendModes()
{
    CRenderManager& renderer = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    renderer.SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    renderer.SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    renderer.SetZBufferEnable(false);
    renderer.SetPolyCullMode(POLY_CULL_NONE);
    renderer.SetSpecularEnable(true);
    renderer.SetPolyFillMode(POLY_FILL_SOLID);
    renderer.SetPolyShadeMode(POLY_SHADE_GOURAUD);
    renderer.SetAlphaBlendEnable(false);
    renderer.SetAlphaTestEnable(false);
    renderer.SetColourWriteEnable(true, true, true, true);
    renderer.SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    renderer.SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);
    renderer.SetTextureOperation(0, TEXTURE_OP_MODULATE);
    renderer.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    renderer.SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
    renderer.SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    renderer.SetTextureOperation(1, TEXTURE_OP_DISABLE);
    renderer.SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
}
int main()
{
    CRenderManager& renderer = systems.display.renderer;
    CRenderStateManager::CDeviceState& device = renderer.DeviceState;
    CRenderStateManager::CStateInfo* states[] = {
        &device.TextureUVModeU[0], &device.TextureUVModeV[0], &device.ZBufferEnable,
        &device.PolyCullMode, &device.SpecularEnable, &device.PolyFillMode,
        &device.PolyShadeMode, &device.AlphaBlendEnable, &device.AlphaTestEnable,
        &device.ColourWriteEnable, &device.TextureArgument1[0],
        &device.TextureArgument2[0], &device.TextureOperation[0],
        &device.TextureAlphaArgument1[0], &device.TextureAlphaArgument2[0],
        &device.TextureAlphaOperation[0], &device.TextureOperation[1], &device.TextureAlphaOperation[1]
    };
    const unsigned long expected[] = {1,1,0,1,1,3,2,0,0,15,2,0,4,2,0,4,1,1};
    CSurfaceCompositor compositor;
    for (unsigned int mode = 0; mode < 8; ++mode) {
        const bool unchanged = (mode & 1) != 0;
        const bool saved = (mode & 2) != 0;
        const bool queued = (mode & 4) != 0;
        renderer.CurrentBookmarkMask = 4;
        renderer.StateStackSize = renderer.StateUpdateListSize = 0;
        for (unsigned int k = 0; k < 18; ++k) {
            states[k]->CurrentState = 45;
            states[k]->DesiredState = unchanged ? expected[k] : 100 + k;
            states[k]->BookmarkMask = saved ? 5 : 1;
            states[k]->DirtyListFlag = queued;
        }
        compositor.SetBaseBlendModes();
        if (renderer.StateStackSize != (unchanged || saved ? 0 : 18) ||
            renderer.StateUpdateListSize != (unchanged || queued ? 0 : 18)) return 1;
        for (unsigned int k = 0; k < 18; ++k) {
            if (states[k]->CurrentState != 45 || states[k]->DesiredState != expected[k] ||
                states[k]->DirtyListFlag != (queued || !unchanged) ||
                states[k]->BookmarkMask != (saved || !unchanged ? 5UL : 1UL)) return 2;
            if (!unchanged && !saved &&
                (renderer.StateStack[k].Value != 100 + k || renderer.StateStack[k].State != states[k])) return 3;
            if (!unchanged && !queued && renderer.StateUpdateList[k] != states[k]) return 4;
        }
    }
    puts("00f38ff0_TEST PASS");
    return 0;
}
