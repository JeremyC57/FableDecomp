// Behaviour test for CEngine::~CEngine (0x00B261D0): the user body Uninitialise(), then EngineComponents / RenderLayers / ScratchZBuffersD24S8 storage, FrameMemoryPool (ClearMemoryBlocks then ~CBaseClassNonCopyable), PEngineVideoOptionsDefs[3..0], PEngineDef, PEnvironmentDef (def pointers: drop a reference, Delete on the last), PreloadCameras storage, ~CIEngine, ~CInitBaseClass.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct DefModel { char Tag; unsigned long RefCount; void Delete() { Log(Tag); } };
struct DefPointerModel {
    DefModel* Object;
    DefPointerModel() : Object(0) { }
    ~DefPointerModel() { if (Object) { DefModel* o = Object; if (--o->RefCount == 0) o->Delete(); Object = 0; } }
};
struct ArrayModel { char Tag; void* Start; ArrayModel() : Start(0) { } ~ArrayModel() { if (Start) { Log(Tag); std::free(Start); } } };
struct BaseClassNonCopyableModel { virtual ~BaseClassNonCopyableModel() { Log('N'); } };
struct SimpleMemoryPoolModel : BaseClassNonCopyableModel { virtual ~SimpleMemoryPoolModel() { Log('C'); } };
struct InitBaseClassModel { virtual ~InitBaseClassModel() { Log('B'); } bool Valid; };
struct IEngineModel : InitBaseClassModel { virtual ~IEngineModel() { Log('I'); } bool Active; };
struct DeviceResetCallbackModel { virtual void OnPreDeviceReset() = 0; };
struct EngineModel : IEngineModel, DeviceResetCallbackModel {
    virtual void OnPreDeviceReset() { }
    void Uninitialise() { Log('U'); }
    ~EngineModel() { Uninitialise(); }
    ArrayModel            PreloadCameras;
    DefPointerModel       PEnvironmentDef;
    DefPointerModel       PEngineDef;
    DefPointerModel       PEngineVideoOptionsDefs[4];
    SimpleMemoryPoolModel FrameMemoryPool;
    ArrayModel            ScratchZBuffersD24S8;
    ArrayModel            RenderLayers;
    ArrayModel            EngineComponents;
};
int main()
{
    DefModel defs[6] = { { '0', 1 }, { '1', 1 }, { '2', 2 }, { '3', 1 }, { 'e', 1 }, { 'v', 1 } };
    EngineModel* e = new EngineModel;
    e->PreloadCameras.Tag = 'P'; e->PreloadCameras.Start = std::malloc(8);
    e->PEnvironmentDef.Object = &defs[5]; e->PEngineDef.Object = &defs[4];
    for (int i = 0; i < 4; ++i) e->PEngineVideoOptionsDefs[i].Object = &defs[i];
    e->ScratchZBuffersD24S8.Tag = '1'; e->ScratchZBuffersD24S8.Start = std::malloc(8);
    e->RenderLayers.Tag = '2'; e->RenderLayers.Start = std::malloc(8);
    e->EngineComponents.Tag = '3'; e->EngineComponents.Start = std::malloc(8);
    InitBaseClassModel* p = e;
    delete p;
    // def 2 is still referenced elsewhere: released (count 2 -> 1) but not deleted
    if (std::strcmp(g_log, "U321CN310evPIB") != 0 || defs[2].RefCount != 1 || defs[3].RefCount != 0) { std::printf("00b261d0_TEST FAIL\n"); return 1; }
    std::printf("00b261d0_TEST PASS\n");
    return 0;
}