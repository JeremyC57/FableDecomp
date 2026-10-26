#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_live = 0;
class CCharString { public:
    char m_text[12];
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_live; }
    ~CCharString() { --g_live; }
};
struct SPair {
    float first; CCharString second;
    SPair(const float& a, const CCharString& b) : first(a), second(b) { }
    SPair(const SPair& p) : first(p.first), second(p.second) { }
};
struct CVector {
    SPair* m_start; SPair* m_finish; SPair* m_end;
    int m_overflows;
    void InsertOverflow(SPair* pos, const SPair& x)
    {
        unsigned long n = (unsigned long)(m_finish - m_start);
        unsigned long len = n + (n > 1 ? n : 1);
        SPair* p = static_cast<SPair*>(std::malloc(len * sizeof(SPair)));
        SPair* d = p;
        for (SPair* s = m_start; s != pos; ++s, ++d) { new (d) SPair(*s); s->~SPair(); }
        new (d) SPair(x); ++d;
        std::free(m_start);
        m_start = p; m_finish = d; m_end = p + len;
        ++m_overflows;
    }
    void push_back(const SPair& x)
    {
        if (m_finish != m_end) { new (m_finish) SPair(x); ++m_finish; }
        else InsertOverflow(m_finish, x);
    }
};
class CMap { public:
    CVector Vector; unsigned char Compare; bool Dirty;
    void Insert(const float& key, const CCharString& value) { Vector.push_back(SPair(key, value)); Dirty = true; }
};
int main()
{
    bool ok = true;
    {
        CMap m; m.Vector.m_start = m.Vector.m_finish = m.Vector.m_end = 0; m.Vector.m_overflows = 0; m.Dirty = false;
        CCharString a("alpha"), b("beta"), c("gamma");
        m.Insert(2.5f, a);
        if (!m.Dirty || m.Vector.m_overflows != 1 || m.Vector.m_finish - m.Vector.m_start != 1) ok = false;
        m.Dirty = false;
        m.Insert(1.0f, b);                                  // full again (capacity 1)
        m.Insert(4.0f, c);                                  // room after doubling: in place
        if (!m.Dirty || m.Vector.m_finish - m.Vector.m_start != 3) ok = false;
        if (ok && (m.Vector.m_start[0].first != 2.5f || std::strcmp(m.Vector.m_start[1].second.m_text, "beta") || std::strcmp(m.Vector.m_start[2].second.m_text, "gamma"))) ok = false;
        if (g_live != 6) ok = false;                         // three locals + three stored, every temporary destroyed
        for (SPair* p = m.Vector.m_start; p != m.Vector.m_finish; ++p) p->~SPair();
        std::free(m.Vector.m_start);
    }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004d2060_TEST FAIL\n"); return 1; }
    std::printf("004d2060_TEST PASS\n");
    return 0;
}