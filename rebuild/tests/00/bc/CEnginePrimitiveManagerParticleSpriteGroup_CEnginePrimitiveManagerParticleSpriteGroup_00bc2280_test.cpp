#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dataDeleted = 0, g_ibUninit = 0, g_vbUninit = 0, g_rootDtor = 0, g_freed = 0, g_pageFreed = -1, g_restore = 0, g_surfUninit = 0, g_baseClassDtor = 0, g_rangeDestroyed = 0;
struct CPixelShaderData { virtual ~CPixelShaderData() { ++g_dataDeleted; } long m_refCount; CPixelShaderData(long r) : m_refCount(r) {} };
struct CVertexShaderData { virtual ~CVertexShaderData() { ++g_dataDeleted; } long m_refCount; CVertexShaderData(long r) : m_refCount(r) {} };
class CShaderBase { public: virtual ~CShaderBase() {} };
class CPixelShader : public CShaderBase { public:
    CPixelShaderData* Data;
    CPixelShader() : Data(0) {}
    virtual ~CPixelShader() { CPixelShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
class CVertexShader : public CShaderBase { public:
    CVertexShaderData* Data;
    CVertexShader() : Data(0) {}
    void Release() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } Data = 0; }
    virtual ~CVertexShader() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
static void my_free(void* p) { ++g_freed; std::free(p); }

struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { ++g_rootDtor; } };
template <class T> void DestroyRange(T* first, T* last) { for (; first != last; ++first) first->~T(); }
template <class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CArray() { DestroyRange(_M_start, _M_finish); if (_M_start) my_free(_M_start); }
};
class CVertexBufferWin32 { public: void Uninitialise() { ++g_vbUninit; } ~CVertexBufferWin32() { Uninitialise(); } };
class CIndexBuffer { public: void Uninitialise() { ++g_ibUninit; } ~CIndexBuffer() { Uninitialise(); } };
class CEnginePrimitiveManagerParticleSpriteGroup : public CBaseClassNonCopyable { public:
    CArray<CVertexShader> VertexShadersBillboardSpritePointLights, VertexShadersCrossedSpritePointLights, VertexShaders3DFaceMeSpritePointLights;
    CVertexShader VertexShadersBillboardSpriteDisplacement, VertexShadersCrossedSpriteDisplacement, VertexShaders3DFaceMeSpriteDisplacement;
    CPixelShader PixelShader, PixelShaderStipple, PixelShaderDisplacement;
    CVertexBufferWin32 VertexBuffer;
    CIndexBuffer IndexBuffer;
};
int main()
{
    CEnginePrimitiveManagerParticleSpriteGroup* g = new CEnginePrimitiveManagerParticleSpriteGroup;
    CArray<CVertexShader>& a = g->VertexShadersCrossedSpritePointLights;
    a._M_start = (CVertexShader*)std::malloc(3 * sizeof(CVertexShader));
    for (int i = 0; i < 3; ++i) new (&a._M_start[i]) CVertexShader;
    a._M_start[1].Data = new CVertexShaderData(1);
    a._M_finish = a._M_start + 3; a._M_end_of_storage = a._M_finish;
    g->PixelShaderStipple.Data = new CPixelShaderData(1);
    g->VertexShaders3DFaceMeSpriteDisplacement.Data = new CVertexShaderData(1);
    delete g;
    bool ok = g_dataDeleted == 3 && g_freed == 1 && g_vbUninit == 1 && g_ibUninit == 1 && g_rootDtor == 1;
    if (!ok) { std::printf("00bc2280_TEST FAIL\n"); return 1; }
    std::printf("00bc2280_TEST PASS\n");
    return 0;
}