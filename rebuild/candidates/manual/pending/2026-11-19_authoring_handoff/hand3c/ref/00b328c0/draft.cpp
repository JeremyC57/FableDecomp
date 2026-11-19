// Grade: DIFFER(501v517) retry1 -- retail_asm FloorToLong in place; residue: rough model around it (frame, lerp chain)
// CEnginePrimitiveRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& params) -- per-scene setup: lerp the draw distance by the load progress, round the cell count up, update every scene grid (two passes OR-ing "still loading"), then the lighting manager, then build each grid's render data from the shadow renderer's scene data
// Retail-asm: 0x00B32927 `fsub [ebp-10h]` + 0x00B3292A `fistp dword ptr [ebp+8]` -- the engine inline-assembly FloorToLong (landed 0x006A68F0); VC7.1 C++ (long)x calls _ftol.
// Leaf: LoadNewScene
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B328C0, length 528 bytes, `ret 4`, ebp frame.
// Callees: 0x00B244F0 (on 0x01436EA4), CEngineSceneGrid 0x00B92F80 / 0x00B92E50 / 0x00B930A0, CEngineLightingManager 0x00B444F0
//   (0x01436E9C), scene-data copy ctor 0x00B38600 (source g_pShadowRenderer 0x01436E60 + 0xc).
// Layout: local model; `this` is the secondary base (+0x1cc) of CEnginePrimitiveRenderer, so the primary part's fields read at
//   negative offsets (this-0x1cc .. this-0x1b4).
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
struct C3DVector { float X, Y, Z; };
struct CEngineCamera { unsigned char _pad[0x54]; C3DVector WorldPosition; };
struct CEngineComponentLoadNewSceneParams {
    unsigned char  Params[0x80];
    CEngineCamera* Camera;          // +0x80
    float          Progress;        // +0x84
    unsigned long  Flags;           // +0x88
};
struct SSceneLoadView { C3DVector Position; float DrawDistance; };
struct CSceneRenderData { unsigned char _pad[0x110]; unsigned char Data[0x110]; CSceneRenderData(const void* src); };   // 0x00B38600
struct CEngineSceneGrid {
    bool Update(CEngineCamera* camera, unsigned long flags);                      // 0x00B92F80
    bool Load(const SSceneLoadView* view, unsigned long flags);                   // 0x00B92E50
    bool Build(void* data, const C3DVector* pos, unsigned long flags);            // 0x00B930A0
};
struct SDetailLevel { unsigned char _pad[0x40]; long A; long B; };
struct CDetailManager { SDetailLevel* Get(long id); };    // 0x00B244F0
struct SDetailOwner { unsigned char _pad[0x20]; long Id; };
extern CDetailManager* g_pDetailManager;    // 0x01436EA4
extern SDetailOwner* g_pDetailOwner;        // 0x01436EB0
struct CEngineLightingManager { void LoadNewScene(CEngineCamera* camera); };   // 0x00B444F0
extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C
extern unsigned char* g_pShadowRenderer;    // 0x01436E60

struct SPrimaryFields {             // the fields this-0x1cc .. this-0x1b4
    long  DetailA;                  // -0x1cc
    long  DetailB;                  // -0x1c8
    float BaseScale;                // -0x1c4
    float CopiedScale;              // -0x1c0
    float Factor;                   // -0x1bc
    float Scaled;                   // -0x1b8
    long  CellCount;                // -0x1b4
};
struct CEnginePrimitiveRendererView {
    void* _vptr;                    // +0x00
    unsigned char _pad[0xd0 - 4];
    CEngineSceneGrid** GridsBegin;  // +0xd0
    CEngineSceneGrid** GridsEnd;    // +0xd4
    long  _d8;
    float NearDistance;             // +0xdc
    float FarDistance;              // +0xe0
    float CellScale;                // +0xe4
    bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
};

bool CEnginePrimitiveRendererView::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    SSceneLoadView view;
    view.Position = params.Camera->WorldPosition;
    view.DrawDistance = (FarDistance - NearDistance) * params.Progress + NearDistance;
    float cells = CellScale * params.Progress;
    long count = FloorToLong(cells);
    SPrimaryFields* primary = (SPrimaryFields*)((unsigned char*)this - 0x1cc);
    primary->CellCount = count;
    primary->CopiedScale = primary->BaseScale;
    primary->Scaled = primary->Factor * primary->BaseScale;
    if (g_pDetailOwner) {
        primary->DetailA = g_pDetailManager->Get(g_pDetailOwner->Id)->A;
        primary->DetailB = g_pDetailManager->Get(g_pDetailOwner->Id)->B;
    }
    unsigned long flags = (params.Flags & 2) ? 0 : 1;
    bool loading = false;
    for (unsigned long i = 0; i < (unsigned long)(GridsEnd - GridsBegin); ++i) {
        loading |= GridsBegin[i]->Update(params.Camera, flags | 2);
        loading |= GridsBegin[i]->Load(&view, flags);
    }
    flags |= 4;
    g_pEngineLightingManager->LoadNewScene(params.Camera);
    for (unsigned long j = 0; j < (unsigned long)(GridsEnd - GridsBegin); ++j) {
        CSceneRenderData data(g_pShadowRenderer + 0xc);
        loading |= GridsBegin[j]->Build(data.Data, &params.Camera->WorldPosition, flags);
    }
    return loading;
}
