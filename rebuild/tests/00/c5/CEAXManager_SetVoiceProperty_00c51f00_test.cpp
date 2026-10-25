#include <cstdio>
static int g_sets = 0, g_commits = 0; static unsigned int g_lastDirty = 0; static void* g_lastVoice = 0;
struct CEAXEffect {
    int status; unsigned int dirtyBits;
    int SetProperty(unsigned int id, void* data, unsigned int size, unsigned int* dirty)
    { (void)id; (void)data; (void)size; ++g_sets; *dirty = dirtyBits; return status; }
};
struct CEAXVoice { CEAXEffect* m_pEffect; };
struct CEAXManager {
    void QueueCommit(unsigned int dirty, CEAXVoice* voice) { ++g_commits; g_lastDirty = dirty; g_lastVoice = voice; }
    int SetVoiceProperty(unsigned int id, void* data, unsigned int size, CEAXVoice* voice)
    {
        int result = -4;
        if (voice) {
            unsigned int dirty;
            result = voice->m_pEffect->SetProperty(id, data, size, &dirty);
            if (!result)
                QueueCommit(dirty, voice);
        }
        return result;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c51f00_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CEAXManager m; int data = 1; CEAXEffect fx; fx.status = 0; fx.dirtyBits = 0x24; CEAXVoice v; v.m_pEffect = &fx;
    CHECK(m.SetVoiceProperty(1, &data, 4, 0) == -4 && g_sets == 0 && g_commits == 0);
    CHECK(m.SetVoiceProperty(1, &data, 4, &v) == 0 && g_sets == 1 && g_commits == 1 && g_lastDirty == 0x24 && g_lastVoice == &v);
    fx.status = -3;
    CHECK(m.SetVoiceProperty(2, &data, 4, &v) == -3 && g_sets == 2 && g_commits == 1);
    std::printf("00c51f00_TEST PASS\n");
    return 0;
}