// Behaviour test for 0x00541350 NUISystem::CDynamicList::~CDynamicList(): the ScrollingChildren storage is released (the
// children themselves are not deleted), then the CChangingStateComponent base destructor runs.
#include <cstdio>
#include <cstdlib>

static int g_order[8];
static int g_n = 0;
static int g_componentsDeleted = 0;

class CComponent {
public:
    virtual ~CComponent() { ++g_componentsDeleted; }
};
class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent() { g_order[g_n++] = 2; --g_componentsDeleted; }
};
class CComponentVector {
public:
    CComponent** Start;
    CComponent** Finish;
    CComponent** End;
    CComponentVector() : Start(0), Finish(0), End(0) { }
    ~CComponentVector() { g_order[g_n++] = 1; if (Start) std::free(Start); }
};
class C2DVector { public: float X, Y; };
class CDynamicList : public CChangingStateComponent {
public:
    virtual ~CDynamicList() { }
    int BInitialised;
    C2DVector PositionOffset;
    long TopIndex;
    long SelectionIndex;
    long NumVisible;
    CComponentVector ScrollingChildren;
};

int main()
{
    CComponent child;
    CDynamicList* list = new CDynamicList;
    list->ScrollingChildren.Start = (CComponent**)std::malloc(2 * sizeof(CComponent*));
    list->ScrollingChildren.Start[0] = &child;
    list->ScrollingChildren.Start[1] = &child;
    list->ScrollingChildren.Finish = list->ScrollingChildren.End = list->ScrollingChildren.Start + 2;
    CComponent* base = list;
    delete base;
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_componentsDeleted != 0) {
        std::printf("00541350_TEST FAIL\n");
        return 1;
    }
    std::printf("00541350_TEST PASS\n");
    return 0;
}