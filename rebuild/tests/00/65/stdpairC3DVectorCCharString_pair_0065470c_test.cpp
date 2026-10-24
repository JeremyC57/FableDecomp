// Behaviour test for 0065470c: std::pair<C3DVector,CCharString>::pair(const C3DVector&, const CCharString&) -- the
// vector is copied member by member and the string through its copy constructor once.
#include <cstdio>
#include <cstring>

static int g_copies = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
template <class T1, class T2> struct SPair {
    T1 first;
    T2 second;
    SPair(const T1& a, const T2& b) : first(a), second(b) { }
};

int main()
{
    C3DVector pos(1.5f, -2.25f, 100.0f);
    CCharString name("HeroSpawn");
    SPair<C3DVector, CCharString> entry(pos, name);
    if (entry.first.X != 1.5f || entry.first.Y != -2.25f || entry.first.Z != 100.0f ||
        std::strcmp(entry.second.PStringData, "HeroSpawn") != 0 || g_copies != 1) {
        std::printf("0065470c_TEST FAIL\n");
        return 1;
    }
    std::printf("0065470c_TEST PASS\n");
    return 0;
}