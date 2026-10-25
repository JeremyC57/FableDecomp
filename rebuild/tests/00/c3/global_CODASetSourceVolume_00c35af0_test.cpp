#include <cstdio>
static void* g_self; static float g_gain;
class CAudioObject { public: float m_gain; void SetGain(float gain) { g_self = this; g_gain = gain; m_gain = gain; } };
extern "C" void __stdcall CODA_SetSourceVolume(CAudioObject* source, float volume) { source->SetGain(volume); }
int main()
{
    CAudioObject o; o.m_gain = 0.0f;
    CODA_SetSourceVolume(&o, 0.5f);
    if (g_self != &o || g_gain != 0.5f || o.m_gain != 0.5f) { std::printf("00c35af0_TEST FAIL\n"); return 1; }
    std::printf("00c35af0_TEST PASS\n");
    return 0;
}
