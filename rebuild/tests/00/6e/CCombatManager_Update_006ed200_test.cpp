// Behaviour test for 0x006ED200 CCombatManager::Update: every combatant of the (STLport red-black) Combatants tree is visited in
// key order through the inline _M_increment walk and passed with force = true, then the dialogue manager updates once.
#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { if (g_n < 32) g_log[g_n++] = v; }
struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct CCombatantInfo { int Id; };
struct Node : public NodeBase { unsigned long Key[2]; CCombatantInfo Info; };
static NodeBase* Increment(NodeBase* n)
{
    if (n->Right != 0) { n = n->Right; while (n->Left != 0) n = n->Left; }
    else { NodeBase* y = n->Parent; while (n == y->Right) { n = y; y = y->Parent; } if (n->Right != y) n = y; }
    return n;
}
class CombatDialogueManager { public: void Update() { Log(999); } };
class CCombatManager { public:
    NodeBase* Header; unsigned Count; CombatDialogueManager DialogueManager;
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force) { Log(info.Id * (force ? 1 : -1)); }
    void Update()
    {
        for (NodeBase* it = Header->Left; it != Header; it = Increment(it))
            UpdateAttackingCombatantsList(((Node*)it)->Info, true);
        DialogueManager.Update();
    }
};
int main()
{
    // tree: root 20, left 10 (right child 15), right 30
    NodeBase header; Node n10, n15, n20, n30;
    n20.Parent = &header; n20.Left = &n10; n20.Right = &n30;
    n10.Parent = &n20; n10.Left = 0; n10.Right = &n15;
    n15.Parent = &n10; n15.Left = 0; n15.Right = 0;
    n30.Parent = &n20; n30.Left = 0; n30.Right = 0;
    header.Color = false; header.Parent = &n20; header.Left = &n10; header.Right = &n30;
    n10.Info.Id = 10; n15.Info.Id = 15; n20.Info.Id = 20; n30.Info.Id = 30;
    CCombatManager m; m.Header = &header; m.Count = 4;
    m.Update();
    static const int expect[] = { 10, 15, 20, 30, 999 };
    if (g_n != 5) { std::printf("006ed200_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (g_log[i] != expect[i]) { std::printf("006ed200_TEST FAIL\n"); return 1; }
    NodeBase empty; empty.Parent = 0; empty.Left = &empty; empty.Right = &empty; m.Header = &empty; g_n = 0;
    m.Update();
    if (g_n != 1 || g_log[0] != 999) { std::printf("006ed200_TEST FAIL\n"); return 1; }
    std::printf("006ed200_TEST PASS\n");
    return 0;
}