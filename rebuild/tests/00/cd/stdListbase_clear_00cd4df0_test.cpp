// behaviour test for 0x00CD4DF0 std::_List_base<CScriptThing >::clear(): every element is destroyed exactly once through its virtual destructor in list order, every node is freed, and the sentinel is left linked to itself; clearing an empty list only re-links the sentinel
#include <cstdio>
#include <cstdlib>
#include <new>
static int gDestroyed = 0;
static int gOrder[8];
class CElem {                                    // stands for CScriptThing: a polymorphic element destroyed in place
public:
    int Id;
    CElem(int id) : Id(id) { }
    virtual ~CElem() { if (gDestroyed < 8) gOrder[gDestroyed] = Id; ++gDestroyed; }
};
struct Node { Node* next; Node* prev; CElem data; Node(int id) : data(id) { } };
struct List { Node* node; };
static Node* NewNode(int id) { return new (std::malloc(sizeof(Node))) Node(id); }
static void Clear(List& l)                       // _List_base::clear()
{
    Node* cur = l.node->next;
    while (cur != l.node) {
        Node* tmp = cur;
        cur = cur->next;
        tmp->data.~CElem();
        std::free(tmp);
    }
    l.node->next = l.node;
    l.node->prev = l.node;
}
static void PushBack(List& l, Node* n) { n->next = l.node; n->prev = l.node->prev; l.node->prev->next = n; l.node->prev = n; }
int main()
{
    int fails = 0;
    List l;
    l.node = NewNode(-1);
    l.node->next = l.node; l.node->prev = l.node;
    for (int i = 0; i < 5; ++i) PushBack(l, NewNode(i));
    Clear(l);
    if (gDestroyed != 5) fails |= 1;
    for (int k = 0; k < 5; ++k) if (gOrder[k] != k) fails |= 2;
    if (l.node->next != l.node || l.node->prev != l.node) fails |= 4;
    Clear(l);
    if (gDestroyed != 5 || l.node->next != l.node || l.node->prev != l.node) fails |= 8;
    PushBack(l, NewNode(7));
    Clear(l);
    if (gDestroyed != 6 || gOrder[5] != 7 || l.node->next != l.node) fails |= 16;
    if (l.node->data.Id != -1) fails |= 32;
    std::free(l.node);
    if (fails) { std::printf("00cd4df0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00cd4df0_TEST PASS\n");
    return 0;
}