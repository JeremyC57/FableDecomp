// 0055ba80 behaviour test for NUISystem::CHoverable::CHoverable(const CHoverable&)
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
static const void* g_observerAdded = 0;
static int g_addCalls = 0;
static bool g_fieldsClearAtAdd = false;
namespace NUISystem {
class CObserver { public: long Tag; CObserver() : Tag(0) { } };
class CChangingStateComponent : public CObserver {
public:
    long State;
    CChangingStateComponent() : State(0) { }
    CChangingStateComponent(const CChangingStateComponent& other) : CObserver(other), State(other.State) { g_copiedFrom = &other; }
};
class CHoverable;
class CManager {
public:
    void AddObserver(CObserver* observer);
    static CManager* __fastcall GetInstance() { static CManager instance; return &instance; }
};
class CHoverable : public CChangingStateComponent {
public:
    long StateBeforeHovered;
    bool WasHovered;
    void* PActionsOnHovered;
    void* PActionsOnUnhovered;
    CHoverable() : StateBeforeHovered(0), WasHovered(false), PActionsOnHovered(0), PActionsOnUnhovered(0) { }
    CHoverable(const CHoverable& other);
};
void CManager::AddObserver(CObserver* observer)
{
    ++g_addCalls;
    g_observerAdded = observer;
    CHoverable* h = static_cast<CHoverable*>(observer);
    g_fieldsClearAtAdd = h->StateBeforeHovered == 0 && !h->WasHovered && h->PActionsOnHovered == 0 && h->PActionsOnUnhovered == 0;
}
CHoverable::CHoverable(const CHoverable& other)
    : CChangingStateComponent(other),
      StateBeforeHovered(0),
      WasHovered(false),
      PActionsOnHovered(0),
      PActionsOnUnhovered(0)
{
    CManager::GetInstance()->AddObserver(this);
}
}
static void run()
{
    static int dummy;
    NUISystem::CHoverable source;
    source.State = 9;
    source.StateBeforeHovered = 2;
    source.WasHovered = true;
    source.PActionsOnHovered = &dummy;
    source.PActionsOnUnhovered = &dummy;
    NUISystem::CHoverable copy(source);
    CHECK(g_copiedFrom == &source);
    CHECK(copy.State == 9);
    CHECK(g_addCalls == 1);
    CHECK(g_observerAdded == static_cast<NUISystem::CObserver*>(&copy));
    CHECK(g_fieldsClearAtAdd);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0055ba80_TEST FAIL\n");
        return 1;
    }
    std::printf("0055ba80_TEST PASS\n");
    return 0;
}