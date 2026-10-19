#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CEngineLocalDetailGenerator::FrameStart(const CEngineComponentParams&) -- when local detail is enabled: on off-world-tick frames update every loaded map's local-detail cache against the time budget (now + LocalDetailMaxUpdateTime), then render every cache from the engine camera, and remember the camera position
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B59A30, 203 bytes, `ret 4` (frame pointer, 8-byte aligned stack for the double).
//   Mangled ?FrameStart@CEngineLocalDetailGenerator@@UAEXABVCEngineComponentParams@@@Z (vtable 0x012A2588 slot 1).
// Callees: GFGetFastSubFrameTimer 0x009E1C70 (`__fastcall`, double), CLocalDetailCacheMap::Update(double) 0x00BDED90,
//   CLocalDetailCacheMap::Render(CEngineCamera*, float) 0x00BDDD70. Globals: the engine camera 0x01436EA0
//   (CameraDesc.WorldPosition +0x54, Ego_r.pdb agrees), the engine state 0x01436E8C (loaded-map list at +0x2c).
// Layout (retail, as the landed ctor 0x00B59B60 / LoadNewScene 0x00B599C0): OldCameraPos +0x18, LocalDetailMaxUpdateTime
//   +0x30, LocalDetailEnabled +0x34 (the PDB members sit 0x38 higher); CEngineComponentParams::OffWorldTickProcessFlag +0x7c.
// Shape: the landed LoadNewScene list walks (a pointer local to the list, the head re-read after each call, each node's map
//   +4 cache null-tested); the time budget is a named double local; the camera position a C3DVector local built by the
//   memberwise user copy ctor (direct offsets) and assigned to OldCameraPos at the end (implicit assignment: `add ebx,18h`);
//   the render loop reads the camera global into a local first thing in the body (retail loads it at the rotated latch,
//   before the cache test).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CEngineCamera {
    unsigned char _pad[0x54];
    C3DVector     WorldPosition;                       // +0x54 (CameraDesc.WorldPosition)
};
extern CEngineCamera* g_pEngineCamera;                 // 0x01436EA0

double __fastcall GFGetFastSubFrameTimer();            // 0x009E1C70

struct CLocalDetailCacheMap {
    bool Update(double time);                          // 0x00BDED90
    void Render(CEngineCamera* camera, float alpha);   // 0x00BDDD70
};

struct CEngineMap {
    void*                 LandscapeMap;                // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;         // +0x04
};

struct SMapListNode {                                  // VC6 std::list node
    SMapListNode* m_pNext;                             // +0x00
    SMapListNode* m_pPrev;                             // +0x04
    CEngineMap*   m_pMap;                              // +0x08
};
struct CMapList {
    SMapListNode* m_pHead;                             // +0x00 sentinel
    unsigned long m_size;                              // +0x04
};
struct SEngineState {
    unsigned char _pad[0x2c];
    CMapList      m_loadedMaps;                        // +0x2c
};
extern SEngineState* g_pEngineState;                   // 0x01436E8C

struct CEngineComponentParams {
    unsigned char _pad[0x7c];
    bool          OffWorldTickProcessFlag;             // +0x7c
};

struct CEngineLocalDetailGenerator {
    virtual void FrameStart(const CEngineComponentParams& params);
    unsigned char _pad04[0x18 - 4];
    C3DVector     OldCameraPos;                        // +0x18
    unsigned char _pad24[0x30 - 0x24];
    float         LocalDetailMaxUpdateTime;            // +0x30
    bool          LocalDetailEnabled;                  // +0x34
    bool          DynamicUpdateEnabled;                // +0x35
};

void CEngineLocalDetailGenerator::FrameStart(const CEngineComponentParams& params)
{
    if (!LocalDetailEnabled)
        return;
    double endTime = GFGetFastSubFrameTimer() + LocalDetailMaxUpdateTime;
    C3DVector cameraPos = g_pEngineCamera->WorldPosition;
    CMapList* maps = &g_pEngineState->m_loadedMaps;
    SMapListNode* node;
    if (params.OffWorldTickProcessFlag) {
        for (node = maps->m_pHead->m_pNext; node != maps->m_pHead; node = node->m_pNext) {
            CLocalDetailCacheMap* cache = node->m_pMap->LocalDetailCacheMap;
            if (cache)
                cache->Update(endTime);
        }
    }
    for (node = maps->m_pHead->m_pNext; node != maps->m_pHead; node = node->m_pNext) {
        CEngineCamera* camera = g_pEngineCamera;
        CLocalDetailCacheMap* cache = node->m_pMap->LocalDetailCacheMap;
        if (cache)
            cache->Render(camera, 0.0f);
    }
    OldCameraPos = cameraPos;
}

// ---- test doubles and driver ----
static int g_log[32]; static int g_n = 0; static double g_time = -1.0; static CEngineCamera* g_cameraSeen = 0;
static void Log(int v) { if (g_n < 32) g_log[g_n++] = v; }
double __fastcall GFGetFastSubFrameTimer() { return 10.0; }
static CLocalDetailCacheMap g_caches[3];
bool CLocalDetailCacheMap::Update(double time) { g_time = time; Log(10 + (int)(this - g_caches)); return true; }
void CLocalDetailCacheMap::Render(CEngineCamera* camera, float alpha) { g_cameraSeen = camera; Log(20 + (int)(this - g_caches) + (alpha == 0.0f ? 0 : 50)); }
static CEngineCamera g_camera;
CEngineCamera* g_pEngineCamera = &g_camera;
static SEngineState g_state;
SEngineState* g_pEngineState = &g_state;
int main()
{
    static CEngineMap maps[3]; static SMapListNode head, n0, n1, n2;
    maps[0].LocalDetailCacheMap = &g_caches[0]; maps[1].LocalDetailCacheMap = 0; maps[2].LocalDetailCacheMap = &g_caches[2];
    head.m_pNext = &n0; n0.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head;
    n0.m_pMap = &maps[0]; n1.m_pMap = &maps[1]; n2.m_pMap = &maps[2];
    g_state.m_loadedMaps.m_pHead = &head;
    g_camera.WorldPosition.X = 1.0f; g_camera.WorldPosition.Y = 2.0f; g_camera.WorldPosition.Z = 3.0f;
    static CEngineLocalDetailGenerator gen; gen.LocalDetailEnabled = true; gen.LocalDetailMaxUpdateTime = 0.5f;
    gen.OldCameraPos.X = gen.OldCameraPos.Y = gen.OldCameraPos.Z = 0.0f;
    static CEngineComponentParams params; params.OffWorldTickProcessFlag = true;
    gen.FrameStart(params);
    static const int want1[] = { 10, 12, 20, 22 };
    bool ok = g_n == 4 && g_time == 10.5 && g_cameraSeen == &g_camera && gen.OldCameraPos.X == 1.0f && gen.OldCameraPos.Z == 3.0f;
    for (int i = 0; ok && i < 4; ++i) ok = g_log[i] == want1[i];
    g_n = 0; params.OffWorldTickProcessFlag = false; g_camera.WorldPosition.Y = 7.0f;
    gen.FrameStart(params);                            // no updates, only renders
    ok = ok && g_n == 2 && g_log[0] == 20 && g_log[1] == 22 && gen.OldCameraPos.Y == 7.0f;
    g_n = 0; gen.LocalDetailEnabled = false; g_camera.WorldPosition.Y = 9.0f;
    gen.FrameStart(params);                            // disabled: nothing
    ok = ok && g_n == 0 && gen.OldCameraPos.Y == 7.0f;
    if (!ok) { std::printf("00b59a30_TEST FAIL\n"); return 1; }
    std::printf("00b59a30_TEST PASS\n");
    return 0;
}
