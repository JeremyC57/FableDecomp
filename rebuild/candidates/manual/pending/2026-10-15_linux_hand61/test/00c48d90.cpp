#include <cstdio>
// Behaviour test for CPointerHolder_00c48d90::CPointerHolder_00c48d90 @ 0x00C48D90
static int g_pointerCtors = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : m_pObject(0) { ++g_pointerCtors; }
    void* m_pObject;
};
class CPointerHolder_00c48d90 {
public:
    CPointerHolder_00c48d90() : m_a(0), m_b(0), m_c(0) {}
    unsigned long m_unknown0, m_a, m_b, m_c;
    CBaseIntelligentPointer m_pointer;
};
int main()
{
    CPointerHolder_00c48d90 h;
    if (h.m_a != 0 || h.m_b != 0 || h.m_c != 0 || g_pointerCtors != 1 || h.m_pointer.m_pObject != 0) { std::printf("00c48d90_TEST FAIL\n"); return 1; }
    std::printf("00c48d90_TEST PASS\n");
    return 0;
}
