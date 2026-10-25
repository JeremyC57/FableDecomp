// Behaviour test for 00c44b80: CAmbientEmitter::ReleaseSounds -- pop every sound from the list and release it to the bank
#include <cstdio>

struct CSoundResource { int m_refs; CSoundResource* m_next; };
static int g_released = 0;
struct CSoundBank { void Release(CSoundResource* s) { --s->m_refs; ++g_released; } };
struct CAmbientScene { CSoundBank* m_pBank; };
struct CAmbientZone { CAmbientScene* m_pScene; };
struct CList {
    CSoundResource* m_head;
    CSoundResource* GetFirst() { return m_head; }
    void RemoveItem(CSoundResource* s) { if (m_head == s) m_head = s->m_next; }
};
struct CAmbientEmitter {
    CAmbientZone* m_pZone;
    CList m_sounds;
    CSoundBank* GetBank() const { return m_pZone->m_pScene->m_pBank; }
    void ReleaseSounds()
    {
        CSoundResource* sound;
        while ((sound = m_sounds.GetFirst()) != 0) {
            m_sounds.RemoveItem(sound);
            GetBank()->Release(sound);
        }
    }
};

int main()
{
    CSoundBank bank; CAmbientScene scene = { &bank }; CAmbientZone zone = { &scene };
    CSoundResource c = { 1, 0 }, b = { 2, &c }, a = { 1, &b };
    CAmbientEmitter e; e.m_pZone = &zone; e.m_sounds.m_head = &a;
    e.ReleaseSounds();
    int ok = e.m_sounds.m_head == 0 && g_released == 3 && a.m_refs == 0 && b.m_refs == 1 && c.m_refs == 0;
    e.ReleaseSounds();
    ok &= g_released == 3;
    if (!ok) { std::printf("00c44b80_TEST FAIL\n"); return 1; }
    std::printf("00c44b80_TEST PASS\n");
    return 0;
}