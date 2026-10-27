// 0056e59b behaviour test for CTCInventoryExperience::SetInventorySelectionState
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
static int g_hookCalls = 0;
static long g_stateAtHook = -1;
class CTCInventoryBase {
public:
    enum EInventorySelectionState { SELECTION_NONE = 0, SELECTION_ITEM = 2 };
    bool FirstRenderFrame;
    EInventorySelectionState InventorySelectionState;
    CTCInventoryBase() : FirstRenderFrame(false), InventorySelectionState(SELECTION_NONE) { }
    virtual ~CTCInventoryBase() { }
    virtual void SetInventorySelectionState(EInventorySelectionState state) { InventorySelectionState = state; }
    void EmptyHook_005bc58b() { ++g_hookCalls; g_stateAtHook = InventorySelectionState; }
};
class CTCInventoryExperience : public CTCInventoryBase {
public:
    virtual void SetInventorySelectionState(EInventorySelectionState state);
};
void CTCInventoryExperience::SetInventorySelectionState(EInventorySelectionState state)
{
    InventorySelectionState = state;
    EmptyHook_005bc58b();
}
static void run()
{
    CTCInventoryExperience exp;
    CTCInventoryBase* base = &exp;
    base->SetInventorySelectionState(CTCInventoryBase::SELECTION_ITEM);
    CHECK(exp.InventorySelectionState == CTCInventoryBase::SELECTION_ITEM);
    CHECK(g_hookCalls == 1);
    CHECK(g_stateAtHook == CTCInventoryBase::SELECTION_ITEM);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0056e59b_TEST FAIL\n");
        return 1;
    }
    std::printf("0056e59b_TEST PASS\n");
    return 0;
}