// Behaviour test for 0x006FB8D0 (STLport _Destroy<pair<CCharString, CSymbolMap::CSymbolInfo> >): runs the pair's
// destructor in place: EntryName, GroupName, then the key; Val untouched, storage not freed.
#include <cstdio>
#include <new>

static int g_log[16];
static int g_count = 0;
static void Log(int id) { if (g_count < 16) g_log[g_count++] = id; }

class CCharString {                                      // model: records its id when destroyed
public:
    CCharString() : Id(0) {}
    ~CCharString() { Log(Id); }
    int Id;
};

class CSymbolMap { public:
    class CSymbolInfo { public: CCharString GroupName; CCharString EntryName; long Val; };
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> inline void _Destroy(T* p) { p->~T(); }
}
typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> TEntry;

int main()
{
    unsigned char storage[sizeof(TEntry) + 8];
    TEntry* e = new (storage) TEntry;
    e->first.Id = 1; e->second.GroupName.Id = 2; e->second.EntryName.Id = 3; e->second.Val = 77;
    std::_Destroy(e);
    if (g_count != 3 || g_log[0] != 3 || g_log[1] != 2 || g_log[2] != 1 || e->second.Val != 77) {
        std::printf("006fb8d0_TEST FAIL\n"); return 1;
    }
    std::printf("006fb8d0_TEST PASS\n");
    return 0;
}