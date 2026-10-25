// Behaviour test for 00c59820: CCodaVoice::GetEAXProperty -- forwards (id, data, size, EAX voice) when EAX is supported, else -9
#include <cstdio>
struct CEAXVoice { int tag; };
static unsigned long g_id = 0; static void* g_data = 0; static unsigned long g_size = 0; static CEAXVoice* g_voice = 0;
class CEAXManager {
public:
    long GetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice) { g_id = id; g_data = data; g_size = size; g_voice = voice; return 7; }
};
static CEAXManager g_manager; static CEAXManager* g_pEAXManager = &g_manager;
class CCodaVoice {
public:
    bool m_eax; CEAXVoice* m_pEAXVoice;
    virtual bool SupportsEAX() { return m_eax; }
    long GetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size)
    {
        if (SupportsEAX())
            return g_pEAXManager->GetVoiceProperty(id, data, size, m_pEAXVoice);
        return -9;
    }
};
int main()
{
    CEAXVoice ev; int value = 0; CCodaVoice v; v.m_eax = false; v.m_pEAXVoice = &ev;
    if (v.GetEAXProperty(0, 3, &value, 4) != -9 || g_voice) { std::printf("00c59820_TEST FAIL\n"); return 1; }
    v.m_eax = true;
    if (v.GetEAXProperty(0, 3, &value, 4) != 7 || g_id != 3 || g_data != &value || g_size != 4 || g_voice != &ev) { std::printf("00c59820_TEST FAIL\n"); return 1; }
    std::printf("00c59820_TEST PASS\n");
    return 0;
}
