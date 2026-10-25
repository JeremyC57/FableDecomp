#include <cstdio>
static int g_order[2], g_n = 0;
class CDriverMap { unsigned char _body[0x30]; public: CDriverMap() { } ~CDriverMap() { g_order[g_n++] = 1; } };
class CSampleMap { unsigned char _body[0x30]; public: CSampleMap() { } ~CSampleMap() { g_order[g_n++] = 2; } };
class CBankProcessor {
public:
    void* m_pData; void* m_pManager; void* m_pLog;
    CDriverMap m_drivers; CSampleMap m_samples;
    void* m_pCriteriaTriggers; void* m_pNumberTriggers;
    bool m_options[7];
    CBankProcessor() { }
    ~CBankProcessor() { }
};
int main()
{
    { CBankProcessor p; }
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("00c2e140_TEST FAIL\n"); return 1; }
    std::printf("00c2e140_TEST PASS\n");
    return 0;
}
