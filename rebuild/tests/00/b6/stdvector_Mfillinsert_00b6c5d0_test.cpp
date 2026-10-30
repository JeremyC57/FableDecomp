// Behaviour test for 0x00B6C5D0 std::vector<CVertexShader>::_M_fill_insert(pos, n, x): n copies of x go in at pos,
// the tail shifts right (both in-place branches), every shared Data ends with the right reference count, and a
// too-small capacity goes to _M_insert_overflow (modelled here as a reallocating insert).
#include <cstdio>
#include <cstdlib>

static int g_deleted = 0;
struct Data { unsigned long ReferenceCount; int id; };
struct Shader {
    Data* D;
    Shader() : D(0) { }
    Shader(const Shader& o) : D(o.D) { if (D) ++D->ReferenceCount; }
    Shader& operator=(const Shader& r)
    {
        if (D && --D->ReferenceCount == 0) { ++g_deleted; std::free(D); }
        D = r.D; ++D->ReferenceCount; return *this;
    }
    ~Shader() { if (D && --D->ReferenceCount == 0) { ++g_deleted; std::free(D); } }
};

inline void* operator new(std::size_t, Shader* p) { return p; }

struct Vec {
    Shader* start; Shader* finish; Shader* eos; int overflowCalls;
    explicit Vec(int cap) : overflowCalls(0)
    {
        start = finish = (Shader*)std::malloc(sizeof(Shader) * cap); eos = start + cap;
    }
    void insert_overflow(Shader* pos, const Shader& x, unsigned int n)
    {
        ++overflowCalls;
        unsigned int sz = finish - start, cap = sz + n + 4;
        Shader* ns = (Shader*)std::malloc(sizeof(Shader) * cap); Shader* cur = ns;
        for (Shader* p = start; p != pos; ++p, ++cur) new (cur) Shader(*p);
        for (unsigned int i = 0; i < n; ++i, ++cur) new (cur) Shader(x);
        for (Shader* p = pos; p != finish; ++p, ++cur) new (cur) Shader(*p);
        for (Shader* p = start; p != finish; ++p) p->~Shader();
        std::free(start); start = ns; finish = cur; eos = ns + cap;
    }
    void fill_insert(Shader* pos, unsigned int n, const Shader& x)
    {
        if (n == 0) return;
        if ((unsigned int)(eos - finish) >= n) {
            Shader x_copy = x;
            unsigned int after = finish - pos; Shader* old_finish = finish;
            if (after > n) {
                Shader* c = finish; for (Shader* p = finish - n; p != old_finish; ++p, ++c) new (c) Shader(*p);
                finish += n;
                Shader* r = old_finish; Shader* l = old_finish - n; while (l != pos) *--r = *--l;
                for (Shader* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                Shader* c = finish; for (unsigned int i = n - after; i > 0; --i, ++c) new (c) Shader(x_copy);
                finish += n - after;
                c = finish; for (Shader* p = pos; p != old_finish; ++p, ++c) new (c) Shader(*p);
                finish += after;
                for (Shader* p = pos; p != old_finish; ++p) *p = x_copy;
            }
        } else
            insert_overflow(pos, x, n);
    }
};

static Data* mk(int id) { Data* d = (Data*)std::malloc(sizeof(Data)); d->ReferenceCount = 0; d->id = id; return d; }

int main()
{
    int bad = 0;
    Data* a = mk(1); Data* b = mk(2); Data* c = mk(3); Data* x = mk(9);
    { Shader hx; hx.D = x; ++x->ReferenceCount;
      Vec v(8);
      Data* init[3] = { a, b, c };
      for (int i = 0; i < 3; ++i) { new (v.finish) Shader(); v.finish->D = init[i]; ++init[i]->ReferenceCount; ++v.finish; }
      v.fill_insert(v.start + 1, 1, hx);                 // elems_after 2 > n 1: in-place shift branch
      int want1[4] = { 1, 9, 2, 3 };
      for (int i = 0; i < 4; ++i) if (v.start[i].D->id != want1[i]) bad = 1;
      v.fill_insert(v.start + 2, 3, hx);                 // elems_after 2 <= n 3: fill-tail branch
      int want2[7] = { 1, 9, 9, 9, 9, 2, 3 };
      if (v.finish - v.start != 7) bad = 1;
      for (int i = 0; i < 7 && !bad; ++i) if (v.start[i].D->id != want2[i]) bad = 1;
      if (x->ReferenceCount != 5 || b->ReferenceCount != 1 || c->ReferenceCount != 1 || v.overflowCalls) bad = 1;
      v.fill_insert(v.start, 0, hx);                     // n == 0: nothing
      v.fill_insert(v.start, 2, hx);                     // capacity 1 < 2: overflow path
      if (v.overflowCalls != 1 || v.finish - v.start != 9 || v.start[0].D->id != 9 || v.start[2].D->id != 1) bad = 1;
      if (x->ReferenceCount != 7) bad = 1;
      for (Shader* p = v.start; p != v.finish; ++p) p->~Shader();
      std::free(v.start);
    }
    if (g_deleted != 4) bad = 1;
    if (bad) { std::printf("00b6c5d0_TEST FAIL\n"); return 1; }
    std::printf("00b6c5d0_TEST PASS\n");
    return 0;
}