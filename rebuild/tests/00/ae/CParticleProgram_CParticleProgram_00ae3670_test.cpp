// Behaviour test for CParticleProgram::~CParticleProgram (0x00AE3670): Clear() runs first, then every member is torn down
// in reverse declaration order: the arrays free their blocks, the export map destroys each key before freeing, the
// constraint stream and the group arrays run their destructors, the embedded simulation clears itself and releases its
// counted render data (the deleter runs when the count reaches zero).
#include <cstdio>
#include <cstdlib>

static char g_log[256];
static int g_len = 0;
static int g_frees = 0;
static void Log(char c) { if (g_len < 255) g_log[g_len++] = c; g_log[g_len] = 0; }
static void TestFree(void* p) { ++g_frees; std::free(p); }

template <class T> class TArray {                       // STLport vector shape: start / finish / end of storage
public:
    T* _M_start; T* _M_finish; T* _M_end;
    TArray() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~TArray() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); if (_M_start) TestFree(_M_start); }
};

class CCharString { public: int id; CCharString() : id(0) {} ~CCharString() { Log('k'); } };
struct SPair { CCharString first; unsigned long second; };

struct SInfo { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteRenderData(void*) { Log('d'); }
class CCountedRenderData {
public:
    void* Data; SInfo* Info;
    ~CCountedRenderData()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};

class CMovableResource { public: virtual ~CMovableResource() { Log('m'); } };
class CParticleSimulation : public CMovableResource {
public:
    CCountedRenderData RenderData;
    void Clear() { Log('s'); }
    virtual ~CParticleSimulation() { Clear(); }
};
class CMemoryDataOutputStream { public: virtual ~CMemoryDataOutputStream() { Log('o'); } };
class C3DGroup2 { public: long BoneIndex; ~C3DGroup2() { Log('g'); } };

class CParticleProgram : public CMovableResource {
public:
    CParticleSimulation InitialSimulation;
    TArray<SPair> ExportParticles;
    TArray<short> RenderTris, RenderQuads, RenderVertices;
    TArray<float> IndexedTextureCoords, TempVertexPositions;
    TArray<long> ParticleIndices, VertexIndices;
    TArray<C3DGroup2> SimGroups, NonSimGroups;
    CMemoryDataOutputStream Constraints;
    TArray<long> LoopStack;
    void Clear() { Log('c'); }
    virtual ~CParticleProgram() { Clear(); }
};

int main()
{
    CParticleProgram* p = new CParticleProgram;
    p->ExportParticles._M_start = (SPair*)std::malloc(sizeof(SPair) * 2);
    p->ExportParticles._M_finish = p->ExportParticles._M_start + 2;
    p->RenderTris._M_start = (short*)std::malloc(8); p->RenderTris._M_finish = p->RenderTris._M_start;
    p->LoopStack._M_start = (long*)std::malloc(8); p->LoopStack._M_finish = p->LoopStack._M_start;
    p->SimGroups._M_start = (C3DGroup2*)std::malloc(sizeof(C3DGroup2)); p->SimGroups._M_finish = p->SimGroups._M_start + 1;
    SInfo* info = new SInfo; info->RefCount = 1; info->DeleteFunc = DeleteRenderData; info->Data = 0;
    p->InitialSimulation.RenderData.Data = 0; p->InitialSimulation.RenderData.Info = info;
    delete p;
    // c = Clear, o = constraint stream, g = the one SimGroups element, kk = two map keys,
    // s = simulation Clear, d = render-data deleter (count 1 -> 0), m = CMovableResource (member, then base)
    const char* want = "cogkksdmm";
    int ok = 1;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) ok = 0;
    if (!ok || g_frees != 4) { std::printf("00ae3670_TEST FAIL\n"); return 1; }
    std::printf("00ae3670_TEST PASS\n");
    return 0;
}