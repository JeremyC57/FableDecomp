// CEngine::RenderFrame(const CEngineInterpolationInfo& info) -- renders one frame: opens a render-state capture block, sets the interpolated world time, builds the component parameters, runs FrameStart on every component whose scene filter matches, renders every layer, runs FrameEnd, frees the frame memory pool, counts the frame and restores the captured render states
// Leaf: ?RenderFrame@CEngine@@AAEXABVCEngineInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B25950, 365 bytes, thiscall `ret 4` (ebp frame aligned to 8).
// Identity (proven): Ego_r.pdb ?RenderFrame@CEngine@@AAEXABVCEngineInterpolationInfo@@@Z (private; donor 0x009AFFD0); its
//   caller is CEngine::Render 0x00B27D90 (Ego_r ?Render@CEngine@@UAEXABVCEngineInterpolationInfo@@K@Z, vtable slot 8).
//   Members used (retail offsets, Ego_r names): CurrentRenderFrame +0x88, NonInterpolatedWorldTime +0x98,
//   InterpolatedWorldTime +0xA8, WorldFramesPerSecond +0xB0, SceneFilterFlags +0xB8, OffWorldTickProcessInfo.
//   ProcessThisFrame +0xF8, FrameMemoryPool +0x130, RenderLayers +0x15C, EngineComponents +0x168;
//   CEngineComponentParams {FrameInterpolation, InterpolationInfo (0x70), FrameID, SceneFilterFlags,
//   OffWorldTickProcessFlag} (Ego_r, 0x80 bytes); the render-state capture uses CRenderStateManager's StateStack +0x8,
//   StateStackSize +0x2808, CurrentBookmarkMask +0x2814 and BookmarkDummyState +0x3A24 (Ego_r layout, matches retail).
// Callees: CEngine::SetDefaultRenderStates 0x00B24850 (Ego_r ?SetDefaultRenderStates@CEngine@@AAEXXZ, private -- the
//   landed QAE spelling is wrong, NAME_FIXES), CEngineInterpolationInfo::CEngineInterpolationInfo() 0x0042FA30,
//   CSimpleMemoryPool::FreeAll 0x00A5C720, CRenderStateManager::RestoreCaptureBlock() 0x00A05840
//   (Ego_r ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ; also bound under two wrong spellings -- NAME_FIXES); virtual CEngineComponent::PeekSceneFilterFlags (slot 10),
//   FrameStart (1), FrameEnd (2), CEngineLayerRenderer::Render (slot 1).
// Shape: the capture-block push is an inline CRenderStateManager member (its name is descriptive -- inlined, so absent
//   from the PDB; written in the body, VC7.1 addresses StateStackSize directly instead of through ecx, grade 2); the
//   CEngine vptr slot is padded to 8 because the class holds doubles (grade 1 had every member 4 bytes low).
// Globals: 0x013CB508 the CRenderStateManager* (no PDB data name; bound ?g_pRenderStateManager_013cb508@@3PAVCRenderStateManager@@A).
class CCharString;
class CEngineComponentLoadNewSceneParams;
class CEngineComponentRenderParams;

class CEngineInterpolationInfoSet {
public:
    float GTPredictedRenderAt;
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};
class CEngineInterpolationInfo {
public:
    CEngineInterpolationInfo();                            // 0x0042FA30
    CEngineInterpolationInfoSet Current;                   // +0x00
    CEngineInterpolationInfoSet Last;                      // +0x0C
    CEngineInterpolationInfoSet PausedCurrent;             // +0x18
    CEngineInterpolationInfoSet PausedLast;                // +0x24
    CEngineInterpolationInfoSet BulletTimeCurrent;         // +0x30
    CEngineInterpolationInfoSet BulletTimeLast;            // +0x3C
    long WFServerCurrentTime;                              // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4C
};
class CEngineComponentParams {
public:
    float FrameInterpolation;                              // +0x00
    CEngineInterpolationInfo InterpolationInfo;            // +0x04
    unsigned long FrameID;                                 // +0x74
    unsigned long SceneFilterFlags;                        // +0x78
    bool OffWorldTickProcessFlag;                          // +0x7C
};
class CEngineComponent {
public:
    virtual ~CEngineComponent();
    virtual void FrameStart(const CEngineComponentParams& params);
    virtual void FrameEnd(const CEngineComponentParams& params);
    virtual bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
    virtual void Render(const CEngineComponentRenderParams& params);
    virtual void PreLayerRender(const CEngineComponentRenderParams& params);
    virtual void PostLayerRender(const CEngineComponentRenderParams& params);
    virtual void WorldFrameUpdate(bool paused);
    virtual void PausedWorldFrameUpdate();
    virtual CCharString GetConsoleEnableFunctionName();
    virtual unsigned long PeekSceneFilterFlags() = 0;      // slot 10 (pure in the base; landed name, not in the PDB)
    virtual void OnReleaseDefaultPoolResources();
    virtual bool OnRestoreDefaultPoolResources();
};
class CEngineLayerRenderer {
public:
    virtual ~CEngineLayerRenderer();
    virtual void Render(const CEngineComponentParams& params);   // slot 1
};
class CSimpleMemoryPool {
public:
    void FreeAll();                                        // 0x00A5C720
    unsigned char _body[0x1C];
};
class CRenderStateManager {
public:
    struct CStateInfo;
    struct CStateStackElement {
        CStateInfo* State;
        unsigned long Value;
    };
    void RestoreCaptureBlock();                            // 0x00A05840
    void CreateCaptureBlock()                              // inline (no PDB entry): push the bookmark marker
    {
        StateStack[StateStackSize].State = (CStateInfo*)BookmarkDummyState;
        StateStackSize++;
        CurrentBookmarkMask <<= 1;
    }
    void* __vftable_or_pad;                                // +0x0000
    void* PD3DDevice;                                      // +0x0004
    CStateStackElement StateStack[1024];                   // +0x0008
    CStateInfo* StateUpdateList[256];                      // +0x2008
    CStateInfo* AllStatesList[256];                        // +0x2408
    long StateStackSize;                                   // +0x2808
    long StateUpdateListSize;                              // +0x280C
    long AllStatesListSize;                                // +0x2810
    unsigned long CurrentBookmarkMask;                     // +0x2814
    unsigned char DeviceState[0x120C];                     // +0x2818
    unsigned char BookmarkDummyState[0x14];                // +0x3A24 CStateInfo
};
extern CRenderStateManager* g_pRenderStateManager_013cb508;   // 0x013CB508

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
template <class T> class CArray : public std::vector<T> { };

class CEngine {
public:
    virtual ~CEngine();
private:
    void RenderFrame(const CEngineInterpolationInfo& info);
    void SetDefaultRenderStates();                         // 0x00B24850
public:
    unsigned char _pad_0x08[0x80];                         // +0x08 (the vptr slot is padded to 8: the class holds doubles)
    long CurrentRenderFrame;                               // +0x88
    long CurrentWorldFrame;                                // +0x8C
    long CurrentPausedWorldFrame;                          // +0x90
    unsigned char _pad_0x94[0x4];                          // +0x94
    double NonInterpolatedWorldTime;                       // +0x98
    double NonInterpolatedPausedWorldTime;                 // +0xA0
    double InterpolatedWorldTime;                          // +0xA8
    unsigned long WorldFramesPerSecond;                    // +0xB0
    bool BlockOnLoad;                                      // +0xB4
    unsigned long SceneFilterFlags;                        // +0xB8
    bool EnableScreenShotMode;                             // +0xBC
    bool EnablePostDeviceReset;                            // +0xBD
    unsigned char VideoOptions[0x38];                      // +0xC0 CEngineVideoOptions
    struct {
        bool ProcessThisFrame;                             // +0xF8
        bool ProcessingEnabled;
        float ProcessTicksBehindAccumilator;
        float MaxProcessTicksBehind;
        float MaxProcessTicksBehindBeforeCatchup;
        float MinProcessFrequency;
        double LastFrameTimeStamp;
    } OffWorldTickProcessInfo;                             // +0xF8
    unsigned char _pad_0x118[0x18];                        // +0x118 PEnvironmentDef .. PEngineVideoOptionsDefs
    CSimpleMemoryPool FrameMemoryPool;                     // +0x130
    unsigned char ScratchZBuffersD24S8[0xC];               // +0x14C
    long AnisotropicFilteringLevel;                        // +0x158
    CArray<CEngineLayerRenderer*> RenderLayers;            // +0x15C
    CArray<CEngineComponent*> EngineComponents;            // +0x168
};

void CEngine::RenderFrame(const CEngineInterpolationInfo& info)
{
    g_pRenderStateManager_013cb508->CreateCaptureBlock();

    InterpolatedWorldTime = NonInterpolatedWorldTime + info.Current.WFInterpolate / WorldFramesPerSecond;
    SetDefaultRenderStates();

    CEngineComponentParams params;
    params.FrameInterpolation = info.Current.WFInterpolate;
    params.InterpolationInfo = info;
    params.FrameID = CurrentRenderFrame;
    params.SceneFilterFlags = SceneFilterFlags;
    params.OffWorldTickProcessFlag = OffWorldTickProcessInfo.ProcessThisFrame;

    CArray<CEngineComponent*>::iterator it;
    for (it = EngineComponents.begin(); it != EngineComponents.end(); ++it)
        if ((*it)->PeekSceneFilterFlags() & SceneFilterFlags)
            (*it)->FrameStart(params);
    for (CArray<CEngineLayerRenderer*>::iterator layer = RenderLayers.begin(); layer != RenderLayers.end(); ++layer)
        (*layer)->Render(params);
    for (it = EngineComponents.begin(); it != EngineComponents.end(); ++it)
        if ((*it)->PeekSceneFilterFlags() & SceneFilterFlags)
            (*it)->FrameEnd(params);

    FrameMemoryPool.FreeAll();
    ++CurrentRenderFrame;
    g_pRenderStateManager_013cb508->RestoreCaptureBlock();
}