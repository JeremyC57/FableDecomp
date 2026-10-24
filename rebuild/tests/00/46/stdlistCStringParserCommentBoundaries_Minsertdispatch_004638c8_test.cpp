// Behaviour test for 0x004638C8 list<CStringParserCommentBoundaries>::_M_insert_dispatch(pos, first, last, __false_type):
// copies [first, last) in order into new nodes linked before pos
#include <cstdio>
#include <cstdlib>
struct CStringParserCommentBoundaries { char* PStart; char* PEnd; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CStringParserCommentBoundaries data; };
struct List {
    NodeBase* node;
    List() { node = (NodeBase*)std::malloc(sizeof(Node)); node->next = node->prev = node; }
    Node* CreateNode(const CStringParserCommentBoundaries& x) { Node* p = (Node*)std::malloc(sizeof(Node)); p->data = x; return p; }
    NodeBase* Insert(NodeBase* n, const CStringParserCommentBoundaries& x)
    {
        Node* tmp = CreateNode(x);
        NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
        return tmp;
    }
    void InsertDispatch(NodeBase* pos, NodeBase* first, NodeBase* last)
    {
        for (; first != last; first = first->next)
            Insert(pos, ((Node*)first)->data);
    }
};
static char g_text[8];
int main()
{
    List src, dst;
    for (int i = 0; i < 4; ++i) { CStringParserCommentBoundaries b = { g_text + i, g_text + i + 1 }; src.Insert(src.node, b); }
    CStringParserCommentBoundaries z = { g_text + 7, g_text + 7 };
    NodeBase* zn = dst.Insert(dst.node, z);                      // dst: [z]
    dst.InsertDispatch(zn, src.node->next->next, src.node);       // insert src[1..3] before z
    dst.InsertDispatch(dst.node, src.node, src.node);            // empty range: nothing
    bool ok = true;
    NodeBase* n = dst.node->next;
    for (int i = 1; i < 4; ++i, n = n->next) ok = ok && ((Node*)n)->data.PStart == g_text + i && ((Node*)n)->data.PEnd == g_text + i + 1;
    ok = ok && n == zn && zn->next == dst.node && dst.node->prev == zn;
    int back = 0; for (NodeBase* m = dst.node->prev; m != dst.node; m = m->prev) ++back;
    ok = ok && back == 4;
    if (!ok) { std::printf("004638c8_TEST FAIL\n"); return 1; }
    std::printf("004638c8_TEST PASS\n");
    return 0;
}