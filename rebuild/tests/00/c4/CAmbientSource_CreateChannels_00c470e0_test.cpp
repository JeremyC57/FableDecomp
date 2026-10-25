// Behaviour test for 00c470e0: CAmbientSource::CreateChannels -- one new channel {id, -1} per definition channel id, added to the channel list
#include <cstdio>

struct CAmbientChannel {
    unsigned int m_soundId; int m_handle;
    CAmbientChannel(unsigned int id) : m_soundId(id), m_handle(-1) {}
};
struct CList {
    CAmbientChannel* m_items[8]; int m_count;
    void AddItem(CAmbientChannel* c) { m_items[m_count++] = c; }
};
struct CIdArray { unsigned int* m_data; unsigned int m_capacity; unsigned int m_count;
    unsigned int GetAt(unsigned int i) const { return m_data[i]; } };
struct CAmbientSourceData { CIdArray m_channelIds; CIdArray m_triggerIds; };
struct CAmbientSourceDef { CAmbientSourceData* m_pData; };
struct CAmbientSource {
    CAmbientSourceDef* m_pDef;
    CList m_channels;
    void CreateChannels()
    {
        CIdArray& ids = m_pDef->m_pData->m_channelIds;
        for (unsigned int i = 0; i < ids.m_count; ++i)
            m_channels.AddItem(new CAmbientChannel(ids.GetAt(i)));
    }
};

int main()
{
    unsigned int channelIds[2] = { 7, 8 }, triggerIds[1] = { 1 };
    CAmbientSourceData data = { { channelIds, 2, 2 }, { triggerIds, 1, 1 } };
    CAmbientSourceDef def = { &data };
    CAmbientSource s; s.m_pDef = &def; s.m_channels.m_count = 0;
    s.CreateChannels();
    int ok = s.m_channels.m_count == 2 && s.m_channels.m_items[0]->m_soundId == 7 && s.m_channels.m_items[1]->m_soundId == 8
          && s.m_channels.m_items[0]->m_handle == -1 && s.m_channels.m_items[1]->m_handle == -1;
    for (int i = 0; i < s.m_channels.m_count; ++i) delete s.m_channels.m_items[i];
    if (!ok) { std::printf("00c470e0_TEST FAIL\n"); return 1; }
    std::printf("00c470e0_TEST PASS\n");
    return 0;
}