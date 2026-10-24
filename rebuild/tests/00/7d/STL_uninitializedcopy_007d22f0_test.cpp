// Behaviour test for 0x007D22F0 __uninitialized_copy<const CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*>: every
// def is copy-constructed (base part, Start, Score) into raw storage; returns the end of the copies.
#include <cstdio>
#include <cstdlib>
#include <new>

class CDefClassBase {
public:
    int GlobalIndex;
    int copies;
    CDefClassBase() : GlobalIndex(0), copies(0) { }
    CDefClassBase(const CDefClassBase& other) : GlobalIndex(other.GlobalIndex), copies(other.copies + 1) { }
    virtual ~CDefClassBase() { }
    virtual int Kind() const { return 0; }
};
class CShoveHaPennyRegionDef : public CDefClassBase {
public:
    float Start;
    float Score;
    virtual int Kind() const { return 7; }
};
struct __false_type { };
template <class I, class F>
F __uninitialized_copy(I first, I last, F result, const __false_type&)
{
    F cur = result;
    for (; first != last; ++first, ++cur)
        new (&*cur) CShoveHaPennyRegionDef(*first);
    return cur;
}

int main()
{
    CShoveHaPennyRegionDef src[3];
    for (int i = 0; i < 3; ++i) { src[i].GlobalIndex = 10 + i; src[i].Start = 0.5f * i; src[i].Score = 2.0f * i; }
    CShoveHaPennyRegionDef* raw = (CShoveHaPennyRegionDef*)std::malloc(3 * sizeof(CShoveHaPennyRegionDef));
    const CShoveHaPennyRegionDef* first = src;
    CShoveHaPennyRegionDef* end = __uninitialized_copy(first, first + 3, raw, __false_type());
    bool ok = end == raw + 3;
    for (int i = 0; i < 3 && ok; ++i)
        ok = raw[i].GlobalIndex == 10 + i && raw[i].Start == 0.5f * i && raw[i].Score == 2.0f * i &&
             raw[i].copies == 1 && raw[i].Kind() == 7;
    ok = ok && __uninitialized_copy(first, first, raw, __false_type()) == raw;
    for (int i = 0; i < 3; ++i) raw[i].~CShoveHaPennyRegionDef();
    std::free(raw);
    if (!ok) { std::printf("007d22f0_TEST FAIL\n"); return 1; }
    std::printf("007d22f0_TEST PASS\n");
    return 0;
}