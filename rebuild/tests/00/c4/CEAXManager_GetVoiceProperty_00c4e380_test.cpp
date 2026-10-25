#include <cstdio>
static int g_gets = 0; static unsigned int g_lastId = 0, g_lastSize = 0;
struct CEAXEffect {
    int status;
    int GetProperty(unsigned int id, void* data, unsigned int size, unsigned int* returned)
    { ++g_gets; g_lastId = id; g_lastSize = size; *returned = size; *(int*)data = 42; return status; }
};
struct CEAXVoice { CEAXEffect* m_pEffect; };
struct CEAXManager {
    int GetVoiceProperty(unsigned int id, void* data, unsigned int size, CEAXVoice* voice)
    {
        int result = -4;
        if (voice && voice->m_pEffect) {
            unsigned int returned;
            result = voice->m_pEffect->GetProperty(id, data, size, &returned);
        }
        return result;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c4e380_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CEAXManager m; int data = 0; CEAXEffect fx; fx.status = 0; CEAXVoice v; v.m_pEffect = 0;
    CHECK(m.GetVoiceProperty(3, &data, 4, 0) == -4 && g_gets == 0);
    CHECK(m.GetVoiceProperty(3, &data, 4, &v) == -4 && g_gets == 0);
    v.m_pEffect = &fx;
    CHECK(m.GetVoiceProperty(0x80000003u, &data, 4, &v) == 0 && g_gets == 1 && g_lastId == 0x80000003u && g_lastSize == 4 && data == 42);
    fx.status = -2;
    CHECK(m.GetVoiceProperty(5, &data, 8, &v) == -2 && g_gets == 2);
    std::printf("00c4e380_TEST PASS\n");
    return 0;
}