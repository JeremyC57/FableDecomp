// Behaviour test for 0x00A32100 _STL::fill over C3DGeneratorObject: every element of [first, last) becomes a copy of val
// (one string assignment each); an empty range is untouched.
#include <cstdio>
#include <new>

static int g_copies = 0, g_assigns = 0;
class CCharString {
public:
    int Id;
    CCharString() : Id(0) { }
    explicit CCharString(int id) : Id(id) { }
    CCharString(const CCharString& o) : Id(o.Id) { ++g_copies; }
    CCharString& operator=(const CCharString& o) { Id = o.Id; ++g_assigns; return *this; }
};
struct CMatrix3x4 { float M[12]; };
class C3DDummyObject { public: CMatrix3x4 Transform; int BoneIndex; };
class C3DGeneratorObject : public C3DDummyObject {
public:
    CCharString  ObjectName;
    unsigned int BankIndex;
    bool         UseLocalOrigin;
};
static void Make(C3DGeneratorObject& g, int k)
{
    for (int i = 0; i < 12; ++i) g.Transform.M[i] = (float)(k * 100 + i);
    g.BoneIndex = k; g.ObjectName.Id = 1000 + k; g.BankIndex = 2000 + k; g.UseLocalOrigin = (k & 1) != 0;
}
static bool Same(const C3DGeneratorObject& g, int k)
{
    for (int i = 0; i < 12; ++i) { float d = g.Transform.M[i] - (float)(k * 100 + i); if (d > 0.001f || d < -0.001f) return false; }
    return g.BoneIndex == k && g.ObjectName.Id == 1000 + k && g.BankIndex == (unsigned int)(2000 + k) && g.UseLocalOrigin == ((k & 1) != 0);
}
static void fill(C3DGeneratorObject* first, C3DGeneratorObject* last, const C3DGeneratorObject& val)
{
    for (; first != last; ++first)
        *first = val;
}
int main()
{
    C3DGeneratorObject a[4];
    for (int i = 0; i < 4; ++i) Make(a[i], i + 1);
    C3DGeneratorObject v; Make(v, 9);
    g_assigns = 0;
    fill(a + 1, a + 3, v);
    if (g_assigns != 2 || !Same(a[0], 1) || !Same(a[1], 9) || !Same(a[2], 9) || !Same(a[3], 4)) { std::printf("00a32100_TEST FAIL\n"); return 1; }
    fill(a, a, v);
    if (g_assigns != 2 || !Same(a[0], 1)) { std::printf("00a32100_TEST FAIL\n"); return 1; }
    std::printf("00a32100_TEST PASS\n");
    return 0;
}