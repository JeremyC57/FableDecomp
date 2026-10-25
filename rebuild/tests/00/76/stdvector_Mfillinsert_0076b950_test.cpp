// Behaviour test for 0x0076B950 std::vector<CDefPointer<const CThingBaseDef> >::_M_fill_insert (STLport fill insert).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_destroyed = 0;

struct Def {
    long m_refCount;
    Def() : m_refCount(1) { }
    void Destroy() { ++g_destroyed; }
};

struct DefPtr {                                 // CDefPointer<const CThingBaseDef>
    Def* Object;
    explicit DefPtr(Def* d) : Object(d) { if (Object) ++Object->m_refCount; }
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    ~DefPtr() { if (Object) if (--Object->m_refCount == 0) Object->Destroy(); }
    DefPtr& operator=(const DefPtr& o)
    {
        Def* old = Object;
        if (old) if (--old->m_refCount == 0) old->Destroy();
        Object = o.Object; if (Object) ++Object->m_refCount;
        return *this;
    }
};

struct Vec {
    DefPtr* m_start; DefPtr* m_finish; DefPtr* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(DefPtr* pos, const DefPtr& x, unsigned int fill_len)     // 0x005B3275
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        DefPtr* ns = (DefPtr*)std::malloc(len * sizeof(DefPtr));
        DefPtr* nf = ns;
        for (DefPtr* p = m_start; p != pos; ++p, ++nf) new (nf) DefPtr(*p);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) DefPtr(x);
        for (DefPtr* p = pos; p != m_finish; ++p, ++nf) new (nf) DefPtr(*p);
        for (DefPtr* d = m_start; d != m_finish; ++d) d->~DefPtr();
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
    void fill_insert(DefPtr* pos, unsigned int n, const DefPtr& x)
    {
        if (n != 0) {
            if ((unsigned int)(m_end - m_finish) >= n) {
                DefPtr x_copy(x);
                const unsigned int elems_after = (unsigned int)(m_finish - pos);
                DefPtr* old_finish = m_finish;
                if (elems_after > n) {
                    DefPtr* r = m_finish;
                    for (DefPtr* p = m_finish - n; p != m_finish; ++p, ++r) new (r) DefPtr(*p);
                    m_finish += n;
                    DefPtr* res = old_finish; DefPtr* last = old_finish - n;
                    for (int k = (int)(last - pos); k > 0; --k) *--res = *--last;
                    for (DefPtr* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    DefPtr* f = m_finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++f) new (f) DefPtr(x_copy);
                    m_finish += n - elems_after;
                    DefPtr* r = m_finish;
                    for (DefPtr* p = pos; p != old_finish; ++p, ++r) new (r) DefPtr(*p);
                    m_finish += elems_after;
                    for (DefPtr* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            } else
                insert_overflow(pos, x, n);
        }
    }
};

int main()
{
    int ok = 1;
    Def a, b;
    {
        DefPtr pa(&a), pb(&b);
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        v.fill_insert(v.m_finish, 3, pa);                                   // grow to 3
        if (v.size() != 3 || a.m_refCount != 1 + 1 + 3) ok = 0;
        v.fill_insert(v.m_finish, 3, pb);                                   // grow to 6
        if (v.size() != 6 || b.m_refCount != 1 + 1 + 3) ok = 0;
        for (int k = 0; k < 3; ++k) { --v.m_finish; v.m_finish->~DefPtr(); }   // [a a a], capacity 6
        v.fill_insert(v.m_start + 1, 1, pb);                               // elems_after 2 > 1
        if (v.size() != 4 || v.m_start[1].Object != &b || v.m_start[3].Object != &a || v.m_start[0].Object != &a) ok = 0;
        v.fill_insert(v.m_start + 3, 2, pb);                               // elems_after 1 <= 2
        if (v.size() != 6 || v.m_start[3].Object != &b || v.m_start[4].Object != &b || v.m_start[5].Object != &a) ok = 0;
        if (a.m_refCount != 1 + 1 + 3 || b.m_refCount != 1 + 1 + 3) ok = 0;
        for (DefPtr* d = v.m_start; d != v.m_finish; ++d) d->~DefPtr();
        std::free(v.m_start);
    }
    if (a.m_refCount != 1 || b.m_refCount != 1 || g_destroyed != 0) ok = 0;
    if (!ok) { std::printf("0076b950_TEST FAIL\n"); return 1; }
    std::printf("0076b950_TEST PASS\n");
    return 0;
}