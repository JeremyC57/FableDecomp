// Behaviour test for 0x00770810 std::vector<list<CWeaponTrailEntry> >::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_nodes = 0;

struct Entry { float d[7]; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Entry data; };

struct List {                                   // 4-byte STLport list: the sentinel pointer
    Node* node;
    List() { node = (Node*)std::malloc(sizeof(Node)); ++g_nodes; node->next = node; node->prev = node; }
    List(const List& x) : node(0)
    {
        Node* n = (Node*)std::malloc(sizeof(Node)); ++g_nodes;
        n->next = n; n->prev = n; node = n;
        for (NodeBase* p = x.node->next; p != x.node; p = p->next) push_back(((Node*)p)->data);
    }
    ~List()
    {
        NodeBase* cur = node->next;
        while (cur != node) { NodeBase* t = cur; cur = cur->next; std::free(t); --g_nodes; }
        std::free(node); --g_nodes;
    }
    void push_back(const Entry& e)
    {
        Node* t = (Node*)std::malloc(sizeof(Node)); ++g_nodes;
        t->data = e; t->next = node; t->prev = node->prev; node->prev->next = t; node->prev = t;
    }
    int count() const { int c = 0; for (NodeBase* p = node->next; p != node; p = p->next) ++c; return c; }
};

struct __false_type { __false_type() { } };

struct Vec {
    List* m_start; List* m_finish; List* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(List* pos, const List& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        List* ns = len != 0 ? (List*)std::malloc(len * sizeof(List)) : 0;
        List* nf = ns;
        for (List* p = m_start; p != pos; ++p, ++nf) new (nf) List(*p);
        if (fill_len == 1) { new (nf) List(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) List(x);
        if (!atend) for (List* p = pos; p != m_finish; ++p, ++nf) new (nf) List(*p);
        for (List* d = m_start; d != m_finish; ++d) d->~List();
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

int main()
{
    int ok = 1;
    {
        Entry e; for (int i = 0; i < 7; ++i) e.d[i] = (float)i;
        List a, b; a.push_back(e); a.push_back(e); b.push_back(e);
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        v.insert_overflow(v.m_finish, a, __false_type(), 1, true);
        v.insert_overflow(v.m_start, b, __false_type(), 2, false);       // len = 1 + 2 = 3
        if (v.size() != 3 || v.m_end - v.m_start != 3) ok = 0;
        if (v.m_start[0].count() != 1 || v.m_start[1].count() != 1 || v.m_start[2].count() != 2) ok = 0;
        if (((Node*)v.m_start[2].node->next)->data.d[6] != 6.0f) ok = 0;
        if (g_nodes != 2 + 3 + 3 + 4) ok = 0;
        for (List* d = v.m_start; d != v.m_finish; ++d) d->~List();
        std::free(v.m_start);
    }
    if (g_nodes != 0) ok = 0;
    if (!ok) { std::printf("00770810_TEST FAIL\n"); return 1; }
    std::printf("00770810_TEST PASS\n");
    return 0;
}