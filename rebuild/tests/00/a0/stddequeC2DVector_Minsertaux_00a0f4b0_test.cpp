// Behaviour test for 00a0f4b0: std::deque<C2DVector>::_M_insert_aux(pos, n, x) (STLport 4.6 _deque.c) -- inserting n
// copies in the middle shifts the shorter side (front or back) over chunk boundaries; checked against a flat array model
#include <cstdio>
#include <cstring>
#include <vector>
class C2DVector { public: float x, y; };
enum { BUF = 4, MAPINIT = 64 };
struct Iter {
    C2DVector* cur; C2DVector* first; C2DVector* last; C2DVector** node;
    void set(C2DVector** n) { node = n; first = *n; last = first + BUF; }
    Iter& operator++() { if (++cur == last) { set(node + 1); cur = first; } return *this; }
    Iter& operator--() { if (cur == first) { set(node - 1); cur = last; } --cur; return *this; }
    Iter& operator+=(long n) { long off = n + (cur - first); if (off >= 0 && off < BUF) cur += n; else { long nn = off > 0 ? off / BUF : -((-off - 1) / BUF) - 1; set(node + nn); cur = first + (off - nn * BUF); } return *this; }
    Iter& operator-=(long n) { return *this += -n; }
    Iter operator+(long n) const { Iter t = *this; return t += n; }
    Iter operator-(long n) const { Iter t = *this; return t -= n; }
    long operator-(const Iter& o) const { return BUF * (node - o.node - 1) + (cur - first) + (o.last - o.cur); }
    C2DVector& operator*() const { return *cur; }
    bool operator==(const Iter& o) const { return cur == o.cur; } bool operator!=(const Iter& o) const { return cur != o.cur; }
};
static Iter ucopy(Iter f, Iter l, Iter r) { for (; f != l; ++f, ++r) *r = *f; return r; }
static Iter copy(Iter f, Iter l, Iter r) { return ucopy(f, l, r); }
static Iter copy_backward(Iter f, Iter l, Iter r) { while (f != l) { --l; --r; *r = *l; } return r; }
static void fill(Iter f, Iter l, const C2DVector& x) { for (; f != l; ++f) *f = x; }
static void ucopy_fill(Iter f1, Iter l1, Iter f2, Iter l2, const C2DVector& x) { Iter m = ucopy(f1, l1, f2); fill(m, l2, x); }
static Iter ufill_copy(Iter r, Iter m, Iter f, Iter l, const C2DVector& x) { fill(r, m, x); return ucopy(f, l, m); }
struct Deque {
    Iter start, finish; C2DVector** map; long map_size;
    Deque() { map_size = MAPINIT; map = new C2DVector*[map_size]; C2DVector** n = map + map_size / 2; *n = new C2DVector[BUF]; start.set(n); start.cur = start.first; finish = start; }
    long size() const { return finish - start; }
    void reallocate_map(long, bool) { std::printf("map reallocation not modelled\n"); }
    void new_elements_at_front(long n) { long nodes = (n + BUF - 1) / BUF; if ((long)(start.node - map) < nodes) reallocate_map(nodes, true); for (long i = 1; i <= nodes; ++i) *(start.node - i) = new C2DVector[BUF]; }
    void new_elements_at_back(long n) { long nodes = (n + BUF - 1) / BUF; if (map_size - (finish.node - map) - 1 < nodes) reallocate_map(nodes, false); for (long i = 1; i <= nodes; ++i) *(finish.node + i) = new C2DVector[BUF]; }
    Iter reserve_front(long n) { long v = start.cur - start.first; if (n > v) new_elements_at_front(n - v); return start - n; }
    Iter reserve_back(long n) { long v = (finish.last - finish.cur) - 1; if (n > v) new_elements_at_back(n - v); return finish + n; }
    void insert_aux(Iter pos, long n, const C2DVector& x)   // STLport 4.6 deque::_M_insert_aux(iterator, size_type, const value_type&)
    {
        const long elems_before = pos - start; long length = size(); C2DVector x_copy = x;
        if (elems_before < length / 2) {
            Iter new_start = reserve_front(n); Iter old_start = start; pos = start + elems_before;
            if (elems_before >= n) { Iter start_n = start + n; ucopy(start, start_n, new_start); start = new_start; copy(start_n, pos, old_start); fill(pos - n, pos, x_copy); }
            else { ucopy_fill(start, pos, new_start, start, x_copy); start = new_start; fill(old_start, pos, x_copy); }
        } else {
            Iter new_finish = reserve_back(n); Iter old_finish = finish; const long elems_after = length - elems_before; pos = finish - elems_after;
            if (elems_after > n) { Iter finish_n = finish - n; ucopy(finish_n, finish, finish); finish = new_finish; copy_backward(pos, finish_n, old_finish); fill(pos, pos + n, x_copy); }
            else { ufill_copy(finish, pos + n, pos, finish, x_copy); finish = new_finish; fill(pos, old_finish, x_copy); }
        }
    }
    void push_back(const C2DVector& v) { Iter nf = reserve_back(1); *finish = v; finish = nf; }
};
static bool check(Deque& d, const std::vector<C2DVector>& model) {
    if (d.size() != (long)model.size()) return false;
    Iter it = d.start; for (size_t i = 0; i < model.size(); ++i, ++it) if ((*it).x != model[i].x || (*it).y != model[i].y) return false;
    return true;
}
int main()
{
    Deque d; std::vector<C2DVector> m;
    for (int i = 0; i < 10; ++i) { C2DVector v = { (float)i, (float)-i }; d.push_back(v); m.push_back(v); }
    C2DVector x = { 99.f, 1.f };
    d.insert_aux(d.start + 2, 5, x); m.insert(m.begin() + 2, 5, x);      // near the front: the front shifts by five
    bool ok = check(d, m);
    d.insert_aux(d.start + 12, 3, x); m.insert(m.begin() + 12, 3, x);    // near the back: the back shifts by three
    ok = ok && check(d, m);
    d.insert_aux(d.start + 1, 1, x); m.insert(m.begin() + 1, 1, x);      // fewer elements before than inserted
    ok = ok && check(d, m);
    d.insert_aux(d.finish - 1, 6, x); m.insert(m.end() - 1, 6, x);       // fewer elements after than inserted
    ok = ok && check(d, m) && d.size() == 25;
    if (!ok) std::printf("size %ld\n", d.size());
    std::printf(ok ? "00a0f4b0_TEST PASS\n" : "00a0f4b0_TEST FAIL\n");
    return ok ? 0 : 1;
}