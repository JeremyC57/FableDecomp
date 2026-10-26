// Behaviour test for 0041d03c: src/0041d03c.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NUISystem::CManager::RenderErrorMessage(CRenderContext* context)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CManager::RenderErrorMessage(CRenderContext* context) -- advance the error-message animation by the real time since the last call, then render the error-message component one meta layer above the current one and restore the layer
// Leaf: RenderErrorMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041D03C, length 108 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double; twice), CManager::UpdateErrorMessage(float) @ 0x0041CE9D, own virtual slots
//   35 GetMetaLayer (+0x8c) and 34 SetMetaLayer (+0x88) (vtable 0x01230134), the error component's virtual slot 2 (+0x08) with four zeros.
// Layout (retail NUISystem::CManager): error-message component +0xa0, last update time (double) +0xa8 (`#pragma pack(4)` keeps the
//   vptr in a 4-byte slot).
// Shape: the delta is an IMPLICIT double->float conversion in the declaration; the explicit `(float)(...)` cast schedules
//   `fld [ebp-4]` above the argument `push ecx` (retail: push first).
#pragma optimize("y", off)
#pragma optimize("s", on)
double __fastcall GFGetTime();                     // 0x009E1BC0
struct CRenderContext;
namespace NUISystem {
class CComponent { public:
    virtual void v00(); virtual void v01();
    virtual void Render(CRenderContext* context, long x, long y, long flags, long alpha);   // slot 2 (+0x08)
};
#pragma pack(push, 4)
class CManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33();
    virtual void SetMetaLayer(long layer);         // slot 34 (+0x88)
    virtual long GetMetaLayer();                   // slot 35 (+0x8c)
    unsigned char _pad04[0xa0 - 4];
    CComponent*   m_pErrorMessage;                 // +0xa0
    unsigned char _pada4[4];
    double        m_lastErrorUpdateTime;           // +0xa8
    void UpdateErrorMessage(float deltaTime);      // 0x0041CE9D
    void RenderErrorMessage(CRenderContext* context);
};
#pragma pack(pop)
void CManager::RenderErrorMessage(CRenderContext* context)
{
    float deltaTime = GFGetTime() - m_lastErrorUpdateTime;
    m_lastErrorUpdateTime = GFGetTime();
    UpdateErrorMessage(deltaTime);
    long layer = GetMetaLayer();
    SetMetaLayer(layer + 1);
    m_pErrorMessage->Render(context, 0, 0, 0, 0);
    SetMetaLayer(layer);
}
}

// ---- stubs and driver ----
void NUISystem::CComponent::v00() {}
void NUISystem::CComponent::v01() {}
void NUISystem::CManager::v00() {}
void NUISystem::CManager::v01() {}
void NUISystem::CManager::v02() {}
void NUISystem::CManager::v03() {}
void NUISystem::CManager::v04() {}
void NUISystem::CManager::v05() {}
void NUISystem::CManager::v06() {}
void NUISystem::CManager::v07() {}
void NUISystem::CManager::v08() {}
void NUISystem::CManager::v09() {}
void NUISystem::CManager::v10() {}
void NUISystem::CManager::v11() {}
void NUISystem::CManager::v12() {}
void NUISystem::CManager::v13() {}
void NUISystem::CManager::v14() {}
void NUISystem::CManager::v15() {}
void NUISystem::CManager::v16() {}
void NUISystem::CManager::v17() {}
void NUISystem::CManager::v18() {}
void NUISystem::CManager::v19() {}
void NUISystem::CManager::v20() {}
void NUISystem::CManager::v21() {}
void NUISystem::CManager::v22() {}
void NUISystem::CManager::v23() {}
void NUISystem::CManager::v24() {}
void NUISystem::CManager::v25() {}
void NUISystem::CManager::v26() {}
void NUISystem::CManager::v27() {}
void NUISystem::CManager::v28() {}
void NUISystem::CManager::v29() {}
void NUISystem::CManager::v30() {}
void NUISystem::CManager::v31() {}
void NUISystem::CManager::v32() {}
void NUISystem::CManager::v33() {}
static double g_times[2] = { 3.5, 4.0 }; static int g_timeCalls = 0;
double __fastcall GFGetTime() { return g_times[g_timeCalls++]; }
static int g_log[16]; static int g_n = 0; static float g_delta = 0; static long g_layers[4]; static int g_nl = 0;
static CRenderContext* g_ctx = 0; static long g_args[4];
void NUISystem::CComponent::Render(CRenderContext* context, long x, long y, long flags, long alpha)
{ g_log[g_n++] = 3; g_ctx = context; g_args[0] = x; g_args[1] = y; g_args[2] = flags; g_args[3] = alpha; }
void NUISystem::CManager::SetMetaLayer(long layer) { g_log[g_n++] = 2; g_layers[g_nl++] = layer; }
long NUISystem::CManager::GetMetaLayer() { g_log[g_n++] = 1; return 7; }
void NUISystem::CManager::UpdateErrorMessage(float deltaTime) { g_log[g_n++] = 0; g_delta = deltaTime; }
int main()
{
    NUISystem::CComponent comp; NUISystem::CManager m;
    m.m_pErrorMessage = &comp; m.m_lastErrorUpdateTime = 1.0;
    m.RenderErrorMessage((CRenderContext*)&g_timeCalls);
    CHECK(g_timeCalls == 2); CHECK(g_delta == 2.5f); CHECK(m.m_lastErrorUpdateTime == 4.0);
    CHECK(g_n == 5); CHECK(g_log[0] == 0); CHECK(g_log[1] == 1); CHECK(g_log[2] == 2); CHECK(g_log[3] == 3); CHECK(g_log[4] == 2);
    CHECK(g_nl == 2); CHECK(g_layers[0] == 8); CHECK(g_layers[1] == 7);
    CHECK(g_ctx == (CRenderContext*)&g_timeCalls); CHECK(g_args[0] == 0 && g_args[1] == 0 && g_args[2] == 0 && g_args[3] == 0);
    if (fails == 0) printf("0041d03c_TEST PASS\n"); else printf("0041d03c_TEST FAIL\n");
    return fails;
}