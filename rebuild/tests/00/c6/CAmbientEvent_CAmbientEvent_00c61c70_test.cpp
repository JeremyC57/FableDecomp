// Behaviour test for 0x00C61C70 CAmbientEvent::CAmbientEvent
#include <cstdio>
class CAmbientEvent { public:
    void*         m_pDef;
    unsigned long m_time;
    bool          m_flag;
    long          m_index;
    CAmbientEvent(void* def, bool flag)
        : m_pDef(def), m_time(0), m_flag(flag), m_index(-1) {}
};
int main()
{
    int def;
    CAmbientEvent a(&def, true), b(0, false);
    bool ok = a.m_pDef == &def && a.m_time == 0 && a.m_flag && a.m_index == -1
           && b.m_pDef == 0 && !b.m_flag && b.m_index == -1;
    if (!ok) { std::printf("00c61c70_TEST FAIL\n"); return 1; }
    std::printf("00c61c70_TEST PASS\n");
    return 0;
}