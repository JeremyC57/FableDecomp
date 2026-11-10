// Behaviour test for 0x0041D207 NUISystem::CManager::ClearActionParams: both action-parameter lists end empty, UI list first.
#include <stdio.h>
#include <list>
namespace NUISystem {
class CActionParam { public: int Kind; };
class CManager { public:
    std::list<long> ThingActionParams;
    std::list<CActionParam*> UIActionParams;
    int Order[2]; int N;
    void ClearUI() { UIActionParams.clear(); Order[N++] = 1; }
    void ClearThing() { ThingActionParams.clear(); Order[N++] = 2; }
    void ClearActionParams() { ClearUI(); ClearThing(); }   // retail: +0x60 clear, then tail-jump +0x5C clear
};
}
int main()
{
    NUISystem::CManager m; m.N = 0;
    NUISystem::CActionParam p;
    m.ThingActionParams.push_back(3); m.ThingActionParams.push_back(7);
    m.UIActionParams.push_back(&p);
    m.ClearActionParams();
    bool ok = m.ThingActionParams.empty() && m.UIActionParams.empty() && m.N == 2 && m.Order[0] == 1 && m.Order[1] == 2;
    m.ClearActionParams();                                       // clearing empty lists is harmless
    ok = ok && m.ThingActionParams.empty() && m.UIActionParams.empty();
    printf(ok ? "0041d207_TEST PASS\n" : "0041d207_TEST FAIL\n");
    return ok ? 0 : 1;
}
