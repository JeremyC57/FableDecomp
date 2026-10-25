// Behaviour test for pair<C3DVector, CCharString>::pair(const pair&) (0x00654DD4): memberwise vector copy + string copy ctor.
#include <cstdio>
#include <cstring>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

static int g_copies = 0;
class CCharString {
public:
    char Text[16];
    CCharString(const char* s) { std::strcpy(Text, s); }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); ++g_copies; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};

int main()
{
    pair<C3DVector, CCharString> a(C3DVector(1.0f, 2.5f, -3.0f), CCharString("ROOM"));
    g_copies = 0;
    pair<C3DVector, CCharString> b(a);
    if (g_copies != 1 || b.first.X != 1.0f || b.first.Y != 2.5f || b.first.Z != -3.0f || std::strcmp(b.second.Text, "ROOM") != 0) {
        std::printf("00654dd4_TEST FAIL\n"); return 1;
    }
    std::printf("00654dd4_TEST PASS\n");
    return 0;
}