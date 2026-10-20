#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
static char g_log[512]; static int g_frees, g_ranged;
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable() { LOG("base"); }
};

template <class T> struct CPODArray {                   // CArray<T> of a trivial T, 12 bytes
    typedef T value_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CPODArray() { if (_M_start) { ++g_frees; std::free(_M_start); } }
};

template <class T> struct allocator { allocator() {} };
template <class T> void FASTCALL _Destroy_range(T* first, T* last, const allocator<T>&) { g_ranged = (int)(last - first); LOG("range"); }

template <class T> struct CArray {                      // CArray<T> of a non-trivial T, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CArray()
    {
        _Destroy_range(_M_start, _M_finish, allocator<T>());
        if (_M_start) {
            ++g_frees;
            std::free(_M_start);
        }
    }
};

class CShaderResource;
template <class T> struct CCountedPointer { T* Object; void* Info; };

class CVertexShaderConstantLayout {                     // 0x14C bytes in retail
public:
    ~CVertexShaderConstantLayout() { LOG("layout"); }
    unsigned char _body[0x14c];
};

class CMemoryBlock;
class CSimpleMemoryPool : public CBaseClassNonCopyable {   // 0x1C bytes, vtable 0x0129A064
public:
    CMemoryBlock*  FirstMemoryBlock;                    // +0x04
    unsigned long  Capacity;                            // +0x08
    unsigned long  GrowthGranularity;                   // +0x0c
    unsigned char  Root[0xc];                           // +0x10 CArea
    void ClearMemoryBlocks() { LOG("pool"); }
    ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
};

class CPixelShaderData {
public:
    CPixelShaderData() {}
    virtual ~CPixelShaderData() { LOG("psdata"); }
    long _04;
    long RefCount;                                      // +0x08
};
class CShaderBase {                                     // vtable 0x0129A030
public:
    virtual ~CShaderBase() {}
};
class CPixelShader : public CShaderBase {               // 8 bytes, vtable 0x0129A054
public:
    CPixelShaderData* Data;                             // +0x04
    ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                delete data;
        }
    }
};

class CIndexedArray_CCharString {                       // CIndexedArray<CCharString>, 0x18 bytes in retail
public:
    ~CIndexedArray_CCharString() { LOG("names"); }
    unsigned char _body[0x18];
};

class CShaderRenderManager : public CBaseClassNonCopyable {   // vtable 0x0129A080
public:
    void*                                   CurrentVSConstantLayout;      // +0x04
    long                                    CurrentVSConstantLayoutIndex; // +0x08
    CPODArray<const CVertexShaderConstantLayout*> VertexShaderConstantLayouts;   // +0x0c
    CVertexShaderConstantLayout             NullConstantLayout;           // +0x18
    void*                                   Lights;                       // +0x164
    void*                                   LightBuffer;                  // +0x168
    unsigned long                           LightCount;                   // +0x16c
    unsigned long                           AllocatedLights;              // +0x170
    unsigned long                           AllocatedLightSize;           // +0x174
    CSimpleMemoryPool                       LightPool;                    // +0x178
    CPODArray<float>                        LightGlobals;                 // +0x194 CArray<C4DVector>
    CPODArray<float>                        LightAttenuations;            // +0x1a0 CArray<C4DVector>
    unsigned char                           _pad1ac[0x34];                // +0x1ac colours, render manager, device, shaders
    CPixelShader                            CurrentPixelShader;           // +0x1e0
    unsigned char                           _pad1e8[0x1c0];               // +0x1e8 matrices, flags, cycle textures
    CArray<CCountedPointer<CShaderResource> > ResourceManagers;           // +0x3a8
    CIndexedArray_CCharString               PSConstNames;                 // +0x3b4
    CPODArray<float>                        PSConstTable;                 // +0x3cc CArray<C4DVector>
    unsigned long                           UpdateFlags;                  // +0x3d8
    CPODArray<float>                        BoneMatrices;                 // +0x3dc CArray<CPreTransposedBoneMatrix>
    void Uninitialise() { LOG("uninit"); }
    CShaderRenderManager() {}
    virtual ~CShaderRenderManager();
};

CShaderRenderManager::~CShaderRenderManager()
{
    Uninitialise();
}

template <class T> static void Fill(T& a, int n) { a._M_start = n ? (typename T::value_type*)std::malloc(n * 4) : 0; a._M_finish = a._M_start + n; a._M_end_of_storage = a._M_finish; }
int main()
{
    void* raw = std::malloc(sizeof(CShaderRenderManager));
    std::memset(raw, 0, sizeof(CShaderRenderManager));
    CShaderRenderManager* m = new (raw) CShaderRenderManager;
    m->VertexShaderConstantLayouts._M_start = (const CVertexShaderConstantLayout**)std::malloc(8);
    m->LightGlobals._M_start = (float*)std::malloc(16);
    m->LightAttenuations._M_start = 0;
    m->ResourceManagers._M_start = (CCountedPointer<CShaderResource>*)std::malloc(3 * sizeof(CCountedPointer<CShaderResource>));
    m->ResourceManagers._M_finish = m->ResourceManagers._M_start + 3;
    m->PSConstTable._M_start = (float*)std::malloc(16);
    m->BoneMatrices._M_start = 0;
    CPixelShaderData* d = new CPixelShaderData; d->RefCount = 2;
    m->CurrentPixelShader.Data = d;
    m->~CShaderRenderManager();
    if (std::strcmp(g_log, "uninit;names;range;pool;base;layout;base;") || g_frees != 4 || g_ranged != 3 || d->RefCount != 1) { std::printf("0098abf0_TEST FAIL\n"); return 1; }
    g_log[0] = 0; g_frees = 0; std::memset(raw, 0, sizeof(CShaderRenderManager));
    m = new (raw) CShaderRenderManager;
    m->CurrentPixelShader.Data = d;   /* last reference: the data is deleted */
    m->~CShaderRenderManager();
    if (std::strcmp(g_log, "uninit;names;range;psdata;pool;base;layout;base;") || g_frees != 0 || g_ranged != 0) { std::printf("0098abf0_TEST FAIL\n"); return 1; }
    std::free(raw);
    std::printf("0098abf0_TEST PASS\n");
    return 0;
}