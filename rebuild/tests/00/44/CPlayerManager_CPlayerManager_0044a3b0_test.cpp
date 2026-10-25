// Behaviour test for CPlayerManager::CPlayerManager (0x0044A3B0): pointers stored, empty player list, the four hero-swap
// script names appended in order (through both the in-place and the overflow push_back paths).
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CCharString {
    char text[24];
    CCharString(const char* t, long = -1) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
};
template <class T> struct Vector {
    T* _M_start; T* _M_finish; T* _M_end;
    Vector() : _M_start(0), _M_finish(0), _M_end(0) { }
    void overflow(const T& x)
    {
        unsigned int n = (unsigned int)(_M_finish - _M_start), cap = n ? 2 * n : 1;
        T* block = (T*)std::malloc(cap * sizeof(T));
        for (unsigned int i = 0; i < n; ++i) std::memcpy(&block[i], &_M_start[i], sizeof(T));
        std::memcpy(&block[n], &x, sizeof(T));
        std::free(_M_start);
        _M_start = block; _M_finish = block + n + 1; _M_end = block + cap;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end) { std::memcpy(_M_finish, &x, sizeof(T)); ++_M_finish; }
        else overflow(x);
    }
};
struct CMainGameComponent { int id; };
struct CGameDefinitionManager { int id; };
struct CPlayer;
struct CPlayerManager {
    CMainGameComponent* Component;
    const CGameDefinitionManager* DefinitionManager;
    Vector<CPlayer*> Players;
    Vector<CCharString> HeroSwapPlayerScriptNames;
    CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& defs)
        : Component(&component), DefinitionManager(&defs)
    {
        { CCharString name("hero_swap_1.tng"); HeroSwapPlayerScriptNames.push_back(name); }
        { CCharString name("hero_swap_2.tng"); HeroSwapPlayerScriptNames.push_back(name); }
        { CCharString name("hero_swap_3.tng"); HeroSwapPlayerScriptNames.push_back(name); }
        { CCharString name("hero_swap_4.tng"); HeroSwapPlayerScriptNames.push_back(name); }
    }
};

int main()
{
    CMainGameComponent component; CGameDefinitionManager defs;
    CPlayerManager manager(component, defs);
    const char* expect[4] = { "hero_swap_1.tng", "hero_swap_2.tng", "hero_swap_3.tng", "hero_swap_4.tng" };
    bool ok = manager.Component == &component && manager.DefinitionManager == &defs
        && manager.Players._M_start == 0 && manager.Players._M_finish == 0
        && manager.HeroSwapPlayerScriptNames._M_finish - manager.HeroSwapPlayerScriptNames._M_start == 4;
    for (int i = 0; ok && i < 4; ++i)
        ok = std::strcmp(manager.HeroSwapPlayerScriptNames._M_start[i].text, expect[i]) == 0;
    std::free(manager.HeroSwapPlayerScriptNames._M_start);
    if (!ok) { std::printf("0044a3b0_TEST FAIL\n"); return 1; }
    std::printf("0044a3b0_TEST PASS\n");
    return 0;
}