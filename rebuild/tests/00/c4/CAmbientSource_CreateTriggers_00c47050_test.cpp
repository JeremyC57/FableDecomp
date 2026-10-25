// Behaviour test for 00c47050: CAmbientSource::CreateTriggers -- one new trigger per definition trigger id, each scheduled
#include <cstdio>

struct CAmbientSource;
static unsigned int g_ids[8]; static int g_count = 0;
struct CAmbientTrigger {
    unsigned int m_soundId; int m_handle;
    CAmbientTrigger(unsigned int id) : m_soundId(id), m_handle(-1) {}
    void Schedule(CAmbientSource*) { g_ids[g_count++] = m_soundId; delete this; }
};
struct CIdArray { unsigned int* m_data; unsigned int m_capacity; unsigned int m_count;
    unsigned int GetAt(unsigned int i) const { return m_data[i]; } };
struct CAmbientSourceData { CIdArray m_channelIds; CIdArray m_triggerIds; };
struct CAmbientSourceDef { CAmbientSourceData* m_pData; };
struct CAmbientSource {
    CAmbientSourceDef* m_pDef;
    void CreateTriggers()
    {
        CIdArray& ids = m_pDef->m_pData->m_triggerIds;
        for (unsigned int i = 0; i < ids.m_count; ++i) {
            CAmbientTrigger* trigger = new CAmbientTrigger(ids.GetAt(i));
            trigger->Schedule(this);
        }
    }
};

int main()
{
    unsigned int channelIds[1] = { 9 }, triggerIds[3] = { 11, 22, 33 };
    CAmbientSourceData data = { { channelIds, 1, 1 }, { triggerIds, 3, 3 } };
    CAmbientSourceDef def = { &data };
    CAmbientSource s; s.m_pDef = &def;
    s.CreateTriggers();
    int ok = g_count == 3 && g_ids[0] == 11 && g_ids[1] == 22 && g_ids[2] == 33;
    data.m_triggerIds.m_count = 0;
    s.CreateTriggers();
    ok &= g_count == 3;
    if (!ok) { std::printf("00c47050_TEST FAIL\n"); return 1; }
    std::printf("00c47050_TEST PASS\n");
    return 0;
}