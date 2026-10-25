// Behaviour test for 00c47300: CAmbientSource::Update -- channels, due triggers, triggers while the set is active, finish when done
#include <cstdio>

static char g_log[16]; static int g_len = 0;
struct CAmbientSet { bool m_bActive; };
struct CAmbientSourceDef { bool m_bDone; bool IsFinished() const { return m_bDone; } };
struct CAmbientSource {
    CAmbientSet* m_pSet;
    CAmbientSourceDef* m_pDef;
    void UpdateChannels() { g_log[g_len++] = 'c'; }
    void ProcessTriggers() { g_log[g_len++] = 'p'; }
    void UpdateTriggers() { g_log[g_len++] = 't'; }
    void Finish() { g_log[g_len++] = 'f'; }
    void Update()
    {
        UpdateChannels();
        ProcessTriggers();
        if (m_pSet->m_bActive)
            UpdateTriggers();
        if (m_pDef->IsFinished())
            Finish();
    }
};

static int Is(const char* want)
{
    int i = 0;
    for (; want[i]; ++i) if (i >= g_len || g_log[i] != want[i]) return 0;
    int ok = i == g_len;
    g_len = 0;
    return ok;
}

int main()
{
    CAmbientSet set = { true }; CAmbientSourceDef def = { false };
    CAmbientSource s; s.m_pSet = &set; s.m_pDef = &def;
    s.Update();
    int ok = Is("cpt");
    set.m_bActive = false; def.m_bDone = true;
    s.Update();
    ok &= Is("cpf");
    set.m_bActive = true;
    s.Update();
    ok &= Is("cptf");
    if (!ok) { std::printf("00c47300_TEST FAIL\n"); return 1; }
    std::printf("00c47300_TEST PASS\n");
    return 0;
}