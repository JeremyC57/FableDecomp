// Behaviour test for 00625350 CConfigOptionsDefaultsDef::operator=: the base part, every scalar, the
// 16-byte record, the member with its own assignment and the flag are copied; *this is returned.
#include <cstdio>

static int g_baseAssigns = 0, g_memberAssigns = 0;
struct CBase { long m_id; CBase& operator=(const CBase& o) { m_id = o.m_id; ++g_baseAssigns; return *this; } };
struct SRecord16 { long v[4]; };
struct CMember { long w; CMember& operator=(const CMember& o) { w = o.w + 1000; ++g_memberAssigns; return *this; } };
struct CDef : CBase {
    long m[7]; SRecord16 r; long m68; CMember m6c; bool m78;
    CDef& operator=(const CDef& o)
    {
        CBase::operator=(o);
        for (int i = 0; i < 7; ++i) m[i] = o.m[i];
        r = o.r; m68 = o.m68; m6c = o.m6c; m78 = o.m78;
        return *this;
    }
};

int main()
{
    CDef a, b;
    a.m_id = 5; for (int i = 0; i < 7; ++i) a.m[i] = i * 3; for (int i = 0; i < 4; ++i) a.r.v[i] = 40 + i;
    a.m68 = 68; a.m6c.w = 1; a.m78 = true;
    b.m_id = 0; b.m78 = false;
    CDef& ret = (b = a);
    bool ok = &ret == &b && b.m_id == 5 && b.m[6] == 18 && b.r.v[3] == 43 && b.m68 == 68 && b.m6c.w == 1001 && b.m78
        && g_baseAssigns == 1 && g_memberAssigns == 1;
    if (!ok) { std::printf("00625350_TEST FAIL\n"); return 1; }
    std::printf("00625350_TEST PASS\n");
    return 0;
}
