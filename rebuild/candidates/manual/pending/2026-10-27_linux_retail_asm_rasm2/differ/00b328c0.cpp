// CEnginePrimitiveRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& params) -- per-scene setup: lerp the draw distance by the load progress, round the cell count up, update every scene grid (two passes OR-ing "still loading"), then the lighting manager, then build each grid's render data from the shadow renderer's scene data
// Leaf: LoadNewScene
// Retail-asm: 0x00B32924..0x00B32947 `fld [ebp - 0xc] ; fsub [ebp - 0x10] ; fistp [ebp + 8] ; fild [ebp + 8] ; fadd [ebp - 4] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0xb32941 ; inc [ebp + 8] ; fld [esi - 0x1bc] ; mov ecx, [ebp + 8]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B328C0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The cell count is the floor block (hence the ebp frame).
#pragma optimize("y", off)
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
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
    float check = count + 1.0f;
    if (*(long*)&cells == *(long*)&check)
        ++count;
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
