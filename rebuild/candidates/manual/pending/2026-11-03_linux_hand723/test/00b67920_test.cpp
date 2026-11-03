// Behaviour test for 0x00B67920 ~CEngineLandscapeRenderer: every existing world map loses its landscape map (deleted, pointer
// cleared), null map slots are skipped, the memory manager is deleted, and the members (shader data, storage) are released.
#include <cstdio>
#include <cstdlib>
static int gLandscapeDeleted = 0, gManagerDeleted = 0, gShaderDataDeleted = 0, gBaseDtor = 0;
struct CEngineLandscapeMap { virtual ~CEngineLandscapeMap() { ++gLandscapeDeleted; } };
struct CEngineMap { CEngineLandscapeMap* LandscapeMap;
    CEngineLandscapeMap* GetLandscapeMap() const { return LandscapeMap; }
    void SetLandscapeMap(CEngineLandscapeMap* m) { LandscapeMap = m; } };
struct CEngineWorldMap { CEngineMap** First; CEngineMap** Last;
    long GetMapCount() const { return (long)(Last - First); }
    CEngineMap* GetMap(long i) const { return First[i]; } };
static CEngineWorldMap gWorld;
static CEngineWorldMap* g_pEngineWorldMap = &gWorld;
struct CEngineLandscapeMemoryManager { virtual ~CEngineLandscapeMemoryManager() { ++gManagerDeleted; } };
struct CShaderData { unsigned long ReferenceCount; virtual ~CShaderData() { ++gShaderDataDeleted; } };
struct CShader { CShaderData* Data; CShader() : Data(0) { } ~CShader() { CShaderData* d = Data; if (d) { if (--d->ReferenceCount == 0) delete d; } } };
struct CList { void* Start; CList() : Start(malloc(8)) { } ~CList() { if (Start) free(Start); } };
struct CEngineComponent { virtual ~CEngineComponent() { ++gBaseDtor; } };
struct CEngineLandscapeRenderer : CEngineComponent {
    CShader BackgroundPixelShader;
    CList LandscapeThemes;
    CEngineLandscapeMemoryManager* MemoryManager;
    virtual ~CEngineLandscapeRenderer()
    {
        for (long i = 0; i < g_pEngineWorldMap->GetMapCount(); ++i) {
            if (g_pEngineWorldMap->GetMap(i)) {
                delete g_pEngineWorldMap->GetMap(i)->GetLandscapeMap();
                g_pEngineWorldMap->GetMap(i)->SetLandscapeMap(0);
            }
        }
        delete MemoryManager;
    }
};
int main()
{
    CEngineMap a, c; a.LandscapeMap = new CEngineLandscapeMap; c.LandscapeMap = 0;
    CEngineMap* maps[3] = { &a, 0, &c };
    gWorld.First = maps; gWorld.Last = maps + 3;
    CEngineLandscapeRenderer* r = new CEngineLandscapeRenderer;
    r->MemoryManager = new CEngineLandscapeMemoryManager;
    CShaderData* d = new CShaderData; d->ReferenceCount = 1; r->BackgroundPixelShader.Data = d;
    CEngineComponent* base = r;
    delete base;
    if (gLandscapeDeleted != 1 || a.LandscapeMap != 0 || c.LandscapeMap != 0 || gManagerDeleted != 1 || gShaderDataDeleted != 1 || gBaseDtor != 1) {
        std::printf("00b67920_TEST FAIL\n");
        return 1;
    }
    std::printf("00b67920_TEST PASS\n");
    return 0;
}
