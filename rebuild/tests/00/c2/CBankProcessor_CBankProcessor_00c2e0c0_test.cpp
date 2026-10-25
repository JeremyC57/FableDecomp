#include <cstdio>
static int g_drivers = 0, g_samples = 0;
class CBankData; class CScriptHost; class CLoadLog;
class CDriverMap { unsigned char _body[0x30]; public: CDriverMap() { ++g_drivers; } ~CDriverMap() { } };
class CSampleMap { unsigned char _body[0x30]; public: CSampleMap() { ++g_samples; } ~CSampleMap() { } };
class CBankProcessor {
public:
    CBankData* m_pData; CScriptHost* m_pManager; CLoadLog* m_pLog;
    CDriverMap m_drivers; CSampleMap m_samples;
    void* m_pCriteriaTriggers; void* m_pNumberTriggers;
    bool m_option74, m_option75, m_option76, m_option77, m_option78, m_option79, m_option7a;
    CBankProcessor(CBankData* data, CScriptHost* manager, CLoadLog* log)
        : m_pData(data), m_pManager(manager), m_pLog(log)
    {
        m_pCriteriaTriggers = 0; m_pNumberTriggers = 0;
        m_option74 = false; m_option75 = false; m_option76 = false; m_option77 = false;
        m_option78 = false; m_option79 = false; m_option7a = false;
    }
};
int main()
{
    int a, b, c;
    CBankProcessor p((CBankData*)&a, (CScriptHost*)&b, (CLoadLog*)&c);
    bool ok = (void*)p.m_pData == &a && (void*)p.m_pManager == &b && (void*)p.m_pLog == &c && g_drivers == 1 && g_samples == 1
        && p.m_pCriteriaTriggers == 0 && p.m_pNumberTriggers == 0 && !p.m_option74 && !p.m_option7a
        && (char*)&p.m_samples - (char*)&p == 0x3c && (char*)&p.m_option7a - (char*)&p == 0x7a;
    if (!ok) { std::printf("00c2e0c0_TEST FAIL\n"); return 1; }
    std::printf("00c2e0c0_TEST PASS\n");
    return 0;
}
