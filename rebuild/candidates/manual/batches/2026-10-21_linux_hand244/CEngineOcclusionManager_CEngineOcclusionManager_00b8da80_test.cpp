#include <cstdio>
#include <cstdlib>
static int g_dataDeleted = 0, g_ibUninit = 0, g_vbUninit = 0, g_rootDtor = 0, g_freed = 0, g_pageFreed = -1, g_restore = 0, g_surfUninit = 0, g_baseClassDtor = 0, g_rangeDestroyed = 0;
struct CPixelShaderData { virtual ~CPixelShaderData() { ++g_dataDeleted; } long m_refCount; CPixelShaderData(long r) : m_refCount(r) {} };
struct CVertexShaderData { virtual ~CVertexShaderData() { ++g_dataDeleted; } long m_refCount; CVertexShaderData(long r) : m_refCount(r) {} };
struct CShaderBase { virtual ~CShaderBase() {} };
struct CPixelShader : CShaderBase {
    CPixelShaderData* Data;
    CPixelShader() : Data(0) {}
    virtual ~CPixelShader() { CPixelShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
struct CVertexShader : CShaderBase {
    CVertexShaderData* Data;
    CVertexShader() : Data(0) {}
    void Release() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } Data = 0; }
    virtual ~CVertexShader() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
static void my_free(void* p) { ++g_freed; std::free(p); }

template <class T> struct CPODArray { T* _M_start; T* _M_finish; T* _M_end_of_storage; CPODArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {} ~CPODArray() { if (_M_start) my_free(_M_start); } };
template <class T> void Destroy_range(T* first, T* last) { g_rangeDestroyed += (int)(last - first); }
template <class T> struct CArray { T* _M_start; T* _M_finish; T* _M_end_of_storage; CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CArray() { Destroy_range(_M_start, _M_finish); if (_M_start) my_free(_M_start); } };
struct CVertexBufferWin32 { void Uninitialise() { ++g_vbUninit; } ~CVertexBufferWin32() { Uninitialise(); } };
struct CIndexBuffer { void Uninitialise() { ++g_ibUninit; } ~CIndexBuffer() { Uninitialise(); } };
struct CEngineComponent { ~CEngineComponent() { ++g_rootDtor; } };
struct Pair { unsigned long a; void* p; void* q; };
struct CEngineOcclusionManager : CEngineComponent {
    CPODArray<int> FlareTests, PrevFrameFlareTests, HandleMapper, PrevFrameHandleMapper;
    CArray<Pair> FlareVertexBuffers;
    CVertexShader VertexShaderFlareTest;
    CVertexBufferWin32 UnitBoxVB;
    CIndexBuffer UnitBoxIB;
    CVertexShader OcclusionBoxVShader;
    CPixelShader OcclusionBoxPixelShader;
    ~CEngineOcclusionManager();
};
CEngineOcclusionManager::~CEngineOcclusionManager() { VertexShaderFlareTest.Release(); }
int main()
{
    CEngineOcclusionManager* m = new CEngineOcclusionManager;
    m->FlareTests._M_start = (int*)std::malloc(16);
    m->PrevFrameHandleMapper._M_start = (int*)std::malloc(16);
    m->FlareVertexBuffers._M_start = (Pair*)std::malloc(2 * sizeof(Pair));
    m->FlareVertexBuffers._M_finish = m->FlareVertexBuffers._M_start + 2;
    m->VertexShaderFlareTest.Data = new CVertexShaderData(1);
    m->OcclusionBoxPixelShader.Data = new CPixelShaderData(1);
    delete m;
    bool ok = g_dataDeleted == 2 && g_freed == 3 && g_rangeDestroyed == 2 && g_vbUninit == 1 && g_ibUninit == 1 && g_rootDtor == 1;
    if (!ok) { std::printf("00b8da80_TEST FAIL\n"); return 1; }
    std::printf("00b8da80_TEST PASS\n");
    return 0;
}
