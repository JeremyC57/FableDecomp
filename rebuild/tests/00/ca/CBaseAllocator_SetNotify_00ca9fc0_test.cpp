#include <cstdio>
// CBaseAllocator::SetNotify: under the allocator lock AddRef the new sink, Release the old one, store the new one.
struct Lock { int depth, entered; };
struct AutoLock { Lock* l; AutoLock(Lock* p) : l(p) { ++l->depth; ++l->entered; } ~AutoLock() { --l->depth; } };
struct Sink { int refs; void AddRef() { ++refs; } void Release() { --refs; } };
struct Allocator {
    Lock m_lock; Sink* m_pNotify;
    int SetNotify(Sink* pNotify)
    {
        AutoLock lck(&m_lock);
        if (pNotify) pNotify->AddRef();
        if (m_pNotify) m_pNotify->Release();
        m_pNotify = pNotify;
        return 0;
    }
};
int main()
{
    Allocator a; a.m_lock.depth = 0; a.m_lock.entered = 0; a.m_pNotify = 0;
    Sink s1, s2; s1.refs = 1; s2.refs = 1;
    if (a.SetNotify(&s1) != 0 || a.m_pNotify != &s1 || s1.refs != 2) { std::printf("00ca9fc0_TEST FAIL\n"); return 1; }
    if (a.SetNotify(&s2) != 0 || a.m_pNotify != &s2 || s1.refs != 1 || s2.refs != 2) { std::printf("00ca9fc0_TEST FAIL\n"); return 1; }
    if (a.SetNotify(0) != 0 || a.m_pNotify != 0 || s2.refs != 1 || a.m_lock.depth != 0 || a.m_lock.entered != 3) { std::printf("00ca9fc0_TEST FAIL\n"); return 1; }
    std::printf("00ca9fc0_TEST PASS\n");
    return 0;
}