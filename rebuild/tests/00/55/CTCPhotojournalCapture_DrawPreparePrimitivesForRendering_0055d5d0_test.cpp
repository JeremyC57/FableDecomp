// Behaviour test for CTCPhotojournalCapture::DrawPreparePrimitivesForRendering (0x0055D5D0): nothing while closed; open: every
// component gets Update(elapsed) and Draw(info.Engine, 0, 0, 0, 0), LastRenderTime advances, frame 2 switches the engine flag
// off, frames >= 2 render the inventory backdrop at 1.0.
#include <cstdio>
struct CTCBase;
static double g_time = 0; static int g_log[32]; static int g_n = 0; static float g_elapsed = -1; static int g_flagCalls = 0;
double GFGetTime() { return g_time; }
struct CIEngine { int id; void SetEngineFlag76(bool on) { ++g_flagCalls; g_log[g_n++] = on ? 91 : 90; } };
struct CEnginePrimitivePrepareInfo { CIEngine* Engine; };
static const CIEngine* g_drawEngine = 0;
namespace NUISystem {
    struct CComponent {
        virtual ~CComponent() { }
        virtual void Update(float elapsed) { g_log[g_n++] = 1; g_elapsed = elapsed; }
        virtual void Draw(CIEngine* engine, long a, long b, long c, long d) { g_log[g_n++] = 2 + a + b + c + d; g_drawEngine = engine; }
    };
}
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CDisplayEngine { CIEngine* Engine3D; };
struct CMainGameComponent { CDisplayEngine* PDisplayEngine; };
static CIEngine g_engine3D; static CDisplayEngine g_display = { &g_engine3D }; static CMainGameComponent g_mgc = { &g_display };
CMainGameComponent* g_pMainGameComponent = &g_mgc;
static float g_backdrop = -1;
struct CTCInventory { void RenderUIBackdropQuad(float alpha) { g_log[g_n++] = 7; g_backdrop = alpha; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(T** out, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCPhotojournalCapture {
    CThing*       ParentThing;
    long          FrameCounter;
    bool          ScreenOpen;
    double        LastRenderTime;
    vector<NUISystem::CComponent*> Components;
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCPhotojournalCapture::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (ScreenOpen) {
        {
            float elapsed = (float)(GFGetTime() - LastRenderTime);
            LastRenderTime = GFGetTime();
            for (unsigned long i = 0; i < Components.size(); ++i) {
                Components[i]->Update(elapsed);
                Components[i]->Draw(info.Engine, 0, 0, 0, 0);
            }
        }
        if (++FrameCounter == 2)
            g_pMainGameComponent->PDisplayEngine->Engine3D->SetEngineFlag76(false);
        if (FrameCounter >= 2) {
            CTCInventory* inventory;
            if (ParentThing->GetInterface(&inventory, 0x11))
                inventory->RenderUIBackdropQuad(1.0f);
        }
    }
}

int main()
{
    int bad = 0;
    CTCInventory inventory;
    CInterfaceMap::SEntry entries[1] = { { 0x11, (CTCBase*)&inventory } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_presentBits[0] = 1UL << 17;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    NUISystem::CComponent a, b; NUISystem::CComponent* list[2] = { &a, &b };
    CIEngine uiEngine;
    CEnginePrimitivePrepareInfo info = { &uiEngine };
    CTCPhotojournalCapture c; c.ParentThing = &thing; c.FrameCounter = 0; c.ScreenOpen = false; c.LastRenderTime = 1.0;
    c.Components._M_start = list; c.Components._M_finish = list + 2; c.Components._M_end_of_storage = list + 2;
    g_time = 1.5;
    c.DrawPreparePrimitivesForRendering(info);
    if (g_n != 0 || c.LastRenderTime != 1.0 || c.FrameCounter != 0) bad |= 1;
    c.ScreenOpen = true;
    c.DrawPreparePrimitivesForRendering(info);           // frame 1
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[3] != 2 || g_elapsed != 0.5f || g_drawEngine != &uiEngine) bad |= 2;
    if (c.LastRenderTime != 1.5 || c.FrameCounter != 1 || g_flagCalls != 0) bad |= 4;
    g_time = 2.0;
    c.DrawPreparePrimitivesForRendering(info);           // frame 2: flag off + backdrop
    if (g_n != 10 || g_log[8] != 90 || g_log[9] != 7 || g_backdrop != 1.0f || g_flagCalls != 1) bad |= 8;
    c.DrawPreparePrimitivesForRendering(info);           // frame 3: backdrop only
    if (g_n != 15 || g_log[14] != 7 || g_flagCalls != 1) bad |= 16;
    thing.m_presentBits[0] = 0;
    c.DrawPreparePrimitivesForRendering(info);           // no inventory component
    if (g_n != 19) bad |= 32;
    if (bad) { std::printf("0055d5d0_TEST FAIL\n"); return 1; }
    std::printf("0055d5d0_TEST PASS\n");
    return 0;
}