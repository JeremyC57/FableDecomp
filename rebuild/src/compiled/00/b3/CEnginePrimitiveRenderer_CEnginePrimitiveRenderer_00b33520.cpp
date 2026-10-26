// CEnginePrimitiveRenderer::~CEnginePrimitiveRenderer() -- delete every scene in SceneList, clear it and the global renderer pointer, then destroy the members (scene array, 23 counted manager pointers, the manager array) and the CEngineComponent / CEnginePrimitiveRendererSharedData bases
// Leaf: ??1CEnginePrimitiveRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B33520, length 1472 bytes (body 0x5bd), plain `ret`.
// Callees: CEngineScene virtual deleting dtor (slot 0, flag 1), memmove @ 0x00BFEAE6 (STLport __copy_trivial in clear()),
//   free @ 0x00BFEA14 (vector storage), CCPPointerInfo::DeleteFunc (slot +4, object in ecx) + operator delete @ 0x00BFE9BC,
//   _Destroy_range<CCountedPointer> @ 0x00B38930 (__fastcall, landed), ~CEngineComponent @ 0x00B59730,
//   ~CEnginePrimitiveRendererSharedData @ 0x00B36EC0. Global: the primitive renderer pointer @ 0x01436E78.
// Layout: engine/CEnginePrimitiveRenderer.h is the PDB view with 16-byte CArrays and the two bases flattened; retail CArrays are
//   12 bytes, so every member sits 4 bytes earlier: PrimitiveManagers +0x274 (PDB +0x274, retail 12 bytes), the 23 manager
//   CCountedPointers +0x280..+0x330 (PDB +0x284..), SceneList +0x338 (PDB +0x33c). The bases: CEnginePrimitiveRendererSharedData
//   (vtable 0x012A1380, 0x268 bytes) and CEngineComponent at +0x268 (vtable 0x012A1348, 0xc bytes), as in the landed
//   CEnginePrimitiveRenderer2D dtor 0x00B4AAA0 (whose CCountedPointer / manager-array models are reused here).
#include <stddef.h>

class CCPPointerInfo { public:
    unsigned int RefCount;
    void (__fastcall *DeleteFunc)(void*);
    void* Data;
};

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* pointer);                                         // 0x00BFEA14
void __cdecl operator delete(void* pointer);                                         // 0x00BFE9BC

template<class T> class CCountedPointer { public:
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

class CEnginePrimitiveManager;
class CEnginePrimitiveManagerStaticMeshes;
class CEnginePrimitiveManagerAnimatedMeshes;
class CEnginePrimitiveManagerRepeatedStaticMeshes;
class CEnginePrimitiveManagerRepeatedZSprites;
class CEnginePrimitiveManagerSprites;
class CEnginePrimitiveManagerFlareSprites;
class CEnginePrimitiveManagerWeaponTrails;
class CEnginePrimitiveManagerSpriteTrails;
class CEnginePrimitiveManagerLines;
class CEnginePrimitiveManagerText;
class CEnginePrimitiveManagerGameMeshes;
class CEnginePrimitiveOcclusionGroupManager;
class CEnginePrimitiveGroupManager;
class CEnginePrimitiveChangerManager;
class CEngineClothRenderer;
class CEnginePrimitiveManagerParticleLineGroup;
class CEnginePrimitiveManagerParticleMeshGroup;
class CEnginePrimitiveManagerParticleSpriteGroup;
class CEnginePrimitiveManagerParticleSpriteTrailGroup;
class CEnginePrimitiveManagerParticleGroup;
class CEnginePrimitiveManagerDecal;
class CEnginePrimitiveManagerParticleDecalGroup;
class CEnginePrimitiveManagerWeatherMask;

struct EmptyRangeTag {};
extern void __fastcall DestroyManagerRange(
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

class CEngineScene { public:
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

class CEngineComponent { public:                          // +0x268, vtable 0x012A1348
    virtual ~CEngineComponent();                     // 0x00B59730
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
    unsigned char Padding[2];
};

class CEnginePrimitiveRenderer : public CEnginePrimitiveRendererSharedData, public CEngineComponent { public:
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

extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;   // 0x01436E78

CEnginePrimitiveRenderer::~CEnginePrimitiveRenderer()
{
    for (CEngineScene** it = SceneList.begin(); it != SceneList.end(); ++it) {
        delete *it;
        *it = 0;
    }
    SceneList.clear();
    g_pPrimitiveRenderer = 0;
}