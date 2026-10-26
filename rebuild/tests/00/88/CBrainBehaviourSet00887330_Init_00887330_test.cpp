#include <stdio.h>
#include <string.h>
class CBrainBehaviour { public: long Name; float Priority; long ScriptAIPriority; CBrainBehaviour() { memset(this, 0, sizeof(*this)); } };
static CBrainBehaviour g_storage[4]; static unsigned long g_count; static CBrainBehaviour g_seen;
struct CRetailVector_CBrainBehaviour {
    CBrainBehaviour* first; CBrainBehaviour* last; CBrainBehaviour* end;
    void resize(unsigned long count, const CBrainBehaviour& value) { g_count = count; g_seen = value; first = g_storage; last = g_storage + count; end = g_storage + 4; for (unsigned long i = 0; i < count; ++i) g_storage[i] = value; }
};
struct CBrainBehaviourSet_00887330 {
    long m_current; long m_04; CRetailVector_CBrainBehaviour m_behaviours; unsigned char _pad[8]; bool m_dirty;
    void Init(long name);
};
void CBrainBehaviourSet_00887330::Init(long name)
{
    CBrainBehaviour value;
    m_behaviours.resize(1, value);
    m_behaviours.first->Name = name;
    m_behaviours.first->Priority = 1.0f;
    m_current = 0;
    m_dirty = false;
}
int main()
{
    CBrainBehaviourSet_00887330 s; s.m_current = 99; s.m_dirty = true; s.m_behaviours.first = 0;
    s.Init(0x5150);
    bool ok = g_count == 1 && g_seen.Name == 0 && g_seen.Priority == 0.0f && g_seen.ScriptAIPriority == 0 && g_storage[0].Name == 0x5150 && g_storage[0].Priority == 1.0f && g_storage[0].ScriptAIPriority == 0 && s.m_current == 0 && !s.m_dirty;
    if (!ok) { printf("00887330_TEST FAIL\n"); return 1; }
    printf("00887330_TEST PASS\n"); return 0;
}