// Behaviour test for CPrisonWarden::OnPredicateFail (0x00DDE6F0): the entity forwards to its game interface's
// RemoveAllQuestInfoElements() (CGameScriptInterfaceBase slot 339) and does nothing else.
#include <cstdio>
class CGameScriptInterfaceBase { public:
    int removeAllCalls;
    CGameScriptInterfaceBase() : removeAllCalls(0) { }
    virtual ~CGameScriptInterfaceBase() { }
    virtual void RemoveAllQuestInfoElements() const { const_cast<CGameScriptInterfaceBase*>(this)->removeAllCalls++; }
};
class CEntityScriptBase { public:
    CGameScriptInterfaceBase* Game;
    virtual ~CEntityScriptBase() { }
    virtual void OnPredicateFail() { }
};
struct CPrisonWarden : CEntityScriptBase {
    void* ParentClass;
    virtual void OnPredicateFail() { Game->RemoveAllQuestInfoElements(); }
};
int main()
{
    CGameScriptInterfaceBase game;
    CPrisonWarden warden; warden.Game = &game; warden.ParentClass = 0;
    CEntityScriptBase* base = &warden;
    base->OnPredicateFail();
    bool ok = game.removeAllCalls == 1;
    base->OnPredicateFail();
    ok = ok && game.removeAllCalls == 2 && warden.ParentClass == 0;
    if (!ok) { std::printf("00dde6f0_TEST FAIL\n"); return 1; }
    std::printf("00dde6f0_TEST PASS\n");
    return 0;
}