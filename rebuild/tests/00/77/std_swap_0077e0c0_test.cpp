// Behaviour test for 0x0077E0C0 std::swap<pair<CCharString,C3DVector> >
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CCharString {
    char Text[16];
    CCharString(const char* s) { std::strcpy(Text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct SPair {
    CCharString first;
    C3DVector second;
    SPair(const char* n, float x) : first(n), second(x, x + 1, x + 2) { }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};
static void Swap(SPair& a, SPair& b)
{
    SPair tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    {
        SPair a("hand", 1.0f), b("foot", 10.0f);
        Swap(a, b);
        if (std::strcmp(a.first.Text, "foot") != 0 || std::strcmp(b.first.Text, "hand") != 0 || a.second.X != 10.0f
            || a.second.Z != 12.0f || b.second.Y != 2.0f) { std::printf("0077e0c0_TEST FAIL\n"); return 1; }
        if (g_live != 2) { std::printf("0077e0c0_TEST FAIL\n"); return 1; }   // the temporary is destroyed
    }
    std::printf("0077e0c0_TEST PASS\n");
    return 0;
}