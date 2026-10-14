// Behaviour test for CAnimFilterAnyGroup::Clone @ 0x008350d0: a fresh stateless filter of the same dynamic type, null when new fails.
#include <cstdio>
#include <cstdlib>
static bool g_fail = false;
void* operator new(size_t size) { if (g_fail) return 0; return std::malloc(size); }
void operator delete(void* p) { std::free(p); }
struct CAnimFilterBase {
    virtual ~CAnimFilterBase() { }
    virtual CAnimFilterBase* Clone() const = 0;
    virtual bool Matches(long group) const = 0;
};
struct CAnimFilterAnyGroup : CAnimFilterBase {
    CAnimFilterAnyGroup() { }
    virtual CAnimFilterBase* Clone() const { return new CAnimFilterAnyGroup; }
    virtual bool Matches(long) const { return true; }
};
int main()
{
    CAnimFilterAnyGroup proto;
    CAnimFilterBase* copy = proto.Clone();
    bool ok = copy != 0 && copy != &proto && copy->Matches(5) && sizeof(CAnimFilterAnyGroup) == 4;
    delete copy;
    g_fail = true;
    if (proto.Clone() != 0) ok = false;
    if (!ok) { std::printf("008350d0_TEST FAIL\n"); return 1; }
    std::printf("008350d0_TEST PASS\n");
    return 0;
}