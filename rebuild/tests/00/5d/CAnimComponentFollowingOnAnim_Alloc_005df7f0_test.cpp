// Behaviour test for CAnimComponentFollowingOnAnim::Alloc @ 0x005df7f0: a fresh component with ref count 1 (and defaulted members), null when new fails.
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
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
struct CAnimComponentFollowingOnAnim : CAnimComponentBase {
    CDefString NextAnimName;
    CAnimComponentFollowingOnAnim() { }
    virtual long GetType() const { return 7; }
    virtual CAnimComponentBase* Alloc() const { return new CAnimComponentFollowingOnAnim; }
};
int main()
{
    CAnimComponentFollowingOnAnim proto;
    CAnimComponentBase* p = proto.Alloc();
    bool ok = p != 0 && p->m_refCount == 1 && p->GetType() == 7 && sizeof(CAnimComponentFollowingOnAnim) == 0xc;
    if (ok) { if (static_cast<CAnimComponentFollowingOnAnim*>(p)->NextAnimName.TablePos != -1) ok = false; }
    g_fail = true;
    if (proto.Alloc() != 0) ok = false;
    if (!ok) { std::printf("005df7f0_TEST FAIL\n"); return 1; }
    std::printf("005df7f0_TEST PASS\n");
    return 0;
}