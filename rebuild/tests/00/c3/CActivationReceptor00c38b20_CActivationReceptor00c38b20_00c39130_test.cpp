// Behaviour test for the CActivationReceptor_00c38b20 constructor (0x00C39130): construction order of the five
// head+entry parts (each entry built from its own part and 1), the state word, registry and interpolated float, and
// the tail words, through a copy of the model with logging constructors.
#include <cstdio>
#include <cstring>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct Head { void* a; void* b; Head(char c) : a(0), b(0) { Log(c); } };
struct Entry { void* owner; void* flag; Entry(char c, void* o, void* f) : owner(o), flag(f) { Log(c); } };
struct Part {
    Head m_head; Entry m_entry;
    Part(char h, char e) : m_head(h), m_entry(e, this, (void*)1) {}
};
struct Registry { int x; Registry() : x(7) { Log('R'); } };
struct InterpolatedFloat { float cur, target; InterpolatedFloat(float t, float) : cur(0.0f), target(t) { Log('F'); } };
struct Receptor {
    Part m_base, m_2c, m_60, m_94, m_cc;
    long m_state; Registry m_registry; InterpolatedFloat m_138; void* m_pSink;
    long m_170, m_174, m_178; float m_scale;
    Receptor() : m_base('a', 'A'), m_2c('b', 'B'), m_60('c', 'C'), m_94('d', 'D'), m_cc('e', 'E'), m_state(0), m_138(0.0f, 0.0f)
    {
        m_170 = -1; m_174 = -1; m_178 = -1; m_pSink = 0; m_scale = 1.0f;
    }
};
int main()
{
    Receptor r;
    bool ok = std::strcmp(g_log, "aAbBcCdDeERF") == 0;
    ok = ok && r.m_2c.m_entry.owner == &r.m_2c && r.m_cc.m_entry.flag == (void*)1 && r.m_base.m_entry.owner == &r.m_base;
    ok = ok && r.m_state == 0 && r.m_170 == -1 && r.m_174 == -1 && r.m_178 == -1 && r.m_pSink == 0 && r.m_scale == 1.0f;
    ok = ok && r.m_138.target == 0.0f && r.m_registry.x == 7;
    if (!ok) { std::printf("00c39130_TEST FAIL\n"); return 1; }
    std::printf("00c39130_TEST PASS\n");
    return 0;
}