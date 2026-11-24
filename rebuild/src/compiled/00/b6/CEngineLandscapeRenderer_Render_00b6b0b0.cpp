// CEngineLandscapeRenderer::Render(const CEngineRenderParams& params) -- render the landscape for one pass: force vertex and
// NF151 (2026-11-20, q4669): the local CShaderRenderManager model, its `g_ShaderRenderManager` global and the CShaderStatesScope helper are
//   replaced by engine/CShaderRenderManager.h + engine/CScopedShadersEnable.h (the landed 0x00B5E700 / 0x00B5FAB0 form): the singleton is the
//   private static ?SingletonInstance@CShaderRenderManager@@0V1@A (0x013BC470) through the inline Get(), the layout through PeekVertexShaderConstantLayout.
//   pixel shaders on for the scope (restored on exit), open a render-state capture block, then for the foreground pass (0x40)
//   render the sorted patches and (if enabled) every visible map's foreground under its own capture block with vertex-shader
//   constant layout 2; for the background pass (4) set the common states and (if enabled) update the lighting and render every
//   visible map's background under a capture block; each handled pass ends with PostRenderClearRenderStates; finally restore.
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6B0B0, 489 bytes (+int3), `ret 4`.
// Callees: CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110
//   / DisablePixelShaders 0x00988190 / SetVertexShaderConstantLayout 0x0098B5E0 (global object 0x013BC470), CEngineLandscapeRenderer
//   RenderSortedPatches 0x00B68DA0 / SetupCommonStates 0x00B67480 / SetupForegroundStates 0x00B677D0 / SetupBackgroundStates 0x00B671A0
//   (manifest has it as a `_global` unknown) / PostRenderClearRenderStates 0x00B67510, CEngineLandscapeMap::RenderForeground
//   0x00BDC2D0 / RenderBackground 0x00BDC060, CRenderStateManager::RestoreCaptureBlock 0x00A05840; world map 0x01436E8C.
// Layout: local model (see 0x00B68610): RenderForegroundEnabled +0x610, RenderBackgroundEnabled +0x611 (names descriptive);
//   CShaderRenderManager as the landed scope constructor 0x009D8AD0 (shader enables +0x390/+0x391, layout +0x08).
// Shape: the shader switch is the landed CShaderStatesScope with an inline constructor (true, true) and an inline restoring
//   destructor; the manager model needs bytes past +0x391 (a struct that ENDS at +0x392 makes VC read the two bools with one
//   `mov eax,dword ptr` and use al/ah); PostRenderClearRenderStates sits at the end of each case (the default skips it); the map
//   list reference is taken first (ebp); capture block = the 0x00B68610 inline.
struct CStateInfo { unsigned char _body[0x14]; };
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
class CRenderStateManager { public:
    void*              __vftable;            // +0x00
    void*              PD3DDevice;           // +0x04
    CStateStackElement StateStack[1024];     // +0x08
    CStateInfo*        StateUpdateList[256]; // +0x2008
    CStateInfo*        AllStatesList[256];   // +0x2408
    long               StateStackSize;       // +0x2808
    long               StateUpdateListSize;  // +0x280c
    long               AllStatesListSize;    // +0x2810
    unsigned long      CurrentBookmarkMask;  // +0x2814
    unsigned char      DeviceState[0x3a24 - 0x2818];   // +0x2818 CDeviceState
    CStateInfo         BookmarkDummyState;   // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = StateStackSize;
        StateStack[size].State = &BookmarkDummyState;
        StateStackSize++;
        CurrentBookmarkMask <<= 1;
    }
    void RestoreCaptureBlock();              // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"

class CEngineLandscapeMap { public:
    void RenderForeground();                     // 0x00BDC2D0
    void RenderBackground();                     // 0x00BDC060
};
class CEngineMap { public: CEngineLandscapeMap* LandscapeMap; };
struct _List_node { _List_node* _M_next; _List_node* _M_prev; CEngineMap* _M_data; };
struct _List_iterator {
    _List_node* _M_node;
    _List_iterator(_List_node* n) : _M_node(n) {}
    CEngineMap*& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
struct CEngineMapList {                            // STLport list<CEngineMap*>
    _List_node* _M_node;
    _List_iterator begin() { return _List_iterator(_M_node->_M_next); }
    _List_iterator end() { return _List_iterator(_M_node); }
};
class CEngineWorldMap { public:
    unsigned char  _pad[0x2c];
    CEngineMapList VisibleMapList;                 // +0x2c
};
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }         // 0x01436E8C

struct CEngineRenderParams {
    void* _pad00;
    long  Pass;                                    // +0x04
};

class CEngineLandscapeRenderer { public:
    unsigned char _pad000[0x610];
    bool          RenderForegroundEnabled;         // +0x610
    bool          RenderBackgroundEnabled;         // +0x611
    private: void RenderSortedPatches(); public:                    // 0x00B68DA0
    private: void SetupCommonStates(); public:                      // 0x00B67480
    private: void SetupForegroundStates(); public:                  // 0x00B677D0
private:
    void SetupBackgroundStates();                  // 0x00B671A0 ?SetupBackgroundStates@CEngineLandscapeRenderer@@AAEXXZ (Ego_r; respelled from the stand-in UpdateLighting at the chunk 711 landing)
public:
    private: void PostRenderClearRenderStates(); public:            // 0x00B67510
    void Render(const CEngineRenderParams& params);
};

void CEngineLandscapeRenderer::Render(const CEngineRenderParams& params)
{
    CEngineMapList& maps = NEngineInternal::GPEngineWorldMap->VisibleMapList;
    CScopedShadersEnable shaderStates(true, true);
    g_pRenderStateManager->CreateCaptureBlock();
    switch (params.Pass) {
    case 0x40:
        RenderSortedPatches();
        if (RenderForegroundEnabled) {
            g_pRenderStateManager->CreateCaptureBlock();
            SetupCommonStates();
            SetupForegroundStates();
            long layout = CShaderRenderManager::Get().PeekVertexShaderConstantLayout();
            CShaderRenderManager::Get().SetVertexShaderConstantLayout(2);
            for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
                (*it)->LandscapeMap->RenderForeground();
            CShaderRenderManager::Get().SetVertexShaderConstantLayout(layout);
            g_pRenderStateManager->RestoreCaptureBlock();
        }
        PostRenderClearRenderStates();
        break;
    case 4:
        SetupCommonStates();
        if (RenderBackgroundEnabled) {
            g_pRenderStateManager->CreateCaptureBlock();
            SetupBackgroundStates();
            for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
                (*it)->LandscapeMap->RenderBackground();
            g_pRenderStateManager->RestoreCaptureBlock();
        }
        PostRenderClearRenderStates();
        break;
    }
    g_pRenderStateManager->RestoreCaptureBlock();
}