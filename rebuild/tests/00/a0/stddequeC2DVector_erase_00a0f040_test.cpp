// Behaviour test for 0x00A0F040 deque<C2DVector>::erase(first, last) (STLport range erase over 16-element nodes).
#include <cstdio>

struct V2 { float X, Y; };
enum { BUF = 16, NODES = 8 };
static V2 g_store[NODES][BUF];
static int g_freed[NODES];
static int g_cleared = 0;

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
    int operator-(const Iter& x) const { return BUF * (int)(node - x.node - 1) + (int)(cur - first) + (int)(x.last - x.cur); }
    void inc() { if (++cur == last) { set_node(node + 1); cur = first; } }
    void dec() { if (cur == first) { set_node(node - 1); cur = last; } --cur; }
};

static Iter Copy(Iter first, Iter last, Iter result)
{
    for (int n = last - first; n > 0; --n) { *result.cur = *first.cur; first.inc(); result.inc(); }
    return result;
}
static Iter CopyBackward(Iter first, Iter last, Iter result)
{
    for (int n = last - first; n > 0; --n) { last.dec(); result.dec(); *result.cur = *last.cur; }
    return result;
}

struct Deque {
    Iter start, finish; V2* map[NODES];
    int size() const { return finish - start; }
    void clear() { ++g_cleared; finish = start; }
    void destroy_nodes(V2** nstart, V2** nfinish) { for (V2** n = nstart; n < nfinish; ++n) g_freed[n - map] = 1; }
    Iter erase(Iter first, Iter last)
    {
        if (first.cur == start.cur && last.cur == finish.cur) {
            clear();
            return finish;
        } else {
            int n = last - first;
            int elems_before = first - start;
            if (elems_before < (size() - n) / 2) {
                CopyBackward(start, first, last);
                Iter new_start = start + n;
                destroy_nodes(start.node, new_start.node);
                start = new_start;
            } else {
                Copy(last, finish, first);
                Iter new_finish = finish - n;
                destroy_nodes(new_finish.node + 1, finish.node + 1);
                finish = new_finish;
            }
            return start + elems_before;
        }
    }
};

static void Fill(Deque& d)
{
    for (int i = 0; i < NODES; ++i) { d.map[i] = g_store[i]; g_freed[i] = 0; }
    d.start.set_node(d.map + 1); d.start.cur = d.start.first + 10;   // 40 elements: node 1 slot 10 .. node 4 slot 2
    d.finish = d.start + 40;
    Iter it = d.start;
    for (int k = 0; k < 40; ++k, it.inc()) { it.cur->X = (float)k; it.cur->Y = 0.0f; }
}

int main()
{
    int ok = 1;
    Deque d;
    Fill(d);                                               // erase [3, 8): front side is shorter -> copy_backward
    Iter r = d.erase(d.start + 3, d.start + 8);
    if (d.size() != 35 || r.cur->X != 8.0f || (d.start + 2).cur->X != 2.0f || d.start.cur->X != 0.0f) ok = 0;
    if (d.start.node != d.map + 1 || d.start.cur != d.start.first + 15) ok = 0;
    Fill(d);                                               // erase [30, 38): back side is shorter -> copy
    r = d.erase(d.start + 30, d.start + 38);
    if (d.size() != 32 || r.cur->X != 38.0f || (d.start + 31).cur->X != 39.0f || !g_freed[4] || g_freed[3]) ok = 0;
    Fill(d);                                               // whole range -> clear()
    r = d.erase(d.start, d.finish);
    if (g_cleared != 1 || d.size() != 0 || r.cur != d.finish.cur) ok = 0;
    if (!ok) { std::printf("00a0f040_TEST FAIL\n"); return 1; }
    std::printf("00a0f040_TEST PASS\n");
    return 0;
}