// Behaviour test for CInstanceManager::StartIdleInstances @ 0x00C2D160 (copy of the manager logic; the 0x180-byte instances are fakes).
#include <cstdio>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class CRampedParam {
public:
    float m_value, m_target, m_time;
    bool  m_bAngle;
    void RampTo(float target, float time) { m_target = target; m_time = time; }
    float GetValue() const
    {
        float value = m_value;
        if (m_bAngle) {
            if (value >= 3.14159265f)
                return value - 6.28318531f;
            if (value < -3.14159265f)
                return value + 6.28318531f;
        }
        return value;
    }
};

class CInstance {                          // fake: state 0 idle, 5 started, 6 finished; Update counts and may go idle
public:
    int  m_state, m_updates, m_idleAfterUpdate;
    long m_lastA, m_lastB, m_lastC, m_info170, m_info174, m_info178;
    CRampedParam m_volume;
    bool IsIdle() const { return m_state == 0; }
    bool IsFinished() const { return m_state == 6; }
    void Start() { m_state = 5; }
    void Update(long a, long b, long c)
    {
        ++m_updates; m_lastA = a; m_lastB = b; m_lastC = c;
        if (m_idleAfterUpdate && m_updates == m_idleAfterUpdate) m_state = 0;
    }
    long GetInfo170() const { return m_info170; }
    long GetInfo174() const { return m_info174; }
    long GetInfo178() const { return m_info178; }
    void Reset(int state)
    {
        m_state = state; m_updates = 0; m_idleAfterUpdate = 0; m_lastA = m_lastB = m_lastC = 0;
        m_info170 = m_info174 = m_info178 = 0;
        m_volume.m_value = 0.0f; m_volume.m_target = -1.0f; m_volume.m_time = -1.0f; m_volume.m_bAngle = false;
    }
};

struct SThemeRequest { long m_words[7]; };

class CInstanceManager {
public:
    long          m_updateA, m_updateB, m_updateC;
    CInstance     m_instances[2];
    long          m_current;
    SThemeRequest m_pending;
    bool          m_bPending;
    int           m_used;
    unsigned int  m_usedIndex;
    SThemeRequest* m_usedRequest;

    void UseInstance(unsigned int index, SThemeRequest* request)
    {
        ++m_used; m_usedIndex = index; m_usedRequest = request; m_instances[index].m_state = 5;
    }
    CInstance* GetCurrentInstance()
    {
        if (m_bPending)
            return 0;
        return &m_instances[m_current];
    }
    bool StartIdleInstances()
    {
        bool allFinished = true;
        for (int i = 0; i < 2; ++i) {
            if (m_instances[i].IsIdle())
                m_instances[i].Start();
            if (!m_instances[i].IsFinished())
                allFinished = false;
        }
        return allFinished;
    }
    void FadeOutAll(float time)
    {
        m_bPending = false;
        for (int i = 0; i < 2; ++i) {
            if (!m_instances[i].IsIdle())
                m_instances[i].m_volume.RampTo(0.0f, time);
        }
    }
    int GetCurrentInfo(long* a, long* b, long* c)
    {
        CInstance* instance = GetCurrentInstance();
        if (!instance) {
            *c = -1;
            *a = -1;
            *b = 0;
            if (!m_bPending)
                return 0;
        } else {
            *a = instance->GetInfo174();
            *c = instance->GetInfo170();
            *b = instance->GetInfo178();
        }
        return 1;
    }
    void Update()
    {
        for (unsigned int i = 0; i < 2; ++i) {
            CInstance& instance = m_instances[i];
            if (!instance.IsIdle()) {
                instance.Update(m_updateA, m_updateB, m_updateC);
                if (instance.IsIdle() && m_bPending) {
                    UseInstance(i, &m_pending);
                    m_current = i;
                    instance.Update(m_updateA, m_updateB, m_updateC);
                    m_bPending = false;
                }
            }
        }
    }
    CInstance* FindQuietestInstance()
    {
        CInstance* best = 0;
        for (int i = 0; i < 2; ++i) {
            CInstance* instance = &m_instances[i];
            if (!instance->IsIdle()) {
                if (!best || instance->m_volume.GetValue() < best->m_volume.GetValue())
                    best = instance;
            }
        }
        return best;
    }
    void Reset(int s0, int s1)
    {
        m_updateA = 1; m_updateB = 2; m_updateC = 3; m_current = 0; m_bPending = false;
        m_used = 0; m_usedIndex = 99; m_usedRequest = 0;
        m_instances[0].Reset(s0); m_instances[1].Reset(s1);
    }
};

int main()
{
    bool ok = true;
    CInstanceManager m;
    m.Reset(0, 6);
    if (m.StartIdleInstances()) ok = false;                // the idle one is started, it is not finished
    if (m.m_instances[0].m_state != 5 || m.m_instances[1].m_state != 6) ok = false;
    m.Reset(6, 6);
    if (!m.StartIdleInstances()) ok = false;               // both finished
    m.Reset(0, 0);
    if (m.StartIdleInstances() || m.m_instances[0].m_state != 5 || m.m_instances[1].m_state != 5) ok = false;
    if (!ok) { std::printf("00c2d160_TEST FAIL\n"); return 1; }
    std::printf("00c2d160_TEST PASS\n");
    return 0;
}