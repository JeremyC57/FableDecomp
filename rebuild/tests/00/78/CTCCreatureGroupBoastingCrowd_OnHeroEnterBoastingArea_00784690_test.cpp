// behaviour test for 0x00784690
#include <stdio.h>
template <class T> class CList {                         // STLport list: one node pointer
public:
    struct CNode { CNode* _M_next; CNode* _M_prev; T _M_data; };
    CNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CThing;
struct CIntelligentThingPointer { void* _pad00; void* PData; };   // CIntelligentPointer<CThing> (8 bytes)
class CQuestManager {
public:
    unsigned char          _pad00[0x68];
    CList<CIntelligentThingPointer> ActiveQuestCards;             // +0x68
};
CQuestManager* g_pQuestManager;
enum EBoastingCrowdState { BOASTING_CROWD_STATE_NONE };
class CTCCreatureGroupBoastingCrowd {
public:
    unsigned char       _pad00[0x30];
    EBoastingCrowdState CrowdState;                      // +0x30
    void OnHeroEnterBoastingArea();
    int entered; EBoastingCrowdState last;
    void EnterCrowdState(EBoastingCrowdState state) { ++entered; last = state; }
};
void CTCCreatureGroupBoastingCrowd::OnHeroEnterBoastingArea()
{
    if (!g_pQuestManager->ActiveQuestCards.empty())
        EnterCrowdState(CrowdState);
}
int main()
{
    static CQuestManager qm; g_pQuestManager = &qm;
    CList<CIntelligentThingPointer>::CNode head, n1; head._M_next = head._M_prev = &head; qm.ActiveQuestCards._M_node = &head;
    CTCCreatureGroupBoastingCrowd c; c.entered = 0; c.CrowdState = (EBoastingCrowdState)3; int ok = 1;
    c.OnHeroEnterBoastingArea(); ok &= c.entered == 0;
    head._M_next = &n1; n1._M_next = &head;
    c.OnHeroEnterBoastingArea(); ok &= c.entered == 1 && c.last == 3;
    printf(ok ? "00784690_TEST PASS\n" : "00784690_TEST FAIL\n");
    return ok ? 0 : 1;
}