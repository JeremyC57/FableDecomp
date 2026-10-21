#include <stdio.h>
inline void* operator new(unsigned int, void* where) throw() { return where; }
static int g_copies; static const void* g_src;
struct CWideString {
    void* m_pData;
    CWideString(void* d) : m_pData(d) {}
    CWideString(const CWideString& other) : m_pData(other.m_pData) { ++g_copies; g_src = &other; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
}
typedef std::pair<CWideString, C3DVector> P;
int main()
{
    P src(CWideString((void*)0x77), C3DVector(1, 2, 3));
    g_copies = 0;
    unsigned char raw[sizeof(P)] = { 0 };
    std::_Construct((P*)raw, src);
    P* d = (P*)raw;
    bool ok = g_copies == 1 && g_src == &src.first && d->first.m_pData == (void*)0x77 && d->second.X == 1 && d->second.Y == 2 && d->second.Z == 3;
    std::_Construct((P*)0, src);
    ok = ok && g_copies == 1;
    printf(ok ? "0043f400_TEST PASS\n" : "0043f400_TEST FAIL\n");
    return ok ? 0 : 1;
}