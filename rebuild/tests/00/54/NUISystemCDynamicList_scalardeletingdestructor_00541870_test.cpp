// Behaviour test for the NUISystem::CDynamicList deleting destructor (0x00541870): the
// ScrollingChildren storage is freed (only when allocated) before the base destructor runs.
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

class CChangingStateComponent : public CComponentRoot, public IComponentA, public IComponentB { public:
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long) {}
    virtual ~CChangingStateComponent() { g_events[g_count++] = 2; }
};

class CComponent;

struct CComponentPtrVector {
    CComponent** m_first; CComponent** m_last; CComponent** m_end;
    CComponentPtrVector() : m_first(0), m_last(0), m_end(0) {}
    ~CComponentPtrVector() { if (m_first) FreeBlock(m_first); }
};

struct C2DVector { float X; float Y; };

class CDynamicList : public CChangingStateComponent { public:
    long                BInitialised;
    C2DVector           PositionOffset;
    long                TopIndex;
    long                SelectionIndex;
    long                NumVisible;
    CComponentPtrVector ScrollingChildren;
    CDynamicList(long owner) : CChangingStateComponent(owner), BInitialised(0) {}
    virtual ~CDynamicList() {}
    virtual void va00() {}
    virtual void vb00() {}
};

} // namespace NUISystem

int main()
{
    NUISystem::CDynamicList* a = new NUISystem::CDynamicList(1);
    void* block = std::malloc(16);
    a->ScrollingChildren.m_first = (NUISystem::CComponent**)block;
    NUISystem::CComponentRoot* root = a;
    delete root;
    bool ok = g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    NUISystem::CDynamicList* b = new NUISystem::CDynamicList(2);
    delete b;
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("00541870_TEST FAIL\n");
        return 1;
    }
    std::printf("00541870_TEST PASS\n");
    return 0;
}