// Behaviour test for CNavQuadTree::UnblockAllSwitchableNodes (0x00A78DD0): every switchable node gets
// SetAsBlocked(its own ID, false).
#include <cstdio>
struct CNavSwitchableLeafNode { unsigned long lastLo, lastHi; int calls; bool blocked; void SetAsBlocked(unsigned long lo, unsigned long hi, bool b) { lastLo = lo; lastHi = hi; ++calls; blocked = b; } };
struct SNode { unsigned long idLo, idHi; CNavSwitchableLeafNode* Node; };
struct CMap { SNode* m_pFirst; SNode* m_pLast; SNode* m_pEnd; unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); } SNode& operator[](unsigned int i) { return m_pFirst[i]; } };
struct CNavQuadTree {
    CMap SwitchableNodes;
    void UnblockAllSwitchableNodes()
    {
        for (unsigned int i = 0; i < SwitchableNodes.size(); ++i)
            SwitchableNodes[i].Node->SetAsBlocked(SwitchableNodes[i].idLo, SwitchableNodes[i].idHi, false);
    }
};
int main()
{
    CNavSwitchableLeafNode n[2]; for (int k = 0; k < 2; ++k) { n[k].calls = 0; n[k].blocked = true; n[k].lastLo = n[k].lastHi = 0; }
    SNode e[2]; e[0].idLo = 10; e[0].idHi = 1; e[0].Node = &n[0]; e[1].idLo = 20; e[1].idHi = 2; e[1].Node = &n[1];
    CNavQuadTree t; t.SwitchableNodes.m_pFirst = e; t.SwitchableNodes.m_pLast = e + 2; t.SwitchableNodes.m_pEnd = e + 2;
    t.UnblockAllSwitchableNodes();
    bool ok = n[0].calls == 1 && !n[0].blocked && n[0].lastLo == 10 && n[0].lastHi == 1 && n[1].calls == 1 && !n[1].blocked && n[1].lastLo == 20 && n[1].lastHi == 2;
    if (!ok) { std::printf("00a78dd0_TEST FAIL\n"); return 1; }
    std::printf("00a78dd0_TEST PASS\n");
    return 0;
}