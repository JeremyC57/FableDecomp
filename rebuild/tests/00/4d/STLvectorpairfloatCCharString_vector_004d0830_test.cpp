#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_copies = 0;
class CCharString { public:
    char m_text[16];
    CCharString(const char* s) { std::strcpy(m_text, s); }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_copies; }
};
struct SPair {
    float first; CCharString second;
    SPair(float f, const char* s) : first(f), second(s) { }
    SPair(const SPair& p) : first(p.first), second(p.second) { }
};
static SPair* UninitializedCopy(const SPair* first, const SPair* last, SPair* result)
{
    for (; first != last; ++first, ++result)
        new (result) SPair(*first);
    return result;
}
struct CVector {
    SPair* m_start; SPair* m_finish; SPair* m_end;
    unsigned long size() const { return (unsigned long)(m_finish - m_start); }
    CVector(const CVector& x) : m_start(0), m_finish(0), m_end(0)
    {
        unsigned long n = x.size();
        m_start = n != 0 ? static_cast<SPair*>(std::malloc(n * sizeof(SPair))) : 0;
        m_finish = m_start;
        m_end = m_start + n;
        m_finish = UninitializedCopy(x.m_start, x.m_finish, m_start);
    }
    CVector() : m_start(0), m_finish(0), m_end(0) { }
};
int main()
{
    bool ok = true;
    SPair src[3] = { SPair(1.5f, "one"), SPair(-2.0f, "two"), SPair(3.25f, "three") };
    CVector a; a.m_start = src; a.m_finish = src + 3; a.m_end = src + 3;
    CVector b(a);
    if (b.size() != 3 || b.m_end != b.m_start + 3 || g_copies != 3) ok = false;
    if (ok && (b.m_start[0].first != 1.5f || std::strcmp(b.m_start[1].second.m_text, "two") || b.m_start[2].first != 3.25f)) ok = false;
    CVector empty;
    CVector c(empty);
    if (c.m_start != 0 || c.m_finish != 0 || c.m_end != 0) ok = false;
    std::free(b.m_start);
    if (!ok) { std::printf("004d0830_TEST FAIL\n"); return 1; }
    std::printf("004d0830_TEST PASS\n");
    return 0;
}