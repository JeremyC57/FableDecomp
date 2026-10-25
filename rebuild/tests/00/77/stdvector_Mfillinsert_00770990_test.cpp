// Behaviour test for 0x00770990 std::vector<list<CWeaponTrailEntry> >::_M_fill_insert (STLport fill insert).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_nodes = 0;

struct Entry { float d[7]; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Entry data; };

struct List {
    Node* node;
    List() { node = (Node*)std::malloc(sizeof(Node)); ++g_nodes; node->next = node; node->prev = node; }
    List(const List& x)
    {
        node = (Node*)std::malloc(sizeof(Node)); ++g_nodes; node->next = node; node->prev = node;
        for (NodeBase* p = x.node->next; p != x.node; p = p->next) push_back(((Node*)p)->data);
    }
    ~List() { clear(); std::free(node); --g_nodes; }
    void clear()
    {
        NodeBase* cur = node->next;
        while (cur != node) { NodeBase* t = cur; cur = cur->next; std::free(t); --g_nodes; }
        node->next = node; node->prev = node;
    }
    List& operator=(const List& x)
    {
        if (this != &x) { clear(); for (NodeBase* p = x.node->next; p != x.node; p = p->next) push_back(((Node*)p)->data); }
        return *this;
    }
    void push_back(const Entry& e)
    {
        Node* t = (Node*)std::malloc(sizeof(Node)); ++g_nodes;
        t->data = e; t->next = node; t->prev = node->prev; node->prev->next = t; node->prev = t;
    }
    int count() const { int c = 0; for (NodeBase* p = node->next; p != node; p = p->next) ++c; return c; }
};

struct Vec {
    List* m_start; List* m_finish; List* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(List* pos, const List& x, unsigned int fill_len)       // 0x00770810 (atend = false)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        List* ns = (List*)std::malloc(len * sizeof(List));
        List* nf = ns;
        for (List* p = m_start; p != pos; ++p, ++nf) new (nf) List(*p);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) List(x);
        for (List* p = pos; p != m_finish; ++p, ++nf) new (nf) List(*p);
        for (List* d = m_start; d != m_finish; ++d) d->~List();
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
    void fill_insert(List* pos, unsigned int n, const List& x)
    {
        if (n != 0) {
            if ((unsigned int)(m_end - m_finish) >= n) {
                List x_copy(x);
                const unsigned int elems_after = (unsigned int)(m_finish - pos);
                List* old_finish = m_finish;
                if (elems_after > n) {
                    List* r = m_finish;
                    for (List* p = m_finish - n; p != m_finish; ++p, ++r) new (r) List(*p);
                    m_finish += n;
                    List* res = old_finish; List* last = old_finish - n;
                    for (int k = (int)(last - pos); k > 0; --k) *--res = *--last;
                    for (List* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    List* f = m_finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++f) new (f) List(x_copy);
                    m_finish += n - elems_after;
                    List* r = m_finish;
                    for (List* p = pos; p != old_finish; ++p, ++r) new (r) List(*p);
                    m_finish += elems_after;
                    for (List* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            } else
                insert_overflow(pos, x, n);
        }
    }
};

int main()
{
    int ok = 1;
    {
        Entry e; for (int i = 0; i < 7; ++i) e.d[i] = (float)i;
        List one, two; one.push_back(e); two.push_back(e); two.push_back(e);
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        v.fill_insert(v.m_finish, 0, one);                                  // no-op
        if (v.m_start != 0) ok = 0;
        v.fill_insert(v.m_finish, 2, one);                                  // grow: len 0 + 2
        if (v.size() != 2 || v.m_end - v.m_start != 2) ok = 0;
        v.fill_insert(v.m_finish, 2, two);                                  // grow: len 2 + 2
        if (v.size() != 4 || v.m_end - v.m_start != 4 || v.m_start[3].count() != 2) ok = 0;
        // make room: drop the last two (in-place paths need spare capacity)
        v.m_finish[-1].~List(); v.m_finish[-2].~List(); v.m_finish -= 2;
        v.fill_insert(v.m_start, 1, two);                                   // elems_after (2) > n (1)
        if (v.size() != 3 || v.m_start[0].count() != 2 || v.m_start[1].count() != 1 || v.m_start[2].count() != 1) ok = 0;
        v.m_finish[-1].~List(); v.m_finish -= 1;                            // [two, one], capacity 4
        v.fill_insert(v.m_start + 1, 2, two);                               // elems_after (1) <= n (2)
        if (v.size() != 4 || v.m_start[0].count() != 2 || v.m_start[1].count() != 2 || v.m_start[2].count() != 2 ||
            v.m_start[3].count() != 1) ok = 0;
        for (List* d = v.m_start; d != v.m_finish; ++d) d->~List();
        std::free(v.m_start);
    }
    if (g_nodes != 0) ok = 0;
    if (!ok) { std::printf("00770990_TEST FAIL\n"); return 1; }
    std::printf("00770990_TEST PASS\n");
    return 0;
}