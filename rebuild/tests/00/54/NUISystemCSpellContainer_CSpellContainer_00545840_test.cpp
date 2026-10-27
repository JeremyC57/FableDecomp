// Behaviour test for 00545840 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
static int g_Removed = 0, g_BaseDtor = 0, g_SpellReleases = 0; static void* g_Last = 0;
struct CObserver { virtual ~CObserver() { } int Pad; };
struct CManager { void RemoveObserver(CObserver* o) { ++g_Removed; g_Last = o; } static CManager* GetInstance() { static CManager m; return &m; } };
struct CUIInterface { virtual ~CUIInterface() { } };
struct CChangingStateComponent : CUIInterface, CObserver { virtual ~CChangingStateComponent() { ++g_BaseDtor; } };
struct SpellPtr { ~SpellPtr() { ++g_SpellReleases; } };
struct SpellVec { SpellPtr* Start; SpellPtr* Finish; SpellVec() { Start = new SpellPtr[2]; Finish = Start + 2; } ~SpellVec() { delete[] Start; } };
struct CSpellContainer : CChangingStateComponent { SpellVec BaseSpells;
    virtual ~CSpellContainer() { CManager::GetInstance()->RemoveObserver(this); } };
int main() {
    CSpellContainer* s = new CSpellContainer; CObserver* obs = s;
    CUIInterface* p = s; delete p;
    if (g_Removed != 1 || g_Last != (void*)obs || g_BaseDtor != 1 || g_SpellReleases != 2) { std::printf("00545840_TEST FAIL\n"); return 1; }
    { std::printf("00545840_TEST PASS\n"); return 0; }
}