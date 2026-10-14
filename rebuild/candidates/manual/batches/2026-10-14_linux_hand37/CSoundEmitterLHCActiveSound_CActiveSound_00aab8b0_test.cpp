#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CBase { virtual void v00() { } CBase() { } };
struct CCharString { void* m_pData; CCharString(const char* t) : m_pData((void*)t) { } };
struct CSoundEmitterLH { struct CActiveSound : CBase { long m_references; void* m_pA; void* m_pB; void* m_pD; bool m_e; bool m_f; unsigned char _pad16[2]; long m_18; CCharString m_name; long m_20;
    CActiveSound(void* a, void* b, const char* name, void* d, bool e, bool f) : CBase(), m_references(1), m_pA(a), m_pB(b), m_pD(d), m_e(e), m_f(f), m_18(0), m_name(name), m_20(0) { } }; };
int main() {
    int a, b, d; CSoundEmitterLH::CActiveSound s(&a, &b, "name", &d, true, false);
    if (s.m_references != 1 || s.m_pA != &a || s.m_pB != &b || s.m_pD != &d || !s.m_e || s.m_f || s.m_18 || std::strcmp((const char*)s.m_name.m_pData, "name") || s.m_20) { std::printf("00aab8b0_TEST FAIL\n"); return 1; }
    std::printf("00aab8b0_TEST PASS\n"); return 0;
}
