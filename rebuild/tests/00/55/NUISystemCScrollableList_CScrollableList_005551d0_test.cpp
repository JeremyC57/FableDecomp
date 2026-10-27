// 005551d0 behaviour test for NUISystem::CScrollableList::CScrollableList(const CScrollableList&)
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
namespace NUISystem {
class CChangingStateComponent {
public:
    long State;
    CChangingStateComponent() : State(0) { }
    CChangingStateComponent(const CChangingStateComponent& other) : State(other.State) { g_copiedFrom = &other; }
};
class CComponent;
struct C2DVector { float X, Y; };
class CScrollableList : public CChangingStateComponent {
public:
    unsigned long TopOfList;
    unsigned long VisibleListItems;
    C2DVector PositionOffset;
    float ScrollTimer;
    CComponent* PUpArrow;
    CComponent* PDownArrow;
    bool UpArrowActive;
    bool DownArrowActive;
    CScrollableList() : TopOfList(0), VisibleListItems(0), ScrollTimer(0.0f), PUpArrow(0), PDownArrow(0), UpArrowActive(false), DownArrowActive(false) { }
    CScrollableList(const CScrollableList& other);
};
CScrollableList::CScrollableList(const CScrollableList& other)
    : CChangingStateComponent(other),
      ScrollTimer(0.0f),
      PUpArrow(0),
      PDownArrow(0),
      UpArrowActive(false),
      DownArrowActive(false)
{
}
}
static void run()
{
    static int dummy;
    NUISystem::CScrollableList source;
    source.State = 4;
    source.ScrollTimer = 2.5f;
    source.PUpArrow = (NUISystem::CComponent*)&dummy;
    source.PDownArrow = (NUISystem::CComponent*)&dummy;
    source.UpArrowActive = true;
    source.DownArrowActive = true;
    NUISystem::CScrollableList copy(source);
    CHECK(g_copiedFrom == &source);
    CHECK(copy.State == 4);
    CHECK(copy.ScrollTimer == 0.0f);
    CHECK(copy.PUpArrow == 0 && copy.PDownArrow == 0);
    CHECK(!copy.UpArrowActive && !copy.DownArrowActive);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005551d0_TEST FAIL\n");
        return 1;
    }
    std::printf("005551d0_TEST PASS\n");
    return 0;
}