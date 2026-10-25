// Behaviour test for 00c44ce0: CAmbientEmitter::AcquireSounds -- acquire every sound id of the definition and add it to the list
#include <cstdio>

struct CSoundResource { unsigned int m_id; };
static CSoundResource g_pool[8];
struct CSoundBank { CSoundResource* Acquire(unsigned int id) { g_pool[id].m_id = id; return &g_pool[id]; } };
struct CAmbientScene { CSoundBank* m_pBank; };
struct CAmbientZone { CAmbientScene* m_pScene; };
struct CPtrArray { unsigned int* m_data; unsigned int m_capacity; unsigned int m_count;
    unsigned int GetAt(unsigned int i) const { return m_data[i]; } };
struct CSoundSet { unsigned int m_header[2]; CPtrArray m_soundIds; };
struct CAmbientDef { unsigned int m_words[2]; CSoundSet* m_pSounds; };
struct CList {
    CSoundResource* m_items[8]; int m_count;
    void AddItem(CSoundResource* s) { m_items[m_count++] = s; }
};
struct CAmbientEmitter {
    CAmbientZone* m_pZone; CAmbientDef* m_pDef; CList m_sounds;
    CSoundBank* GetBank() const { return m_pZone->m_pScene->m_pBank; }
    void AcquireSounds()
    {
        CPtrArray& ids = m_pDef->m_pSounds->m_soundIds;
        for (unsigned int i = 0; i < ids.m_count; ++i)
            m_sounds.AddItem(GetBank()->Acquire(ids.GetAt(i)));
    }
};

int main()
{
    CSoundBank bank; CAmbientScene scene = { &bank }; CAmbientZone zone = { &scene };
    unsigned int ids[3] = { 5, 2, 7 };
    CSoundSet set = { { 0, 0 }, { ids, 3, 3 } };
    CAmbientDef def = { { 0, 0 }, &set };
    CAmbientEmitter e; e.m_pZone = &zone; e.m_pDef = &def; e.m_sounds.m_count = 0;
    e.AcquireSounds();
    int ok = e.m_sounds.m_count == 3 && e.m_sounds.m_items[0] == &g_pool[5] && e.m_sounds.m_items[2] == &g_pool[7] && g_pool[2].m_id == 2;
    set.m_soundIds.m_count = 0;
    e.AcquireSounds();
    ok &= e.m_sounds.m_count == 3;
    if (!ok) { std::printf("00c44ce0_TEST FAIL\n"); return 1; }
    std::printf("00c44ce0_TEST PASS\n");
    return 0;
}