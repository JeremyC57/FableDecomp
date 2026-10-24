// Behaviour test for 0x0066EAB7 NHeroInformationScreens::CManager::CManager(CWorld*): World stored, 11 null screen
// slots, empty inventory-screen / scroll-bar vectors before InitialiseScrollBarIndices(5), counters zero, fade built.
#include <stdio.h>
#include <stdlib.h>
#include <new>
class CWorld { public: int id; };
namespace NHeroInformationScreens {
class CBase { public: int id; };
struct CAllocator { CAllocator() { } };
template <class T> class CVector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CVector(unsigned int n, const T& value, const CAllocator& a)
    {
        _M_start = (T*)malloc(n * sizeof(T));
        for (unsigned int i = 0; i < n; ++i) _M_start[i] = value;
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
static int g_fadeBuilt = 0;
static long g_scrollCount = -1;
static bool g_scrollSawEmpty = false;
class CFadeInFadeOutBase { public: CFadeInFadeOutBase() { ++g_fadeBuilt; } long _data[10]; };
class CManager {
public:
    CManager(CWorld* world);
    void InitialiseScrollBarIndices(long count)
    {
        g_scrollCount = count;
        g_scrollSawEmpty = ScrollBarIndices._M_start == 0 && InventoryScreens._M_start == 0 && ScrollBarIndices.size() == 0
            && CurrentScreen == 0 && LastScreen == 0 && HighlightedScreen == 0 && !OpenFlag && g_fadeBuilt == 1;
    }
    CWorld*               World;
    CVector<const CBase*> Screens;
    CVector<const CBase*> InventoryScreens;
    CVector<long>         ScrollBarIndices;
    long                  CurrentScreen;
    long                  LastScreen;
    long                  HighlightedScreen;
    bool                  OpenFlag;
    CFadeInFadeOutBase    ScreenEffectFade;
};
enum { NUM_HERO_INFORMATION_SCREENS = 11 };
CManager::CManager(CWorld* world)
    : World(world),
      Screens(NUM_HERO_INFORMATION_SCREENS, 0, CAllocator()),
      CurrentScreen(0),
      LastScreen(0),
      HighlightedScreen(0),
      OpenFlag(false)
{
    InitialiseScrollBarIndices(5);
}
}
int main()
{
    using namespace NHeroInformationScreens;
    CWorld w;
    void* mem = malloc(sizeof(CManager));
    for (unsigned int i = 0; i < sizeof(CManager); ++i) ((unsigned char*)mem)[i] = 0xCD;
    CManager* m = new (mem) CManager(&w);
    bool ok = m->World == &w && m->Screens.size() == 11 && g_scrollCount == 5 && g_scrollSawEmpty;
    for (unsigned int i = 0; ok && i < 11; ++i) ok = m->Screens._M_start[i] == 0;
    if (!ok) { printf("0066eab7_TEST FAIL\n"); return 1; }
    printf("0066eab7_TEST PASS\n");
    return 0;
}
