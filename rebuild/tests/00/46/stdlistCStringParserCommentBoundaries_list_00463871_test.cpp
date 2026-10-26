// Behaviour test for 0x00463871 list<CStringParserCommentBoundaries>::list(const list&): a fresh self-linked sentinel, then a
// copy of every element of the source in order (the source untouched)
#include <cstdio>
#include <cstdlib>
class CStringParserCommentBoundaries { public: char* PStart; char* PEnd; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CStringParserCommentBoundaries data; };
struct ListBase {
    NodeBase* node;
    ListBase() { node = (NodeBase*)std::malloc(sizeof(Node)); node->next = node; node->prev = node; }
};
struct List : ListBase {
    List() { }
    void InsertRange(NodeBase* pos, NodeBase* first, NodeBase* last)
    {
        for (; first != last; first = first->next) {
            Node* tmp = (Node*)std::malloc(sizeof(Node)); tmp->data = ((Node*)first)->data;
            NodeBase* p = pos->prev; tmp->next = pos; tmp->prev = p; p->next = tmp; pos->prev = tmp;
        }
    }
    List(const List& x) : ListBase() { InsertRange(node->next, x.node->next, x.node); }
};
static char g_text[8];
int main()
{
    List a;
    for (int i = 0; i < 3; ++i) { Node* t = (Node*)std::malloc(sizeof(Node)); t->data.PStart = g_text + i; t->data.PEnd = g_text + 2 * i;
        NodeBase* p = a.node->prev; t->next = a.node; t->prev = p; p->next = t; a.node->prev = t; }
    List b(a);
    List empty;
    List c(empty);
    bool ok = b.node != a.node && c.node->next == c.node && c.node->prev == c.node;
    NodeBase* n = b.node->next; NodeBase* m = a.node->next;
    for (int i = 0; i < 3; ++i, n = n->next, m = m->next)
        ok = ok && n != m && ((Node*)n)->data.PStart == g_text + i && ((Node*)n)->data.PEnd == g_text + 2 * i;
    ok = ok && n == b.node && m == a.node;
    if (!ok) { std::printf("00463871_TEST FAIL\n"); return 1; }
    std::printf("00463871_TEST PASS\n");
    return 0;
}