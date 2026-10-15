#include <cstdio>
#include <cstring>
#include <new>
// Behaviour of NUISystem::CYesNo::CYesNo (0x0041CADC): the changing-state base gets the argument, both own dwords start at 0.
static long g_baseState = 0;
struct CComponentRoot_Retail { virtual long v00() { return 0; } };
struct CComponentBase2_Retail { virtual long v00() { return 0; } unsigned char _body[0x10]; };
struct CComponentBase3_Retail { virtual long v00() { return 0; } };
struct CChangingStateComponent : CComponentRoot_Retail, CComponentBase2_Retail, CComponentBase3_Retail {
    long m_state;
    CChangingStateComponent(long state) : m_state(state) { g_baseState = state; }
};
struct CYesNo : CChangingStateComponent {
    long m_result;
    long m_selection;
    virtual long v00() { return 1; }
    CYesNo(long state) : CChangingStateComponent(state), m_result(0), m_selection(0) {}
};
union UStorage { double d; void* p; unsigned char bytes[sizeof(CYesNo)]; };
int main()
{
    UStorage storage;
    std::memset(storage.bytes, 0xAB, sizeof(storage.bytes));
    CYesNo* yesNo = new (storage.bytes) CYesNo(5);
    if (g_baseState != 5 || yesNo->m_state != 5 || yesNo->m_result != 0 || yesNo->m_selection != 0 || yesNo->v00() != 1) {
        std::printf("0041cadc_TEST FAIL\n"); return 1;
    }
    std::printf("0041cadc_TEST PASS\n");
    return 0;
}