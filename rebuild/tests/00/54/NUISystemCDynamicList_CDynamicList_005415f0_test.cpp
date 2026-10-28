// Behaviour test for 0x005415F0 NUISystem::CDynamicList::CDynamicList(long): the CChangingStateComponent(def) base first, then the list state cleared (BInitialised, TopIndex, SelectionIndex, NumVisible = 0; PositionOffset left alone), the scrolling-children vector empty, and GetDataFromDef() last
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_def = -1, g_step = 0, g_baseStep = 0, g_dataStep = 0, g_stateSeen = -1;
class C2DVector { public: float X, Y; };
struct CChangingStateComponent { CChangingStateComponent(int def) { g_def = def; g_baseStep = ++g_step; } };
struct Vec { void* start; void* finish; void* eos; Vec() : start(0), finish(0), eos(0) { } };
struct CDynamicList : CChangingStateComponent {
    int BInitialised; C2DVector PositionOffset; int TopIndex; int SelectionIndex; int NumVisible; Vec ScrollingChildren;
    CDynamicList(int def) : CChangingStateComponent(def), BInitialised(0), TopIndex(0), SelectionIndex(0), NumVisible(0) { GetDataFromDef(); }
    void GetDataFromDef() { g_dataStep = ++g_step; g_stateSeen = BInitialised + TopIndex + SelectionIndex + NumVisible + (ScrollingChildren.start ? 1 : 0); }
};
inline void* operator new(tsize, void* p) { return p; }
int main()
{
    static unsigned char raw[sizeof(CDynamicList)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CDynamicList* l = new (raw) CDynamicList(77);
    CHECK(g_def == 77 && g_baseStep == 1 && g_dataStep == 2 && g_stateSeen == 0);
    CHECK(l->ScrollingChildren.start == 0 && l->ScrollingChildren.eos == 0);
    unsigned char* po = (unsigned char*)&l->PositionOffset; CHECK(po[0] == 0xCD && po[7] == 0xCD);
    if (fails == 0) printf("005415f0_TEST PASS\n"); else printf("005415f0_TEST FAIL\n");
    return fails;
}