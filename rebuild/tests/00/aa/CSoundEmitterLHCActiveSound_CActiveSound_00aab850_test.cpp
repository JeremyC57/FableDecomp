// Behaviour test for CSoundEmitterLH::CActiveSound::CActiveSound(a, b, c, d, e, f) @ 0x00AAB850: the four
// words land in m_pA / m_pB / m_18 / m_pD, the flags in m_e / m_f, the count and trailing word start at 0
// and the name is empty.
#include <cstdio>

static const char g_empty[1] = { 0 };

struct CBase {
    virtual ~CBase() {}
    CBase() {}
};

struct CCharString {
    const char* m_pData;
    CCharString() : m_pData(g_empty) {}
};

struct CSoundEmitterLH {
    struct CActiveSound : CBase {
        long        m_references;
        void*       m_pA;
        void*       m_pB;
        void*       m_pD;
        bool        m_e;
        bool        m_f;
        void*       m_18;
        CCharString m_name;
        long        m_20;
        CActiveSound(void* a, void* b, void* c, void* d, bool e, bool f)
            : CBase(), m_references(0), m_pA(a), m_pB(b), m_pD(d), m_e(e), m_f(f), m_18(c), m_name(), m_20(0) {}
    };
};

int main()
{
    int a, b, c, d;
    CSoundEmitterLH::CActiveSound s(&a, &b, &c, &d, true, false);
    bool ok = s.m_pA == &a && s.m_pB == &b && s.m_18 == &c && s.m_pD == &d && s.m_e && !s.m_f
           && s.m_references == 0 && s.m_20 == 0 && s.m_name.m_pData == g_empty;
    CSoundEmitterLH::CActiveSound t(0, 0, 0, 0, false, true);
    if (t.m_e || !t.m_f || t.m_18 != 0) ok = false;
    if (!ok) { std::printf("00aab850_TEST FAIL\n"); return 1; }
    std::printf("00aab850_TEST PASS\n");
    return 0;
}