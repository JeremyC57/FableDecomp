// 0055b4c0 behaviour test for NUISystem::CClickable::CClickable(const CClickable&)
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
static bool g_clearAtGetData = false;
namespace NUISystem {
class CHoverable {                                          // stand-in: the base copy constructor 0x0055BA80
public:
    long State;
    CHoverable() : State(0) { }
    CHoverable(const CHoverable& other) : State(other.State) { g_copiedFrom = &other; }
};
class CClickable : public CHoverable {
public:
    long StateBeforeLeftClicked;
    bool WasLeftClicked;
    void* PActionsOnLeftClicked;
    void* PActionsOnLeftHeld;
    void* PActionsOnLeftUnclicked;
    long StateBeforeRightClicked;
    bool WasRightClicked;
    void* PActionsOnRightClicked;
    float InputDelay;
    float LastInputTime;
    CClickable() : StateBeforeLeftClicked(0), WasLeftClicked(false), PActionsOnLeftClicked(0), PActionsOnLeftHeld(0),
                   PActionsOnLeftUnclicked(0), StateBeforeRightClicked(0), WasRightClicked(false), PActionsOnRightClicked(0),
                   InputDelay(0.0f), LastInputTime(0.0f) { }
    CClickable(const CClickable& other);
    void GetDataFromDef()
    {
        ++g_getDataCalls;
        g_clearAtGetData = StateBeforeLeftClicked == 0 && !WasLeftClicked && PActionsOnLeftClicked == 0 && PActionsOnLeftHeld == 0
            && PActionsOnLeftUnclicked == 0 && StateBeforeRightClicked == 0 && !WasRightClicked && PActionsOnRightClicked == 0;
        InputDelay = 0.25f;
    }
};
CClickable::CClickable(const CClickable& other)
    : CHoverable(other),
      StateBeforeLeftClicked(0),
      WasLeftClicked(false),
      PActionsOnLeftClicked(0),
      PActionsOnLeftHeld(0),
      PActionsOnLeftUnclicked(0),
      StateBeforeRightClicked(0),
      WasRightClicked(false),
      PActionsOnRightClicked(0)
{
    GetDataFromDef();
}
}
static void run()
{
    static int dummy;
    NUISystem::CClickable source;
    source.State = 3;
    source.StateBeforeLeftClicked = 1;
    source.WasLeftClicked = true;
    source.PActionsOnLeftClicked = &dummy;
    source.PActionsOnRightClicked = &dummy;
    source.WasRightClicked = true;
    NUISystem::CClickable copy(source);
    CHECK(g_copiedFrom == &source);
    CHECK(copy.State == 3);
    CHECK(g_getDataCalls == 1);
    CHECK(g_clearAtGetData);
    CHECK(copy.InputDelay == 0.25f);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0055b4c0_TEST FAIL\n");
        return 1;
    }
    std::printf("0055b4c0_TEST PASS\n");
    return 0;
}