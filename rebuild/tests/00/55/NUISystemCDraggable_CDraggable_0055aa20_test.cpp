// 0055aa20 behaviour test for NUISystem::CDraggable::CDraggable(const CDraggable&)
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
static bool g_stateAtGetData = false;
namespace NUISystem {
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CClickable {
public:
    long State;
    CClickable() : State(0) { }
    CClickable(const CClickable& other) : State(other.State) { g_copiedFrom = &other; }
};
class CDraggable : public CClickable {
public:
    C2DVector PositionBeforeDrag;
    bool WasDragged;
    void* PActionsOnDropped;
    void* PActionsOnDroppedNowhere;
    bool SnapBackIfDroppedNowhere;
    bool ChangeLayer;
    CDraggable() : PositionBeforeDrag(1.0f, 1.0f), WasDragged(true), PActionsOnDropped(0), PActionsOnDroppedNowhere(0),
                   SnapBackIfDroppedNowhere(false), ChangeLayer(true) { }
    CDraggable(const CDraggable& other);
    void GetDataFromDef()
    {
        ++g_getDataCalls;
        g_stateAtGetData = PositionBeforeDrag.X == 0.0f && PositionBeforeDrag.Y == 0.0f && !WasDragged && PActionsOnDropped == 0
            && PActionsOnDroppedNowhere == 0 && SnapBackIfDroppedNowhere && !ChangeLayer;
    }
};
CDraggable::CDraggable(const CDraggable& other)
    : CClickable(other),
      PositionBeforeDrag(0.0f, 0.0f),
      WasDragged(false),
      PActionsOnDropped(0),
      PActionsOnDroppedNowhere(0),
      SnapBackIfDroppedNowhere(true),
      ChangeLayer(false)
{
    GetDataFromDef();
}
}
static void run()
{
    static int dummy;
    NUISystem::CDraggable source;
    source.State = 6;
    source.PActionsOnDropped = &dummy;
    NUISystem::CDraggable copy(source);
    CHECK(g_copiedFrom == &source);
    CHECK(copy.State == 6);
    CHECK(g_getDataCalls == 1);
    CHECK(g_stateAtGetData);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0055aa20_TEST FAIL\n");
        return 1;
    }
    std::printf("0055aa20_TEST PASS\n");
    return 0;
}