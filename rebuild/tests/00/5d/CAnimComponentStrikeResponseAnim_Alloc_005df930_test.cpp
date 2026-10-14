// Behaviour test for CAnimComponentStrikeResponseAnim::Alloc @ 0x005df930: a fresh component with ref count 1 (and defaulted members), null when new fails.
#include <cstdio>
#include <cstdlib>
static bool g_fail = false;
void* operator new(size_t size) { if (g_fail) return 0; return std::malloc(size); }
void operator delete(void* p) { std::free(p); }
struct CPersistContext; struct CCharString; struct CDataInputStream; struct CDataOutputStream;
struct CIVCountedPointeeBase { virtual void v0() { } long m_refCount; CIVCountedPointeeBase() : m_refCount(1) { } };
struct CAnimComponentBase : CIVCountedPointeeBase {
    virtual void v1() { }
    virtual long GetType() const { return -1; }
    virtual CAnimComponentBase* Alloc() const { return 0; }
};
struct CCharString { void* PStringData; CCharString() : PStringData(0) { } };
struct CAnimComponentStrikeResponseAnim : CAnimComponentBase {
    CCharString ResponseAnimName;
    CAnimComponentStrikeResponseAnim() { }
    virtual long GetType() const { return 7; }
    virtual CAnimComponentBase* Alloc() const { return new CAnimComponentStrikeResponseAnim; }
};
int main()
{
    CAnimComponentStrikeResponseAnim proto;
    CAnimComponentBase* p = proto.Alloc();
    bool ok = p != 0 && p->m_refCount == 1 && p->GetType() == 7 && sizeof(CAnimComponentStrikeResponseAnim) == 0xc;
    if (ok) { if (static_cast<CAnimComponentStrikeResponseAnim*>(p)->ResponseAnimName.PStringData != 0) ok = false; }
    g_fail = true;
    if (proto.Alloc() != 0) ok = false;
    if (!ok) { std::printf("005df930_TEST FAIL\n"); return 1; }
    std::printf("005df930_TEST PASS\n");
    return 0;
}