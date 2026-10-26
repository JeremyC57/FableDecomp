// Behaviour test for 0x00768BC0: name, tag, then the intelligent pointer (base ctor clears the item) are constructed in order.
#include <cstdio>
static int g_n = 0;
class CCharString { public: int order; CCharString() { order = g_n++; } };
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() {} void* PItem; int order; CBaseIntelligentPointer() : PItem(0) { order = g_n++; } };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: CIntelligentPointer() {} };
class CThing;
struct CEntry { CCharString m_name; CCharString m_tag; long m_value08; long m_value0c; CIntelligentPointer<CThing> m_thing; CEntry() {} };
int main()
{
    CEntry e;
    if (g_n != 3 || e.m_name.order != 0 || e.m_tag.order != 1 || e.m_thing.order != 2 || e.m_thing.PItem != 0) { std::printf("00768bc0_TEST FAIL\n"); return 1; }
    std::printf("00768bc0_TEST PASS\n");
    return 0;
}