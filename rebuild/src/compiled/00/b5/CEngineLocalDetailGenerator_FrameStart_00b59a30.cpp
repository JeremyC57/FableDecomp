// CEngineLocalDetailGenerator::FrameStart(const CEngineComponentParams&) -- when local detail is enabled: on off-world-tick frames update every loaded map's local-detail cache against the time budget (now + LocalDetailMaxUpdateTime), then render every cache from the engine camera, and remember the camera position
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B59A30, 205 bytes, `ret 4` (frame pointer, 8-byte aligned stack for the double).
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CEngineCamera { public:
    unsigned char _pad[0x54];
    C3DVector     WorldPosition;                       // +0x54 (CameraDesc.WorldPosition)
};
extern CEngineCamera* g_pEngineCamera;                 // 0x01436EA0

double __fastcall GFGetFastSubFrameTimer();            // 0x009E1C70

class CLocalDetailCacheMap { public:
    bool Update(double time);                          // 0x00BDED90
    void Render(CEngineCamera* camera, float alpha);   // 0x00BDDD70
};

class CEngineMap { public:
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

class CEngineComponentParams { public:
    unsigned char _pad[0x7c];
    bool          OffWorldTickProcessFlag;             // +0x7c
};

class CEngineLocalDetailGenerator { public:
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