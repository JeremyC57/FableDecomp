// Behaviour test for pair<C3DVector, CCharString>::operator=(const pair&) (0x00654EB6): bitwise vector copy + string assignment.
#include <cstdio>
#include <cstring>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

static int g_assigns = 0;
class CCharString {
public:
    char Text[16];
    CCharString(const char* s) { std::strcpy(Text, s); }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); }
    CCharString& operator=(const CCharString& other) { std::strcpy(Text, other.Text); ++g_assigns; return *this; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair& operator=(const pair& p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }
};

int main()
{
    pair<C3DVector, CCharString> a(C3DVector(1.0f, 2.0f, 3.0f), CCharString("A"));
    pair<C3DVector, CCharString> b(C3DVector(-4.0f, 0.5f, 9.0f), CCharString("BEE"));
    pair<C3DVector, CCharString>& r = (a = b);
    if (&r != &a || g_assigns != 1 || a.first.X != -4.0f || a.first.Y != 0.5f || a.first.Z != 9.0f || std::strcmp(a.second.Text, "BEE") != 0) {
        std::printf("00654eb6_TEST FAIL\n"); return 1;
    }
    std::printf("00654eb6_TEST PASS\n");
    return 0;
}