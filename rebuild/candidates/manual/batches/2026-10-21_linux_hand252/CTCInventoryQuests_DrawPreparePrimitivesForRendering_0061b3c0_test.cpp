#include <cstdio>
#include <vector>
// Behaviour test for CTCInventoryQuests::DrawPreparePrimitivesForRendering (0x0061B3C0): same body as src/, with the
// clock, components, front-end manager, primitive handles and the inventory draw helpers stubbed and recorded.
static double g_time = 0.0;
static double GFGetTime() { return g_time; }
struct CViewport { int id; };
struct CEnginePrimitivePrepareInfo { CViewport* PViewport; };
static int g_updates = 0, g_renders = 0; static float g_lastDelta = -1.0f; static CViewport* g_lastViewport = 0;
namespace NUISystem {
struct CComponent {
    void Update(float d) { ++g_updates; g_lastDelta = d; }
    void Render(CViewport* v, long, long, long, long) { ++g_renders; g_lastViewport = v; }
};
}
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return *(begin() + n); }
};
static int g_destroyed = 0;
struct CRefCount {
    long RefCount;
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
};
struct CFadeSettingsObject : CRefCount {
    float FadeTime;
    void Destroy() { ++g_destroyed; }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
static CFadeSettingsObject g_fade;
struct CFadeSettingsPtr {
    CFadeSettingsObject* Object;
    CFadeSettingsPtr() : Object(0) { }
    CFadeSettingsPtr(const CFadeSettingsPtr& o) : Object(o.Object) { if (Object) ++Object->RefCount; }   // no NRVO under VC7.1
    ~CFadeSettingsPtr() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    CFadeSettingsObject* operator->() const { return Object; }
};
namespace NUISystem {
struct CFrontEndManager {
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
    CFadeSettingsPtr GetFadeSettings() { ++g_fade.RefCount; CFadeSettingsPtr p; p.Object = &g_fade; return p; }
};
}
static int g_cleared = 0;
struct CEnginePrimitiveHandle { long Handle; void Clear() { ++g_cleared; Handle = 0; } };
template <class R, class T> struct mem_fun_ref_t {
    R (T::*_M_f)();
    explicit mem_fun_ref_t(R (T::*f)()) : _M_f(f) { }
    R operator()(T& r) const { return (r.*_M_f)(); }
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*f)()) { return mem_fun_ref_t<R, T>(f); }
template <class I, class F> F _For_each(I first, I last, F f) { for (; first != last; ++first) f(*first); return f; }
template <class I, class F> inline F for_each(I first, I last, F f) { return _For_each(first, last, f); }
struct CPrimitiveHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last) { _M_finish = first + (_M_finish - last); return first; }
    void clear() { erase(begin(), end()); }
};
static float g_backdrop = -1.0f; static int g_frames = 0; static bool g_frameArg = false; static int g_testQuests = 0;
struct CTCInventoryBase {
    CComponentVector Components;
    bool             FirstRenderFrame;
    double           LastRenderTime;
    bool             InventoryOpen;
    void RenderUIBackdropQuad(float alpha) { g_backdrop = alpha; }
    void DrawInventoryFrame(const CEnginePrimitivePrepareInfo&, bool b) { ++g_frames; g_frameArg = b; }
};
static bool g_active = false;
namespace NHeroInformationScreens { struct CBase { bool IsActive() { return g_active; } }; }
struct CTCInventoryQuests : CTCInventoryBase, NHeroInformationScreens::CBase {
    CPrimitiveHandleVector InventoryQuestCardSelectPrimitiveHandles;
    float            AccumFadeTime;
    void DrawActiveTestQuests(const CEnginePrimitivePrepareInfo&) { ++g_testQuests; }
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};
void CTCInventoryQuests::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (InventoryOpen) {
        if (!IsActive()) {
            float deltaTime = (float)(GFGetTime() - LastRenderTime);
            LastRenderTime = GFGetTime();
            for (unsigned long i = 0; i < Components.size(); ++i) {
                Components[i]->Update(deltaTime);
                if (FirstRenderFrame)
                    FirstRenderFrame = false;
                else
                    Components[i]->Render(info.PViewport, 0, 0, 0, 0);
            }
            AccumFadeTime += deltaTime;
            float alpha = AccumFadeTime / NUISystem::CFrontEndManager::GetInstance()->GetFadeSettings()->FadeTime;
            if (alpha > 1.0f)
                alpha = 1.0f;
            RenderUIBackdropQuad(alpha);
        } else {
            for_each(InventoryQuestCardSelectPrimitiveHandles.begin(), InventoryQuestCardSelectPrimitiveHandles.end(),
                     mem_fun_ref(&CEnginePrimitiveHandle::Clear));
            InventoryQuestCardSelectPrimitiveHandles.clear();
            DrawInventoryFrame(info, IsActive());
            DrawActiveTestQuests(info);
        }
    }
}

int main()
{
    int fails = 0;
    NUISystem::CComponent comps[3];
    NUISystem::CComponent* list[3] = { &comps[0], &comps[1], &comps[2] };
    CEnginePrimitiveHandle handles[2] = { { 5 }, { 6 } };
    CViewport vp; CEnginePrimitivePrepareInfo info; info.PViewport = &vp;
    CTCInventoryQuests q;
    q.Components._M_start = list; q.Components._M_finish = list + 3;
    q.FirstRenderFrame = true; q.LastRenderTime = 1.0; q.InventoryOpen = false; q.AccumFadeTime = 0.0f;
    q.InventoryQuestCardSelectPrimitiveHandles._M_start = handles; q.InventoryQuestCardSelectPrimitiveHandles._M_finish = handles + 2;
    g_fade.RefCount = 1; g_fade.FadeTime = 2.0f;
    g_time = 1.5;
    q.DrawPreparePrimitivesForRendering(info);                  // closed: nothing
    if (g_updates || g_frames || g_backdrop != -1.0f) ++fails;
    q.InventoryOpen = true;
    q.DrawPreparePrimitivesForRendering(info);                  // delta 0.5, first frame skips one render
    if (g_updates != 3 || g_renders != 2 || g_lastDelta != 0.5f || g_lastViewport != &vp || q.FirstRenderFrame) ++fails;
    if (q.LastRenderTime != 1.5 || q.AccumFadeTime != 0.5f || g_backdrop != 0.25f) ++fails;
    if (g_fade.RefCount != 1 || g_destroyed) ++fails;
    g_time = 4.0;
    q.DrawPreparePrimitivesForRendering(info);                  // delta 2.5, accum 3.0 -> clamped to 1
    if (g_updates != 6 || g_renders != 5 || g_backdrop != 1.0f || q.AccumFadeTime != 3.0f) ++fails;
    g_fade.RefCount = 0;                                          // the getter's reference is the last one
    q.DrawPreparePrimitivesForRendering(info);
    if (g_destroyed != 1) ++fails;
    g_active = true;
    q.DrawPreparePrimitivesForRendering(info);                  // information screen
    if (g_cleared != 2 || q.InventoryQuestCardSelectPrimitiveHandles._M_finish != handles || g_frames != 1 || !g_frameArg
        || g_testQuests != 1 || g_updates != 9) ++fails;
    if (fails) { std::printf("0061b3c0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0061b3c0_TEST PASS\n");
    return 0;
}
