// Behaviour test for 0x006BD8A0 CMusicManager::UpdateCurrentMusicOffset().
#include <cstdio>

struct CSoundSystem {
    bool  m_musicEnabled;
    bool  m_hasPosition;
    float m_position;
    int   m_queries;
    virtual bool GetMusicPosition(float* seconds) { ++m_queries; if (!m_hasPosition) return false; *seconds = m_position; return true; }
    virtual ~CSoundSystem() { }
};
static CSoundSystem* g_pSoundSystem = 0;

struct CRegisteredMusicEntry { void* Src; void* Dst; float Gain; float Offset; bool CacheToHD; };
struct Pair { int first; CRegisteredMusicEntry second; };

struct CRegisteredMusicMap {
    Pair* First;
    Pair* Last;
    Pair* LowerBound(const int& key)
    {
        Pair* first = First;
        long count = (long)(Last - First);
        while (count > 0) {
            long half = count >> 1;
            if (first[half].first < key) { first += half + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    Pair* Find(const int& key) { Pair* it = LowerBound(key); return (it == Last || key < it->first) ? Last : it; }
    Pair* End() { return Last; }
};

class CMusicManager { public:
    CRegisteredMusicMap RegisteredMusic;
    int CurrentMusicID;
    void UpdateCurrentMusicOffset()
    {
        CSoundSystem* sound = g_pSoundSystem;
        if (sound && sound->m_musicEnabled) {
            Pair* it = RegisteredMusic.Find(CurrentMusicID);
            if (it != RegisteredMusic.End()) {
                float offset;
                if (sound->GetMusicPosition(&offset))
                    it->second.Offset = offset;
            }
        }
    }
};

int main()
{
    Pair music[3];
    int ids[3] = { -5, 2, 40 };
    for (int i = 0; i < 3; ++i) { music[i].first = ids[i]; music[i].second.Offset = 0.0f; }
    CMusicManager m; m.RegisteredMusic.First = music; m.RegisteredMusic.Last = music + 3; m.CurrentMusicID = 2;
    m.UpdateCurrentMusicOffset();                       // no sound system: nothing
    int ok = music[1].second.Offset == 0.0f;
    CSoundSystem s; s.m_musicEnabled = false; s.m_hasPosition = true; s.m_position = 12.5f; s.m_queries = 0;
    g_pSoundSystem = &s;
    m.UpdateCurrentMusicOffset();                       // music disabled: nothing
    if (music[1].second.Offset != 0.0f || s.m_queries != 0) ok = 0;
    s.m_musicEnabled = true;
    m.UpdateCurrentMusicOffset();
    if (music[1].second.Offset != 12.5f || s.m_queries != 1) ok = 0;
    m.CurrentMusicID = 3;                               // unregistered id: no query
    m.UpdateCurrentMusicOffset();
    if (s.m_queries != 1) ok = 0;
    m.CurrentMusicID = -5; s.m_hasPosition = false;     // no position: entry untouched
    m.UpdateCurrentMusicOffset();
    if (s.m_queries != 2 || music[0].second.Offset != 0.0f) ok = 0;
    if (!ok) { std::printf("006bd8a0_TEST FAIL\n"); return 1; }
    std::printf("006bd8a0_TEST PASS\n");
    return 0;
}