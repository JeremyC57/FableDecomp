// Behaviour test for 0x00A32170 _STL::__copy (counted) over C3DGeneratorObject: [first, last) assigned onto result in
// order (overlapping downward copy as the vector erase uses it), the end of the destination returned; empty range -> result.
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };
static C3DGeneratorObject* __copy(C3DGeneratorObject* first, C3DGeneratorObject* last, C3DGeneratorObject* result,
                                  const random_access_iterator_tag&, int*)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
int main()
{
    C3DGeneratorObject a[5];
    for (int i = 0; i < 5; ++i) Make(a[i], i + 1);
    g_assigns = 0;
    C3DGeneratorObject* end = __copy(a + 2, a + 5, a + 1, random_access_iterator_tag(), (int*)0);
    if (end != a + 4 || g_assigns != 3) { std::printf("00a32170_TEST FAIL\n"); return 1; }
    if (!Same(a[0], 1) || !Same(a[1], 3) || !Same(a[2], 4) || !Same(a[3], 5) || !Same(a[4], 5)) { std::printf("00a32170_TEST FAIL\n"); return 1; }
    if (__copy(a, a, a + 2, random_access_iterator_tag(), (int*)0) != a + 2 || g_assigns != 3) { std::printf("00a32170_TEST FAIL\n"); return 1; }
    std::printf("00a32170_TEST PASS\n");
    return 0;
}