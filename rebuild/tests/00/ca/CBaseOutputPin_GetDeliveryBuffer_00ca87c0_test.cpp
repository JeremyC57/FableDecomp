#include <cstdio>
// CBaseOutputPin::GetDeliveryBuffer: m_pAllocator->GetBuffer(...) or E_NOINTERFACE.
struct Alloc { int calls; unsigned flags; virtual int GetBuffer(void** pp, long long* s, long long* e, unsigned f) { ++calls; flags = f; *pp = this; return 7; } };
struct OutPin {
    Alloc* m_pAllocator;
    int GetDeliveryBuffer(void** pp, long long* s, long long* e, unsigned f)
    { if (m_pAllocator != 0) return m_pAllocator->GetBuffer(pp, s, e, f); else return (int)0x80004002; }
};
int main()
{
    Alloc al; al.calls = 0; al.flags = 0; OutPin p; void* smp = 0;
    p.m_pAllocator = 0;
    if (p.GetDeliveryBuffer(&smp, 0, 0, 0) != (int)0x80004002 || smp != 0) { std::printf("00ca87c0_TEST FAIL\n"); return 1; }
    p.m_pAllocator = &al;
    if (p.GetDeliveryBuffer(&smp, 0, 0, 3) != 7 || smp != &al || al.calls != 1 || al.flags != 3) { std::printf("00ca87c0_TEST FAIL\n"); return 1; }
    std::printf("00ca87c0_TEST PASS\n");
    return 0;
}