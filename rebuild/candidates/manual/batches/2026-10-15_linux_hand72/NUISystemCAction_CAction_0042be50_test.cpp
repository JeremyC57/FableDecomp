#include <cstdio>
// Behaviour model of 0x0042BE50: NUISystem::CAction() -- zero type, two empty lists, Valid.
static int g_listsBuilt = 0;
struct SRetailAllocator { SRetailAllocator() {} };
struct SNode { SNode* next; SNode* prev; };
struct CRetailList {
    SNode* m_pNode;
    CRetailList(const SRetailAllocator& = SRetailAllocator()) : m_pNode(new SNode) { m_pNode->next = m_pNode; m_pNode->prev = m_pNode; ++g_listsBuilt; }
    ~CRetailList() { delete m_pNode; }
    bool empty() const { return m_pNode->next == m_pNode; }
};
enum EActionType { ACTION_NONE = 0, ACTION_OTHER = 7 };
struct CAction {
    EActionType ActionType; CRetailList UIActionParams; CRetailList ThingActionParams; bool Valid;
    CAction() : ActionType(ACTION_NONE), UIActionParams(), ThingActionParams(), Valid(true) {}
};
int main()
{
    CAction a;
    if (a.ActionType != ACTION_NONE || !a.Valid || g_listsBuilt != 2 || !a.UIActionParams.empty() || !a.ThingActionParams.empty())
        { std::printf("0042be50_TEST FAIL\n"); return 1; }
    std::printf("0042be50_TEST PASS\n");
    return 0;
}
