// Behaviour test for NUISystem::CSwappingStateComponent::CSwappingStateComponent (0x00547600): empty STLport-style vector before GetDataFromDef.
#include <cstdio>
struct SSwapState { unsigned long State; float Time; };
struct CSwapStateAllocator { CSwapStateAllocator() {} };
struct CSwapStateAllocProxy : CSwapStateAllocator {
    SSwapState* _M_data;
    CSwapStateAllocProxy(const CSwapStateAllocator& a, SSwapState* p) : CSwapStateAllocator(a), _M_data(p) {}
};
struct CSwapStateVectorBase {
    SSwapState* _M_start;
    SSwapState* _M_finish;
    CSwapStateAllocProxy _M_end_of_storage;
    CSwapStateVectorBase(const CSwapStateAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
};
struct CSwapStateVector : CSwapStateVectorBase {
    explicit CSwapStateVector(const CSwapStateAllocator& a = CSwapStateAllocator()) : CSwapStateVectorBase(a) {}
};
struct CChangingStateComponent {
    long m_owner;
    CChangingStateComponent(long owner) : m_owner(owner) {}
    virtual ~CChangingStateComponent() {}
};
static bool g_emptyBeforeDef = false;
struct CSwappingStateComponent : CChangingStateComponent {
    CSwapStateVector SwappingStates;
    float LastSwapTime;
    unsigned long Seed;
    CSwappingStateComponent(long owner) : CChangingStateComponent(owner) { GetDataFromDef(); }
    void GetDataFromDef()
    {
        g_emptyBeforeDef = SwappingStates._M_start == 0 && SwappingStates._M_finish == 0 && SwappingStates._M_end_of_storage._M_data == 0;
    }
};
int main()
{
    CSwappingStateComponent s(3);
    if (s.m_owner != 3 || !g_emptyBeforeDef) { std::printf("00547600_TEST FAIL\n"); return 1; }
    std::printf("00547600_TEST PASS\n");
    return 0;
}
