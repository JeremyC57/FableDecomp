// Behaviour test for 0x007658B0 std::vector<CCountedPointer<NShape::CShape> >::insert(pos, x).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_Destroyed = 0;

struct CShape {
    int id;
    void DeleteData() { ++g_Destroyed; }
};

struct SCountBlock {
    int     m_references;
    void (CShape::*m_pfnDestroy)();
    CShape* m_pItem;
};

struct CountedPointer {
    CShape*      Data;
    SCountBlock* Info;
    CountedPointer(CShape* s)
    {
        Data = s;
        Info = new SCountBlock;
        Info->m_references = 1;
        Info->m_pfnDestroy = &CShape::DeleteData;
        Info->m_pItem = s;
    }
    CountedPointer(const CountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CountedPointer() { release(); }
    void release()
    {
        if (Info && --Info->m_references == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            delete Info;
        }
    }
    CountedPointer& operator=(const CountedPointer& o)
    {
        if (Info != o.Info) {
            release();
            Data = o.Data;
            Info = o.Info;
            if (Info) ++Info->m_references;
        }
        return *this;
    }
};

struct Vec {
    CountedPointer* m_start;
    CountedPointer* m_finish;
    CountedPointer* m_end;
    int overflows;
    void overflow(CountedPointer* pos, const CountedPointer& x)        // _M_insert_overflow(pos, x, tag, 1, false)
    {
        ++overflows;
        unsigned int old = (unsigned int)(m_finish - m_start);
        unsigned int len = old + (old < 1 ? 1 : old);
        CountedPointer* ns = (CountedPointer*)std::malloc(len * sizeof(CountedPointer));
        CountedPointer* nf = ns;
        for (CountedPointer* p = m_start; p != pos; ++p, ++nf) new (nf) CountedPointer(*p);
        new (nf) CountedPointer(x); ++nf;
        for (CountedPointer* p = pos; p != m_finish; ++p, ++nf) new (nf) CountedPointer(*p);
        for (CountedPointer* p = m_start; p != m_finish; ++p) p->~CountedPointer();
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
    CountedPointer* insert(CountedPointer* pos, const CountedPointer& x)
    {
        unsigned int n = (unsigned int)(pos - m_start);
        if (m_finish != m_end) {
            if (pos == m_finish) {
                new (m_finish) CountedPointer(x);
                ++m_finish;
            } else {
                new (m_finish) CountedPointer(*(m_finish - 1));
                ++m_finish;
                CountedPointer x_copy = x;
                CountedPointer* last = m_finish - 2;
                CountedPointer* result = m_finish - 1;
                for (int k = (int)(last - pos); k > 0; --k) *--result = *--last;
                *pos = x_copy;
            }
        } else
            overflow(pos, x);
        return m_start + n;
    }
};

int main()
{
    int ok = 1;
    CShape s1, s2, s3;
    s1.id = 1; s2.id = 2; s3.id = 3;
    Vec v;
    v.m_start = (CountedPointer*)std::malloc(4 * sizeof(CountedPointer));
    v.m_finish = v.m_start;
    v.m_end = v.m_start + 4;
    v.overflows = 0;
    {
        CountedPointer a(&s1), b(&s2), c(&s3);
        CountedPointer* r = v.insert(v.m_finish, a);              // append in place
        if (r != v.m_start || v.m_start[0].Data != &s1 || a.Info->m_references != 2) ok = 0;
        r = v.insert(v.m_start, b);                               // front insert: shift a up, assign b
        if (r != v.m_start || v.m_start[0].Data != &s2 || v.m_start[1].Data != &s1) ok = 0;
        if (a.Info->m_references != 2 || b.Info->m_references != 2) ok = 0;
        r = v.insert(v.m_start + 1, c);                           // middle insert: [b, c, a]
        if (r != v.m_start + 1 || v.m_start[1].Data != &s3 || v.m_start[2].Data != &s1) ok = 0;
        r = v.insert(v.m_finish, a);                              // fills the 4th slot: [b, c, a, a]
        if (v.m_finish - v.m_start != 4 || a.Info->m_references != 3 || v.overflows != 0) ok = 0;
        r = v.insert(v.m_start, c);                               // full: grows through the overflow path
        if (v.overflows != 1 || v.m_finish - v.m_start != 5 || v.m_end - v.m_start != 8 || r != v.m_start) ok = 0;
        if (v.m_start[0].Data != &s3 || v.m_start[1].Data != &s2 || v.m_start[4].Data != &s1) ok = 0;
        if (c.Info->m_references != 3 || b.Info->m_references != 2 || a.Info->m_references != 3) ok = 0;
    }
    if (g_Destroyed != 0) ok = 0;                                 // the vector still holds every shape
    for (CountedPointer* p = v.m_start; p != v.m_finish; ++p) p->~CountedPointer();
    std::free(v.m_start);
    if (g_Destroyed != 3) ok = 0;
    if (sizeof(CShape*) == 4 && sizeof(CountedPointer) != 8) ok = 0;
    if (!ok) { std::printf("007658b0_TEST FAIL\n"); return 1; }
    std::printf("007658b0_TEST PASS\n");
    return 0;
}