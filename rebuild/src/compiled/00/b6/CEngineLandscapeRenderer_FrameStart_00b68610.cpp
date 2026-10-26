// CEngineLandscapeRenderer::FrameStart(const CEngineComponentParams& params) -- per-frame landscape update: pick the current
//   vertex-buffer / shader set by the render settings' mode (2 = alternate set), then (if the params ask for a landscape update)
//   run the memory manager's FrameStart, open two render-state capture blocks, take the sub-frame time plus the renderer's
//   offset, walk the world map's visible maps to update background LODs / tesselation (each optional) and foregrounds, load the
//   foreground textures and restore both capture blocks.
// Leaf: FrameStart
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

struct SRenderSettings_01436e60 {
    unsigned char _pad[0x334];
    long          m_clothMode;                     // +0x334
};
extern SRenderSettings_01436e60* g_pRenderSettings_01436e60;   // 0x01436E60

double __fastcall GFGetFastSubFrameTimer();        // 0x009E1C70

class CLandscapeGPUUpdateQueue { public: unsigned char _body[0x10]; };
class CEngineLandscapeMap { public:
    int  UpdateBackgroundLOD(CLandscapeGPUUpdateQueue* queue, double time, bool force);   // 0x00BDC180
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
extern CEngineWorldMap* g_pEngineWorldMap;         // 0x01436E8C

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
    bool LoadForegroundTextures(bool force);               // 0x00B68070
    virtual void FrameStart(const CEngineComponentParams& params);
};

void CEngineLandscapeRenderer::FrameStart(const CEngineComponentParams& params)
{
    if (!Enabled)
        return;
    if (g_pRenderSettings_01436e60->m_clothMode == 2) {
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
    g_pRenderStateManager->CreateCaptureBlock();
    g_pRenderStateManager->CreateCaptureBlock();
    double time = GFGetFastSubFrameTimer() + TimeOffset;
    CEngineMapList& maps = g_pEngineWorldMap->VisibleMapList;
    if (UpdateBackgroundLODs)
        for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
            (*it)->LandscapeMap->UpdateBackgroundLOD(&GPUUpdateQueue, time, false);
    if (UpdateBackgroundTesselations)
        for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
            (*it)->LandscapeMap->UpdateBackgroundTesselation();
    for (_List_iterator it = maps.begin(); it != maps.end(); ++it)
        (*it)->LandscapeMap->UpdateForeground();
    LoadForegroundTextures(false);
    g_pRenderStateManager->RestoreCaptureBlock();
    g_pRenderStateManager->RestoreCaptureBlock();
}