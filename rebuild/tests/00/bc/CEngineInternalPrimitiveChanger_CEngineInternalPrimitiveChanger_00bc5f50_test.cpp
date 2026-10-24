// Behaviour test for 00bc5f50 (CEngineInternalPrimitiveChanger destructor): the mesh effect is released first, then every primitive handle is destroyed and the array storage freed.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_events[16];
static int g_count = 0;
static int g_frees = 0;
static void test_free(void* p) { ++g_frees; std::free(p); }
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase() { }
    virtual void OnLastReference() { g_events[g_count++] = 1; }
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(0) { }
    void Release()
    {
        if (--IVRefCount == 0)
            OnLastReference();
    }
};
template <class T> struct CIVCountedPointer {
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    ~CIVCountedPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};
struct CEnginePersistentPrimitiveBase : CIVCountedPointeeBase { };
struct CEngineMeshEffect : CIVCountedPointeeBase { };
struct CEnginePrimitiveHandle {
    virtual ~CEnginePrimitiveHandle() { g_events[g_count++] = 2; }   // stands in for the out-of-line handle destructor
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            test_free(_M_start);
    }
};
template <class T> struct CArray : vector<T> { };
struct CEngineInternalPrimitiveBase : CIVCountedPointeeBase {
    unsigned char _body[0x40];
};
struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {
    CArray<CEnginePrimitiveHandle>        Primitives;
    CIVCountedPointer<CEngineMeshEffect>  MeshEffect;
    long                                  ActiveIndex;
    bool                                  PreloadAll;
    unsigned long                         PrimitiveFlags;
    CEngineInternalPrimitiveChanger() : ActiveIndex(0), PreloadAll(false), PrimitiveFlags(0) { }
    ~CEngineInternalPrimitiveChanger();
};
CEngineInternalPrimitiveChanger::~CEngineInternalPrimitiveChanger()
{
}
int main()
{
    CEngineMeshEffect effect;
    effect.IVRefCount = 1;
    CEnginePersistentPrimitiveBase primitive;
    primitive.IVRefCount = 5;
    {
        CEngineInternalPrimitiveChanger changer;
        changer.MeshEffect.m_p = &effect;
        CEnginePrimitiveHandle* handles = static_cast<CEnginePrimitiveHandle*>(std::malloc(2 * sizeof(CEnginePrimitiveHandle)));
        new (&handles[0]) CEnginePrimitiveHandle();
        new (&handles[1]) CEnginePrimitiveHandle();
        handles[0].Data.m_p = &primitive;
        changer.Primitives._M_start = handles;
        changer.Primitives._M_finish = handles + 2;
        changer.Primitives._M_end_of_storage = handles + 2;
    }
    // mesh effect's last reference (1), then the two handles (2, 2); the first handle released one primitive reference
    if (g_count != 3 || g_events[0] != 1 || g_events[1] != 2 || g_events[2] != 2) { std::printf("00bc5f50_TEST FAIL\n"); return 1; }
    if (g_frees != 1 || primitive.IVRefCount != 4 || effect.IVRefCount != 0) { std::printf("00bc5f50_TEST FAIL\n"); return 1; }
    std::printf("00bc5f50_TEST PASS\n");
    return 0;
}