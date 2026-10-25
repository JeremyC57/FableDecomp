// Behaviour test for 00ca7c80: CBasePin::~CBasePin -- name freed, then m_mt, then the CBaseObject counter
#include <cstdio>
#include <cstring>
static char g_seq[16] = ""; static long g_objects = 0;
class CBaseObject { public: CBaseObject() { ++g_objects; } ~CBaseObject() { --g_objects; std::strcat(g_seq, "O"); } };
class CMediaType { public: ~CMediaType() { std::strcat(g_seq, "M"); } };
class CUnknown : public CBaseObject { public: virtual ~CUnknown() { } };
class CBasePin : public CUnknown {
public:
    unsigned short* m_pName; CMediaType m_mt;
    CBasePin() { m_pName = new unsigned short[6]; }
    virtual ~CBasePin() { delete[] m_pName; m_pName = 0; std::strcat(g_seq, "N"); }
};
int main()
{
    CBasePin* p = new CBasePin; int live = g_objects;
    delete p;
    int ok = live == 1 && g_objects == 0 && !std::strcmp(g_seq, "NMO");
    std::printf(ok ? "00ca7c80_TEST PASS\n" : "00ca7c80_TEST FAIL\n");
    return ok ? 0 : 1;
}
