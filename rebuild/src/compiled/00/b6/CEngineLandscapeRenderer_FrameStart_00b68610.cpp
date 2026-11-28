// Flags: 
// CEngineLandscapeRenderer::FrameStart(const CEngineComponentParams& params) -- per-frame landscape update: pick the current
//   vertex-buffer / shader set by the render settings' mode (2 = alternate set), then (if the params ask for a landscape update)
//   run the memory manager's FrameStart, open two render-state capture blocks, take the sub-frame time plus the renderer's
//   offset, walk the world map's visible maps to update background LODs / tesselation (each optional) and foregrounds, load the
//   foreground textures and restore both capture blocks.
// Leaf: ?FrameStart@CEngineLandscapeRenderer@@UAEXABVCEngineComponentParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B68610, 440 bytes (+int3), `ret 4`, 8-byte aligned frame for the double time.
// Callees: CEngineLandscapeMemoryManager::FrameStart 0x00BD9AC0, GFGetFastSubFrameTimer 0x009E1C70 (__fastcall double),
//   CEngineLandscapeMap::UpdateBackgroundLOD 0x00BDC180 / UpdateBackgroundTesselation 0x00BDC020 / UpdateForeground 0x00BDC510,
//   CEngineLandscapeRenderer::LoadForegroundTextures 0x00B68070, CRenderStateManager::RestoreCaptureBlock 0x00A05840 (x2).
//   Globals: render settings 0x01436E60 (+0x334 mode, as the landed 0x00B99B50), CEngineWorldMap 0x01436E8C (VisibleMapList
//   list<CEngineMap*> at retail +0x2c, PDB +0x30), CRenderStateManager 0x013CB508 (as the landed 0x00B25C00).
// Layout: local model; engine/CEngineLandscapeRenderer.h matches only up to GPUUpdateQueue +0x0c -- the TLC object differs from
//   Ego_r.pdb past +0x1c (the buffer/shader pointer sets +0x4f4..+0x5a4, flags +0x63c/+0x650, offset +0x640, memory manager +0x6b0;
//   names descriptive).
// Shape: the capture-block push must index through a `long&` to the stack size but increment the MEMBER (`StateStackSize++`)
//   before shifting the mask: the first inlined copy then reloads/increments the size in ebx (`++size` through the reference
//   gives `inc dword ptr [ecx]`, the member increment after the shift picks edi). The same inline matches 0x00B6B0B0.
// NF45 (round 7): 0x01436E60 is NEngineInternal::GPShadowRenderer (Ego_r.pdb / FableWin.pdb
//   ?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A) and +0x334 its RenderingTechnique, read through the FableWin.pdb inline
//   PeekRenderingTechnique() (engine/CEngineShadowRenderer.h _MEMBERS, as the landed 0x00B99B50); the two capture-block pushes and
//   restores on 0x013CB508 are two CRenderStateScopedCaptureBlock objects (engine/CRenderStateCaptureBlock.h, the header's inline
//   StartCaptureBlock; RestoreCaptureBlock 0x00A05840 in their destructors). Was SRenderSettings_01436e60 / g_pRenderSettings_01436e60 and a
//   local CRenderStateManager / CStateInfo / CStateStackElement model with g_pRenderStateManager.
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CEngineShadowRenderer.h"
#include "engine/CRenderStateCaptureBlock.h"
enum LS_LOADING_STATUS {};   // PDB enum type (enumerators not modelled)
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }   // 0x01436E60

double __fastcall GFGetFastSubFrameTimer();        // 0x009E1C70

class CLandscapeGPUUpdateQueue { public: unsigned char _body[0x10]; };
class CEngineLandscapeMap { public:
    // Naming (NF439 gen, 2026-11-25): callee 0x00BDC180 binds its PDB spelling ?UpdateBackgroundLOD@CEngineLandscapeMap@@QAE?AW4LS_LOADING_STATUS@@AAVCLandscapeGPUUpdateQueue@@N_N@Z (T1 target; was ?UpdateBackgroundLOD@CEngineLandscapeMap@@QAEHPAVCLandscapeGPUUpdateQueue@@N_N@Z).
    LS_LOADING_STATUS UpdateBackgroundLOD(CLandscapeGPUUpdateQueue& queue, double time, bool force);
    void UpdateBackgroundTesselation();                                                   // 0x00BDC020
    int  UpdateForeground();                                                              // 0x00BDC510
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

class CEngineLandscapeMemoryManager { public: void FrameStart(); };   // 0x00BD9AC0

class CEngineComponentParams { public: unsigned char _pad[0x7c]; bool UpdateLandscape; };

class CEngineLandscapeRenderer { public:
    virtual void v00();
    unsigned char             _pad04[4];
    bool                      Enabled;                     // +0x08
    unsigned char             _pad09[3];
    CLandscapeGPUUpdateQueue  GPUUpdateQueue;              // +0x0c
    unsigned char             _pad1c[0x4f4 - 0x1c];
    unsigned char             A4f4[0xc], A500[0xc], A50c[0xc]; // +0x4f4
    unsigned char             _pad518[0x520 - 0x518];
    unsigned char             B520[0xc], B52c[0xc], B538[0xc]; // +0x520
    void*                     Current544[3];               // +0x544
    unsigned char             _pad550[0x578 - 0x550];
    unsigned char             C578[8], C580[8], C588[8], C590[8];   // +0x578
    unsigned char             _pad598[0x5a0 - 0x598];
    void*                     Current5a0[2];               // +0x5a0
    unsigned char             _pad5a8[0x63c - 0x5a8];
    bool                      UpdateBackgroundLODs;        // +0x63c
    unsigned char             _pad63d[3];
    float                     TimeOffset;                  // +0x640
    unsigned char             _pad644[0x650 - 0x644];
    bool                      UpdateBackgroundTesselations;   // +0x650
    unsigned char             _pad651[0x6b0 - 0x651];
    CEngineLandscapeMemoryManager* MemoryManager;          // +0x6b0
private:
    bool LoadForegroundTextures(bool loading_new_scene);   // FableWin 02C887C0; retail 00B68070.
public:
    virtual void FrameStart(const CEngineComponentParams& params);
};

void CEngineLandscapeRenderer::FrameStart(const CEngineComponentParams& params)
{
    if (!Enabled)
        return;
    if (NEngineInternal::GPShadowRenderer->PeekRenderingTechnique() == CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER) {
        Current5a0[0] = C588;
        Current5a0[1] = C590;
        Current544[0] = B520;
        Current544[1] = B52c;
        Current544[2] = B538;
    } else {
        Current5a0[0] = C578;
        Current5a0[1] = C580;
        Current544[0] = A4f4;
        Current544[1] = A500;
        Current544[2] = A50c;
    }
    if (!params.UpdateLandscape)
        return;
    MemoryManager->FrameStart();
    CRenderStateScopedCaptureBlock capture_block_1;
    CRenderStateScopedCaptureBlock capture_block_2;
    double time = GFGetFastSubFrameTimer() + TimeOffset;
    CEngineMapList& maps = NEngineInternal::GPEngineWorldMap->VisibleMapList;
    if (UpdateBackgroundLODs)
        for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
            ((int)((*it)->LandscapeMap->UpdateBackgroundLOD(*(&GPUUpdateQueue), time, false)));
    if (UpdateBackgroundTesselations)
        for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
            (*it)->LandscapeMap->UpdateBackgroundTesselation();
    for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
        (*it)->LandscapeMap->UpdateForeground();
    LoadForegroundTextures(false);
}