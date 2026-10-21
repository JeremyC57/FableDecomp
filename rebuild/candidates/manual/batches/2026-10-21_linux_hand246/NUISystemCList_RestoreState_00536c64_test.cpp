// Behaviour test for NUISystem::CList::RestoreState @ 0x00536C64 (copy of the model over a small component tree).
#include <cstdio>
static bool g_restoring; static int g_restoringSeen, g_updates, g_actions;
struct CComponent;
struct ChildPointer { CComponent* Data; void* CountInfo; };
struct ChildVector { ChildPointer* Start; ChildPointer* Finish; unsigned long size() const { return (unsigned long)(Finish - Start); } };
struct ComponentVector { CComponent** Start; CComponent** Finish; };
struct CComponent {
    long type, selected, state, pending; ChildVector children; ComponentVector ScrollingChildren;
    void Update(float) { ++g_updates; if (g_restoring) ++g_restoringSeen; }
    void ChangeState(long s) { state = s; pending = 2; }
    bool IsStateChangeFinished() { return pending-- <= 0; }
    const ChildVector& GetChildren() { return children; }
    long GetType() { return type; }
    void ExecuteActions(void* a) { if (a == (void*)this) ++g_actions; }
    void* GetActionsOnSelect() { return this; }
    void SetSelectedChild(long i) { selected = i; }
    const ComponentVector& GetScrollingChildren() { return ScrollingChildren; }
};
struct ListNode { ListNode* _M_next; ListNode* _M_prev; long _M_data; };
struct LongList { ListNode* _M_node; };
struct CList : CComponent {
    void RestoreState(const LongList& path)
    {
        g_restoring = true;
        CComponent* current = this;
        for (ListNode* node = path._M_node->_M_next; node != path._M_node; node = node->_M_next) {
            long index = node->_M_data;
            current->SetSelectedChild(index);
            current->ChangeState(5);
            while (!current->IsStateChangeFinished()) Update(5.0f);
            for (long frame = 10; frame; --frame) Update(1.0f);
            if (current->GetScrollingChildren().Start[index]->GetType() == 7) {
                CComponent* child = current->GetScrollingChildren().Start[index];
                child->ExecuteActions(child->GetActionsOnSelect());
            }
            CComponent* selected = current->ScrollingChildren.Start[index];
            unsigned long i = 0;
            for (; i != selected->GetChildren().size(); ++i) {
                CComponent* candidate = selected->GetChildren().Start[i].Data;
                if (candidate->GetType() == 8) { current = candidate; break; }
            }
            if (i == selected->GetChildren().size()) break;
        }
        g_restoring = false;
    }
};
static CComponent make(long type) { CComponent c; c.type = type; c.selected = -1; c.state = 0; c.pending = 0; c.children.Start = c.children.Finish = 0; c.ScrollingChildren.Start = c.ScrollingChildren.Finish = 0; return c; }
int main()
{
    int bad = 0;
    CComponent sub = make(8);                       // nested list reached through item 1
    CComponent leaf = make(7);
    CComponent* subItems[1] = { &leaf }; sub.ScrollingChildren.Start = subItems; sub.ScrollingChildren.Finish = subItems + 1;
    CComponent item0 = make(1), item1 = make(1);
    ChildPointer item1Kids[2] = { { &leaf, 0 }, { &sub, 0 } }; item1.children.Start = item1Kids; item1.children.Finish = item1Kids + 2;
    CList root; root.type = 8; root.selected = -1; root.state = 0; root.pending = 0; root.children.Start = root.children.Finish = 0;
    CComponent* rootItems[2] = { &item0, &item1 }; root.ScrollingChildren.Start = rootItems; root.ScrollingChildren.Finish = rootItems + 2;
    ListNode head, n1, n2, n3; head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data = 1; n2._M_data = 0; n3._M_data = 0;
    LongList path; path._M_node = &head;
    root.RestoreState(path);
    if (root.selected != 1 || root.state != 5 || sub.selected != 0 || sub.state != 5 || g_actions != 1 || g_restoring) bad++;
    if (g_updates != 2 * (2 + 10) || g_restoringSeen != g_updates) bad++;   // two levels visited, then leaf has no list child
    if (bad) { std::printf("00536c64_TEST FAIL\n"); return 1; }
    std::printf("00536c64_TEST PASS\n");
    return 0;
}
