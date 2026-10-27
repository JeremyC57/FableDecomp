// 0054ea60 behaviour test for NUISystem::CViewport::CViewport(const CViewport&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
static const void* g_copiedFrom = 0;
static int g_getDataCalls = 0;
static unsigned long g_indexAtGetData = 99;
static bool g_handleEmptyAtGetData = false;
namespace NUISystem {
class CChangingStateComponent {                             // stand-in: the base copy constructor 0x0052CCA0
public:
    long State;
    CChangingStateComponent() : State(0) { }
    CChangingStateComponent(const CChangingStateComponent& other) : State(other.State) { g_copiedFrom = &other; }
};
class CEnginePrimitiveHandle {
public:
    void* Data;
    CEnginePrimitiveHandle() : Data(0) { }
};
template <class T> class CVectorView {
public:
    T* Start; T* Finish; T* EndOfStorage;
    CVectorView() : Start(0), Finish(0), EndOfStorage(0) { }
};
class CComponent;
class CViewport : public CChangingStateComponent {
public:
    CEnginePrimitiveHandle Handle;
    CVectorView<CComponent*> FakeChildren;
    unsigned long GraphicIndex;
    CViewport() : GraphicIndex(0) { }
    CViewport(const CViewport& other);
    void GetDataFromDef()
    {
        ++g_getDataCalls;
        g_indexAtGetData = GraphicIndex;
        g_handleEmptyAtGetData = Handle.Data == 0 && FakeChildren.Start == 0;
        GraphicIndex = 3;
    }
};
CViewport::CViewport(const CViewport& other)
    : CChangingStateComponent(other),
      GraphicIndex(0)
{
    GetDataFromDef();
}
}
static void run()
{
    static int dummy;
    NUISystem::CViewport source;
    source.State = 7;
    source.Handle.Data = &dummy;
    source.FakeChildren.Start = (NUISystem::CComponent**)&dummy;
    source.GraphicIndex = 5;
    NUISystem::CViewport copy(source);
    CHECK(g_copiedFrom == &source);
    CHECK(copy.State == 7);
    CHECK(g_getDataCalls == 1);
    CHECK(g_indexAtGetData == 0);
    CHECK(g_handleEmptyAtGetData);
    CHECK(copy.GraphicIndex == 3);
    CHECK(copy.FakeChildren.Finish == 0 && copy.FakeChildren.EndOfStorage == 0);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0054ea60_TEST FAIL\n");
        return 1;
    }
    std::printf("0054ea60_TEST PASS\n");
    return 0;
}