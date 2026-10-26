// Behaviour test for STLport vector<CEnginePrimitiveHandle>::vector(size_type n) (0x00485D30): allocate exactly n slots
// (none when n == 0), copy-construct a default (null) handle into each, finish = start + n; the default temporary's
// destructor releases nothing.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_released = 0;
static int g_lastHandle = 0;

struct CPrimitive {
    long RefCount;
    void OnLastHandleReleased() { ++g_lastHandle; }
    void Destroy() { ++g_released; }
};

class CEnginePrimitiveHandle { public:
    CPrimitive* Object;
    CEnginePrimitiveHandle() : Object(0) {}
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CEnginePrimitiveHandle()
    {
        if ((Object ? Object->RefCount : 0) == 1)
            Object->OnLastHandleReleased();
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
        Object = 0;
    }
};

struct HandleVector {
    CEnginePrimitiveHandle* Start;
    CEnginePrimitiveHandle* Finish;
    CEnginePrimitiveHandle* EndOfStorage;
    explicit HandleVector(unsigned int n)
    {
        Start = n != 0 ? (CEnginePrimitiveHandle*)std::malloc(n * sizeof(CEnginePrimitiveHandle)) : 0;
        Finish = Start;
        EndOfStorage = Start + n;
        CEnginePrimitiveHandle x;                       // the default temporary
        CEnginePrimitiveHandle* cur = Start;
        for (unsigned int i = n; i > 0; --i, ++cur)
            new (cur) CEnginePrimitiveHandle(x);
        Finish = cur;
    }
    ~HandleVector()
    {
        for (CEnginePrimitiveHandle* p = Start; p != Finish; ++p)
            p->~CEnginePrimitiveHandle();
        if (Start)
            std::free(Start);
    }
};

int main()
{
    {
        HandleVector v(5);
        if (v.Finish - v.Start != 5 || v.EndOfStorage != v.Finish) { std::printf("00485d30_TEST FAIL\n"); return 1; }
        for (CEnginePrimitiveHandle* p = v.Start; p != v.Finish; ++p)
            if (p->Object != 0) { std::printf("00485d30_TEST FAIL\n"); return 1; }
    }
    {
        HandleVector e(0);
        if (e.Start != 0 || e.Finish != 0 || e.EndOfStorage != 0) { std::printf("00485d30_TEST FAIL\n"); return 1; }
    }
    if (g_released != 0 || g_lastHandle != 0) { std::printf("00485d30_TEST FAIL\n"); return 1; }
    std::printf("00485d30_TEST PASS\n");
    return 0;
}