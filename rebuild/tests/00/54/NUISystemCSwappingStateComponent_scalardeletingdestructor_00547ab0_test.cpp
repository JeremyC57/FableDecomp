// Behaviour test for the NUISystem::CSwappingStateComponent deleting destructor (0x00547ab0): the
// SwappingStates storage is freed (only when allocated) before the base destructor runs.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;
static void* g_freed = 0;

static void FreeBlock(void* p) { g_freed = p; g_events[g_count++] = 1; std::free(p); }

namespace NUISystem {

struct IComponentA { virtual void va00() {} unsigned char _state[0x10]; };
struct IComponentB { virtual void vb00() {} };
struct CComponentRoot { virtual ~CComponentRoot() {} };

struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long) {}
    virtual ~CChangingStateComponent() { g_events[g_count++] = 2; }
};

struct SSwappingState { unsigned long first; float second; };

struct CSwappingStateVector {
    SSwappingState* m_first; SSwappingState* m_last; SSwappingState* m_end;
    CSwappingStateVector() : m_first(0), m_last(0), m_end(0) {}
    ~CSwappingStateVector() { if (m_first) FreeBlock(m_first); }
};

struct CSwappingStateComponent : CChangingStateComponent {
    CSwappingStateVector SwappingStates;
    float                LastSwapTime;
    unsigned long        Seed;
    CSwappingStateComponent(long owner) : CChangingStateComponent(owner), LastSwapTime(0.0f), Seed(0) {}
    virtual ~CSwappingStateComponent() {}
    virtual void va00() {}
    virtual void vb00() {}
};

} // namespace NUISystem

int main()
{
    NUISystem::CSwappingStateComponent* a = new NUISystem::CSwappingStateComponent(1);
    void* block = std::malloc(16);
    a->SwappingStates.m_first = (NUISystem::SSwappingState*)block;
    NUISystem::CComponentRoot* root = a;
    delete root;
    bool ok = g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    NUISystem::CSwappingStateComponent* b = new NUISystem::CSwappingStateComponent(2);
    delete b;
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("00547ab0_TEST FAIL\n");
        return 1;
    }
    std::printf("00547ab0_TEST PASS\n");
    return 0;
}