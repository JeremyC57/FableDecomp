// Behaviour test for 00c20880: CSoundEvent::WaitForMemory -- state 1 -> 4, joins the waiting-for-memory list and starts its virtual player (sample, level, pitch)
#include <cstdio>

struct CSoundEventManager; struct CSoundEvent;
static int g_checks = 0, g_badState = 0; static long g_lastExpected = -99;

struct SEventLevel { float m_gain; long m_muted; };
struct SEventRecord {
    int m_a, m_b, m_c; unsigned char m_flag;
    SEventRecord() : m_a(0), m_b(0), m_c(0), m_flag(0) { }
};
struct SPlayDesc {
    long m_mode; SEventRecord m_record; float m_unused14[2]; float m_distance; float m_minDistance; float m_maxDistance;
    SPlayDesc() : m_mode(0), m_record(), m_minDistance(0.0f), m_maxDistance(0.0f) { m_distance = -5.0f; }
};
struct CEventList {
    CSoundEvent* m_items[4]; int m_count;
    CEventList() : m_count(0) { }
    void Remove(CSoundEvent* v) { for (int i = 0; i < m_count; ++i) if (m_items[i] == v) { m_items[i] = m_items[--m_count]; return; } }
    void Add(CSoundEvent* v) { m_items[m_count++] = v; }
    bool Has(CSoundEvent* v) const { for (int i = 0; i < m_count; ++i) if (m_items[i] == v) return true; return false; }
};
struct CSoundEventManager { CEventList m_waitingForData, m_waitingForPlayer, m_waitingForMemory; };
struct CSoundPlayer {
    SEventLevel m_outLevel; int m_reads, m_updates; CSoundEventManager* m_readMixer;
    CSoundPlayer() : m_reads(0), m_updates(0), m_readMixer(0) { m_outLevel.m_gain = 0.25f; m_outLevel.m_muted = 0; }
    virtual ~CSoundPlayer() { }
    virtual void ReadLevel(CSoundEventManager* manager, SEventLevel* level) { ++m_reads; m_readMixer = manager; *level = m_outLevel; }
    virtual void Update() { ++m_updates; }
};
struct CVirtualPlayer : CSoundPlayer {
    long m_startSample; SEventLevel* m_startLevel; float m_startPitch; int m_starts;
    CVirtualPlayer() : m_startSample(0), m_startLevel(0), m_startPitch(0.0f), m_starts(0) { }
    void Start(long sample, SEventLevel* level, float pitch) { ++m_starts; m_startSample = sample; m_startLevel = level; m_startPitch = pitch; }
};
struct CPhysicalPlayer : CSoundPlayer {
    int m_detaches, m_plays; SPlayDesc m_desc; CSoundEvent* m_voice; SEventLevel* m_playLevel; float m_pitch, m_gain;
    CPhysicalPlayer() : m_detaches(0), m_plays(0), m_voice(0), m_playLevel(0), m_pitch(0.0f), m_gain(0.0f) { }
    void Detach(CSoundEventManager*) { ++m_detaches; }
    void Play(CSoundEventManager*, SPlayDesc* desc, CSoundEvent* event, SEventLevel* level, float pitch, float gain)
    { ++m_plays; m_desc = *desc; m_voice = event; m_playLevel = level; m_pitch = pitch; m_gain = gain; }
};
struct CSoundChannel {
    long m_priority; int m_starts, m_stops;
    CSoundChannel() : m_priority(0), m_starts(0), m_stops(0) { }
    void Start(CSoundEventManager*, long priority) { ++m_starts; m_priority = priority; }
    void Stop(CSoundEventManager*, long priority) { ++m_stops; m_priority = priority; }
};
struct CSampleRef { long m_sample; long GetSample() { return m_sample; } };
struct CSoundParams {
    long m_mode; float m_pitch, m_gain, m_distance; SEventRecord m_record; long m_ma, m_mb;
    float GetPitch(CSoundEventManager*) { return m_pitch; }
    float GetGain(CSoundEventManager*) { return m_gain; }
    bool Matches(long a, long b) { return a == m_ma && b == m_mb; }
    float GetDistance() { return m_distance; }
    SEventRecord GetRecord() const { return m_record; }
};

struct CSoundEvent {
    SEventLevel m_level; long m_priority; CSampleRef m_sample; CSoundParams m_params; CVirtualPlayer m_virtualPlayer;
    CPhysicalPlayer* m_pPhysicalPlayer; CSoundChannel* m_pChannel; unsigned int m_dirty; unsigned short m_state; bool m_listening;
    int m_stopListening; long m_setPriority;
    CSoundEvent() : m_priority(3), m_pPhysicalPlayer(0), m_pChannel(0), m_dirty(0), m_state(1), m_listening(false), m_stopListening(0), m_setPriority(0)
    {
        m_level.m_gain = 0.5f; m_level.m_muted = 0; m_sample.m_sample = 77;
        m_params.m_mode = 0; m_params.m_pitch = 1.5f; m_params.m_gain = 0.75f; m_params.m_distance = 12.0f;
        m_params.m_record.m_a = 1; m_params.m_record.m_b = 2; m_params.m_record.m_c = 3; m_params.m_record.m_flag = 1;
        m_params.m_ma = 10; m_params.m_mb = 20;
    }
    void CheckState(long expected) { ++g_checks; g_lastExpected = expected; if (expected != -1 && expected != (long)m_state) ++g_badState; }
    void SetPriority(CSoundEventManager*, long priority) { m_setPriority = priority; m_priority = priority; }
    void StopListening(CSoundEventManager*) { ++m_stopListening; m_listening = false; }
    void WaitForMemory(CSoundEventManager* manager)
    {
        CheckState(1);
        manager->m_waitingForMemory.Add(this);
        long sample = m_sample.GetSample();
        m_virtualPlayer.Start(sample, &m_level, m_params.GetPitch(manager));
        m_state = 4;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
#define CHECK(c) do { if (!(c)) { std::printf("00c20880_TEST FAIL\n"); return 1; } } while (0)

int main()
{
    CSoundEvent v; CSoundEventManager m;
    v.WaitForMemory(&m);
    CHECK(v.m_state == 4 && m.m_waitingForMemory.Has(&v) && m.m_waitingForData.m_count == 0);
    CHECK(v.m_virtualPlayer.m_starts == 1 && v.m_virtualPlayer.m_startSample == 77 && v.m_virtualPlayer.m_startLevel == &v.m_level && Near(v.m_virtualPlayer.m_startPitch, 1.5f));
    CHECK(g_lastExpected == 1 && g_badState == 0);
    std::printf("00c20880_TEST PASS\n");
    return 0;
}