#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_scenesDeleted = 0, g_objectsDestroyed = 0, g_rangeCalls = 0, g_sharedDtor = 0, g_componentDtor = 0;
struct CCPPointerInfo {
    unsigned int RefCount;
    void ( *DeleteFunc)(void*);
    void* Data;
};


template<class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;

    ~CCountedPointer()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CEnginePrimitiveManager;
struct CEnginePrimitiveManagerStaticMeshes;
struct CEnginePrimitiveManagerAnimatedMeshes;
struct CEnginePrimitiveManagerRepeatedStaticMeshes;
struct CEnginePrimitiveManagerRepeatedZSprites;
struct CEnginePrimitiveManagerSprites;
struct CEnginePrimitiveManagerFlareSprites;
struct CEnginePrimitiveManagerWeaponTrails;
struct CEnginePrimitiveManagerSpriteTrails;
struct CEnginePrimitiveManagerLines;
struct CEnginePrimitiveManagerText;
struct CEnginePrimitiveManagerGameMeshes;
struct CEnginePrimitiveOcclusionGroupManager;
struct CEnginePrimitiveGroupManager;
struct CEnginePrimitiveChangerManager;
struct CEngineClothRenderer;
struct CEnginePrimitiveManagerParticleLineGroup;
struct CEnginePrimitiveManagerParticleMeshGroup;
struct CEnginePrimitiveManagerParticleSpriteGroup;
struct CEnginePrimitiveManagerParticleSpriteTrailGroup;
struct CEnginePrimitiveManagerParticleGroup;
struct CEnginePrimitiveManagerDecal;
struct CEnginePrimitiveManagerParticleDecalGroup;
struct CEnginePrimitiveManagerWeatherMask;

struct EmptyRangeTag {};
extern void  DestroyManagerRange(
    CCountedPointer<CEnginePrimitiveManager>* first,
    CCountedPointer<CEnginePrimitiveManager>* last,
    EmptyRangeTag* tag); // 0x00B38930

struct CManagerPointerArray {
    CCountedPointer<CEnginePrimitiveManager>* Start;
    CCountedPointer<CEnginePrimitiveManager>* Finish;
    CCountedPointer<CEnginePrimitiveManager>* EndOfStorage;

    ~CManagerPointerArray()
    {
        EmptyRangeTag tag;
        DestroyManagerRange(Start, Finish, &tag);
        if (Start != 0)
            free(Start);
    }
};

struct CEngineScene {
    virtual ~CEngineScene();
};

inline void* CopyTrivial(const void* first, const void* last, void* result)   // STLport __copy_trivial
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct CScenePointerArray {                         // STLport vector<CEngineScene*>, 12 bytes
    CEngineScene** _M_start;
    CEngineScene** _M_finish;
    CEngineScene** _M_end_of_storage;
    CEngineScene** begin() { return _M_start; }
    CEngineScene** end() { return _M_finish; }
    CEngineScene** erase(CEngineScene** first, CEngineScene** last)
    {
        CEngineScene** i = (CEngineScene**)CopyTrivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    ~CScenePointerArray()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

struct CEnginePrimitiveRendererSharedData {       // vtable 0x012A1380 (as the derived), 0x268 bytes
    virtual ~CEnginePrimitiveRendererSharedData();   // 0x00B36EC0
    unsigned char _pad004[0x264];
};

struct CEngineComponent {                          // +0x268, vtable 0x012A1348
    virtual ~CEngineComponent();                     // 0x00B59730
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
    unsigned char Padding[2];
};

struct CEnginePrimitiveRenderer : CEnginePrimitiveRendererSharedData, CEngineComponent {
    CManagerPointerArray PrimitiveManagers;          // +0x274
    CCountedPointer<CEnginePrimitiveManagerStaticMeshes> StaticMeshManager; // +0x280
    CCountedPointer<CEnginePrimitiveManagerAnimatedMeshes> AnimatedMeshManager; // +0x288
    CCountedPointer<CEnginePrimitiveManagerRepeatedStaticMeshes> RepeatedStaticMeshManager; // +0x290
    CCountedPointer<CEnginePrimitiveManagerRepeatedZSprites> RepeatedZSpriteManager; // +0x298
    CCountedPointer<CEnginePrimitiveManagerSprites> SpriteManager;     // +0x2a0
    CCountedPointer<CEnginePrimitiveManagerFlareSprites> FlareSpriteManager; // +0x2a8
    CCountedPointer<CEnginePrimitiveManagerWeaponTrails> WeaponTrailManager; // +0x2b0
    CCountedPointer<CEnginePrimitiveManagerSpriteTrails> SpriteTrailManager; // +0x2b8
    CCountedPointer<CEnginePrimitiveManagerLines> LineManager;         // +0x2c0
    CCountedPointer<CEnginePrimitiveManagerText> TextManager;          // +0x2c8
    CCountedPointer<CEnginePrimitiveManagerGameMeshes> GameMeshManager; // +0x2d0
    CCountedPointer<CEnginePrimitiveOcclusionGroupManager> OcclusionGroupManager; // +0x2d8
    CCountedPointer<CEnginePrimitiveGroupManager> PrimitiveGroupManager; // +0x2e0
    CCountedPointer<CEnginePrimitiveChangerManager> PrimitiveChangerManager; // +0x2e8
    CCountedPointer<CEngineClothRenderer> ClothRenderer;               // +0x2f0
    CCountedPointer<CEnginePrimitiveManagerParticleLineGroup> ParticleLineGroupManager; // +0x2f8
    CCountedPointer<CEnginePrimitiveManagerParticleMeshGroup> ParticleMeshGroupManager; // +0x300
    CCountedPointer<CEnginePrimitiveManagerParticleSpriteGroup> ParticleSpriteGroupManager; // +0x308
    CCountedPointer<CEnginePrimitiveManagerParticleSpriteTrailGroup> ParticleSpriteTrailGroupManager; // +0x310
    CCountedPointer<CEnginePrimitiveManagerParticleGroup> ParticleGroupManager; // +0x318
    CCountedPointer<CEnginePrimitiveManagerDecal> DecalManager;        // +0x320
    CCountedPointer<CEnginePrimitiveManagerParticleDecalGroup> DecalGroupManager; // +0x328
    CCountedPointer<CEnginePrimitiveManagerWeatherMask> WeatherMaskManager; // +0x330
    CScenePointerArray SceneList;                    // +0x338
    virtual ~CEnginePrimitiveRenderer();
};

CEnginePrimitiveRenderer* g_pPrimitiveRenderer;   // 0x01436E78

CEnginePrimitiveRenderer::~CEnginePrimitiveRenderer()
{
    for (CEngineScene** it = SceneList.begin(); it != SceneList.end(); ++it) {
        delete *it;
        *it = 0;
    }
    SceneList.clear();
    g_pPrimitiveRenderer = 0;
}

CEngineScene::~CEngineScene() { ++g_scenesDeleted; }
CEnginePrimitiveRendererSharedData::~CEnginePrimitiveRendererSharedData() { ++g_sharedDtor; }
CEngineComponent::~CEngineComponent() { ++g_componentDtor; }
void DestroyManagerRange(CCountedPointer<CEnginePrimitiveManager>* first, CCountedPointer<CEnginePrimitiveManager>* last, EmptyRangeTag*)
{
    ++g_rangeCalls;
    for (; first != last; ++first) first->~CCountedPointer<CEnginePrimitiveManager>();
}
static void DestroyObject(void*) { ++g_objectsDestroyed; }
static CCPPointerInfo* NewInfo(unsigned int refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs; info->DeleteFunc = DestroyObject; info->Data = 0;
    return info;
}

int main()
{
    CEnginePrimitiveRenderer* r = new CEnginePrimitiveRenderer;
    g_pPrimitiveRenderer = r;
    r->SceneList._M_start = (CEngineScene**)std::malloc(3 * sizeof(CEngineScene*));
    r->SceneList._M_start[0] = new CEngineScene;
    r->SceneList._M_start[1] = 0;
    r->SceneList._M_start[2] = new CEngineScene;
    r->SceneList._M_finish = r->SceneList._M_start + 3;
    r->SceneList._M_end_of_storage = r->SceneList._M_finish;
    r->PrimitiveManagers.Start = (CCountedPointer<CEnginePrimitiveManager>*)std::malloc(2 * sizeof(CCountedPointer<CEnginePrimitiveManager>));
    r->PrimitiveManagers.Start[0].Data = 0; r->PrimitiveManagers.Start[0].Info = NewInfo(1);
    r->PrimitiveManagers.Start[1].Data = 0; r->PrimitiveManagers.Start[1].Info = 0;
    r->PrimitiveManagers.Finish = r->PrimitiveManagers.Start + 2;
    r->PrimitiveManagers.EndOfStorage = r->PrimitiveManagers.Finish;
    CCountedPointer<CEnginePrimitiveManagerStaticMeshes>* first = &r->StaticMeshManager;
    CCountedPointer<CEnginePrimitiveManagerWeatherMask>* lastPtr = &r->WeatherMaskManager;
    char* p = (char*)first;
    int count = 0;
    while (p <= (char*)lastPtr) {
        CCountedPointer<CEnginePrimitiveManagerStaticMeshes>* cp = (CCountedPointer<CEnginePrimitiveManagerStaticMeshes>*)p;
        cp->Data = 0;
        cp->Info = (count % 2 == 0) ? NewInfo(1) : 0;
        ++count;
        p += sizeof(CCountedPointer<CEnginePrimitiveManagerStaticMeshes>);
    }
    CCPPointerInfo* shared = NewInfo(2);
    r->TextManager.Info = shared;   // one reference survives
    r->~CEnginePrimitiveRenderer();
    int expectedObjects = 1 + 12;   // manager array [0] + the 12 even-indexed members
    if (r->TextManager.Info == 0 && shared->RefCount == 1) { } else { std::printf("00b33520_TEST FAIL\n"); return 1; }
    int ok = g_scenesDeleted == 2 && g_pPrimitiveRenderer == 0 && g_rangeCalls == 1 && g_sharedDtor == 1 && g_componentDtor == 1
        && count == 23 && g_objectsDestroyed == expectedObjects;
    ::operator delete(r);
    delete shared;
    if (!ok) { std::printf("00b33520_TEST FAIL\n"); return 1; }
    std::printf("00b33520_TEST PASS\n");
    return 0;
}
