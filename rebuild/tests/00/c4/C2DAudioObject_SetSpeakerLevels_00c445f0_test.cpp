// Behaviour test for 0x00C445F0 C2DAudioObject::SetSpeakerLevels
#include <cstdio>
struct E { unsigned int m_speaker; int m_level; };
struct Tab { int m_count; E* m_pEntries; };
struct Obj {
    unsigned int m_dirtyFlags; Tab* m_pLevels;
    void SetSpeakerLevels(const Tab* levels)
    {
        unsigned int pending = m_dirtyFlags & 0xa02;
        if (pending) m_dirtyFlags = pending | 0x400;
        for (int i = 0; i < levels->m_count; ++i) {
            int j = 0;
            for (; j < m_pLevels->m_count; ++j) {
                if (m_pLevels->m_pEntries[j].m_speaker == levels->m_pEntries[i].m_speaker) break;
                m_pLevels->m_pEntries[j].m_level = -10000;
            }
            int* level = &m_pLevels->m_pEntries[j].m_level;
            if (*level != levels->m_pEntries[i].m_level) { *level = levels->m_pEntries[i].m_level; m_dirtyFlags |= 0x400; }
        }
    }
};
static int Fail() { std::printf("00c445f0_TEST FAIL\n"); return 1; }
int main()
{
    E own[3] = { { 1, 0 }, { 2, 0 }, { 4, 0 } }; Tab ownT = { 3, own };
    E in[1] = { { 4, -300 } }; Tab inT = { 1, in };
    Obj o; o.m_dirtyFlags = 0x1001; o.m_pLevels = &ownT;
    o.SetSpeakerLevels(&inT);
    // speakers before the match are floored to -10000 mB; bits outside 0xa02 survive only when nothing was pending
    if (own[0].m_level != -10000 || own[1].m_level != -10000 || own[2].m_level != -300 || o.m_dirtyFlags != 0x1401) return Fail();
    o.m_dirtyFlags = 0x1202; o.SetSpeakerLevels(&inT);
    if (o.m_dirtyFlags != 0x602) return Fail();                    // pending bits keep only 0xa02 and gain 0x400
    std::printf("00c445f0_TEST PASS\n");
    return 0;
}