#include <cstdio>
#include <stdlib.h>
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
template <class T> inline void _Destroy(T* first, T* last)   // STLport range destroy (by value: the end stays in a register)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
template <class T> class CMallocVector<T*> {             // pointer elements: nothing to destroy
public:
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CMainGameComponent;
class CGameDefinitionManager;
class CPlayer;
class CPlayerManager : public CBaseClassNonCopyable {
public:
    CMainGameComponent*         Component;               // +0x04
    CGameDefinitionManager*     DefinitionManager;       // +0x08
    CMallocVector<CPlayer*>     Players;                 // +0x0c
    long                        PlayerNeutral;           // +0x18
    long                        MainPlayer;              // +0x1c
    CMallocVector<CCharString>  HeroSwapPlayerScriptNames;   // +0x20
    virtual ~CPlayerManager();
private:
    void DestroyPlayers();                               // 0x0044A070
};
CPlayerManager::~CPlayerManager()
{
    DestroyPlayers();
}

static int g_destroy_players, g_strings, g_base;
CCharString::~CCharString() { ++g_strings; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_base; }
void CPlayerManager::DestroyPlayers() { ++g_destroy_players; }
int main()
{
    CPlayerManager* m = new CPlayerManager;
    m->Players._M_start = (CPlayer**)malloc(3 * sizeof(CPlayer*)); m->Players._M_finish = m->Players._M_end_of_storage = m->Players._M_start + 3;
    m->HeroSwapPlayerScriptNames._M_start = (CCharString*)malloc(2 * sizeof(CCharString));
    m->HeroSwapPlayerScriptNames._M_finish = m->HeroSwapPlayerScriptNames._M_end_of_storage = m->HeroSwapPlayerScriptNames._M_start + 2;
    delete m;
    bool ok = g_destroy_players == 1 && g_strings == 2 && g_base == 1;
    CPlayerManager* e = new CPlayerManager;
    e->Players._M_start = e->Players._M_finish = e->Players._M_end_of_storage = 0;
    e->HeroSwapPlayerScriptNames._M_start = e->HeroSwapPlayerScriptNames._M_finish = e->HeroSwapPlayerScriptNames._M_end_of_storage = 0;
    delete e;
    ok = ok && g_destroy_players == 2 && g_strings == 2 && g_base == 2 && sizeof(CPlayerManager) == 0x2c;
    std::printf(ok ? "0044a140_TEST PASS\n" : "0044a140_TEST FAIL\n");
    return ok ? 0 : 1;
}