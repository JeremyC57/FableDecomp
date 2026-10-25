// Behaviour test for 0x00A0FAA0 deque<C2DVector>::_M_fill_insert (front / back / middle dispatch of an n-copy insert).
#include <cstdio>

struct V2 { float X, Y; };
enum { BUF = 16, NODES = 8 };
static V2 g_store[NODES][BUF];
static int g_aux = 0;

struct Iter {
    V2* cur; V2* first; V2* last; V2** node;
    void set_node(V2** n) { node = n; first = *n; last = first + BUF; }
    void advance(int n)
    {
        int offset = n + (int)(cur - first);
        if (offset >= 0 && offset < BUF) cur += n;
        else {
            int node_offset = offset > 0 ? offset / BUF : -((-offset - 1) / BUF) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * BUF);
        }
    }
    Iter operator+(int n) const { Iter t = *this; t.advance(n); return t; }
    Iter operator-(int n) const { Iter t = *this; t.advance(-n); return t; }
    void inc() { if (++cur == last) { set_node(node + 1); cur = first; } }
};

static void UninitFill(Iter first, Iter last, const V2& x) { for (; first.cur != last.cur; first.inc()) *first.cur = x; }

struct Deque {
    Iter start, finish; V2* map[NODES];
    Iter reserve_front(unsigned int n) { return start - (int)n; }    // all nodes pre-allocated in the test
    Iter reserve_back(unsigned int n) { return finish + (int)n; }
    void insert_aux(Iter, unsigned int, const V2&) { ++g_aux; }
    void fill_insert(Iter pos, unsigned int n, const V2& x)
    {
        if (pos.cur == start.cur) {
            Iter new_start = reserve_front(n);
            UninitFill(new_start, start, x);
            start = new_start;
        } else if (pos.cur == finish.cur) {
            Iter new_finish = reserve_back(n);
            UninitFill(finish, new_finish, x);
            finish = new_finish;
        } else
            insert_aux(pos, n, x);
    }
    int size() const { return BUF * (int)(finish.node - start.node - 1) + (int)(finish.cur - finish.first) + (int)(start.last - start.cur); }
};

int main()
{
    int ok = 1;
    Deque d;
    for (int i = 0; i < NODES; ++i) d.map[i] = g_store[i];
    d.start.set_node(d.map + 3); d.start.cur = d.start.first + 2;
    d.start.cur->X = 7.0f; d.start.cur->Y = 8.0f;     // one element at node 3 slot 2
    d.finish = d.start + 1;
    V2 a = { 1.0f, 2.0f }, b = { 3.0f, 4.0f };
    d.fill_insert(d.finish, 20, a);                       // back: crosses into node 4
    if (d.size() != 21 || d.finish.node != d.map + 4 || d.finish.cur != d.finish.first + 7) ok = 0;
    d.fill_insert(d.start, 5, b);                         // front: crosses back into node 2
    if (d.size() != 26 || d.start.node != d.map + 2 || d.start.cur != d.start.first + 13) ok = 0;
    if (d.start.cur->X != 3.0f || g_store[3][1].Y != 4.0f || g_store[3][2].X != 7.0f || g_store[3][3].X != 1.0f ||
        g_store[4][6].Y != 2.0f) ok = 0;
    Iter mid = d.start + 7;
    d.fill_insert(mid, 2, a);                             // middle: deferred to _M_insert_aux
    if (g_aux != 1 || d.size() != 26) ok = 0;
    if (!ok) { std::printf("00a0faa0_TEST FAIL\n"); return 1; }
    std::printf("00a0faa0_TEST PASS\n");
    return 0;
}