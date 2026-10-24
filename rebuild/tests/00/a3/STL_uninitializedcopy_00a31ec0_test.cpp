// Behaviour test for 0x00A31EC0 _STL::__uninitialized_copy over C3DGeneratorObject: every element copy-constructed into
// raw storage (one string copy each), the returned pointer is the end of the copies; an empty range copies nothing.
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
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
static C3DGeneratorObject* __uninitialized_copy(const C3DGeneratorObject* first, const C3DGeneratorObject* last,
                                                C3DGeneratorObject* result, const __false_type&)
{
    C3DGeneratorObject* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
int main()
{
    C3DGeneratorObject src[3];
    for (int i = 0; i < 3; ++i) Make(src[i], i + 1);
    static unsigned char raw[3 * sizeof(C3DGeneratorObject)];
    C3DGeneratorObject* dst = (C3DGeneratorObject*)raw;
    g_copies = 0;
    C3DGeneratorObject* end = __uninitialized_copy(src, src + 3, dst, __false_type());
    if (end != dst + 3 || g_copies != 3) { std::printf("00a31ec0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (!Same(dst[i], i + 1)) { std::printf("00a31ec0_TEST FAIL\n"); return 1; }
    if (__uninitialized_copy(src, src, dst, __false_type()) != dst || g_copies != 3) { std::printf("00a31ec0_TEST FAIL\n"); return 1; }
    std::printf("00a31ec0_TEST PASS\n");
    return 0;
}