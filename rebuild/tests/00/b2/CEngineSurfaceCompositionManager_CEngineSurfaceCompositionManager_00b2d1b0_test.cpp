// Behaviour test for CEngineSurfaceCompositionManager::~CEngineSurfaceCompositionManager: every cached composite surface drops
// its graphic-frame reference (frame data deleted, after Uninitialise when initialised, only on the last reference; the
// pointer cleared either way), releases its managed texture and is removed from the resource bank; then the members and bases
// are destroyed in reverse order.
#include <cstdio>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CGraphicFrameData { public:
    unsigned long ReferenceCount;
    int           Id;
    unsigned char IsInitialised : 1;
    ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); Log(1000 + Id); }
    void Uninitialise() { Log(1100 + Id); }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    void UnInitialise()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            FrameData = 0;
        }
    }
};
class CManagedTexture { public:
    int Id;
    bool Release() { Log(2000 + Id); return true; }
};
class CResource { public: int ResourceId; };
class CCompositeSurfaceCache : public CResource { public:
    CGraphicFrame   GraphicFrame;
    CManagedTexture ManagedTexture;
};
struct Node { Node* Next; CCompositeSurfaceCache* Data; };
class CResourceBank { public:
    virtual ~CResourceBank() { Log(9000); }
    void RemoveResource(CResource& resource) { Log(3000 + resource.ResourceId); }
};
class CEngineComponent { public: virtual ~CEngineComponent() { Log(9001); } };
class CEngineSurfaceCompositionManager : public CResourceBank, public CEngineComponent { public:
    Node* CachedSurfaces;                                   // sentinel of the circular list
    virtual ~CEngineSurfaceCompositionManager()
    {
        for (Node* it = CachedSurfaces->Next; it != CachedSurfaces; it = it->Next) {
            CCompositeSurfaceCache* cache = it->Data;
            cache->GraphicFrame.UnInitialise();
            cache->ManagedTexture.Release();
            RemoveResource(*cache);
        }
        Log(8000);                                          // members (shader, texture manager, banks, list) follow
    }
};
int main()
{
    CGraphicFrameData* a = new CGraphicFrameData; a->ReferenceCount = 1; a->Id = 1; a->IsInitialised = 1;
    CGraphicFrameData* b = new CGraphicFrameData; b->ReferenceCount = 2; b->Id = 2; b->IsInitialised = 0;
    CCompositeSurfaceCache c1, c2, c3;
    c1.ResourceId = 1; c1.GraphicFrame.FrameData = a; c1.ManagedTexture.Id = 1;
    c2.ResourceId = 2; c2.GraphicFrame.FrameData = b; c2.ManagedTexture.Id = 2;
    c3.ResourceId = 3; c3.GraphicFrame.FrameData = 0; c3.ManagedTexture.Id = 3;
    Node sentinel, n1, n2, n3;
    sentinel.Next = &n1; n1.Next = &n2; n2.Next = &n3; n3.Next = &sentinel;
    n1.Data = &c1; n2.Data = &c2; n3.Data = &c3;
    CEngineSurfaceCompositionManager* m = new CEngineSurfaceCompositionManager;
    m->CachedSurfaces = &sentinel;
    CResourceBank* bank = m;
    delete bank;
    const int want[] = { 1101, 1001, 2001, 3001, 2002, 3002, 2003, 3003, 8000, 9001, 9000 };
    int n = sizeof(want) / sizeof(want[0]);
    bool ok = g_n == n && c1.GraphicFrame.FrameData == 0 && c2.GraphicFrame.FrameData == 0 && b->ReferenceCount == 1;
    for (int i = 0; ok && i < n; ++i) ok = g_log[i] == want[i];
    delete b;
    if (!ok) { std::printf("00b2d1b0_TEST FAIL\n"); return 1; }
    std::printf("00b2d1b0_TEST PASS\n");
    return 0;
}