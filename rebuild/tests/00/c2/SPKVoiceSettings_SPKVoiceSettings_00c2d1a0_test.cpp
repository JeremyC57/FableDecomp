#include <cstdio>
#include <cstring>
#include <new>
// Behaviour model of SPKVoiceSettings::SPKVoiceSettings() @ 0x00C2D1A0 (libpk, descriptive name): id 0, the reference
// constructed by SPKReference() (stand-in), enabled 1, priority 0, delay 0, gain 1.0f.
static int gRefCtorCalls = 0;
struct SPKReference {
    void* m_pObject;
    void* m_pOwner;
    SPKReference() : m_pObject(0), m_pOwner(0) { ++gRefCtorCalls; }   // stand-in for 0x00C167B0
};
struct SPKVoiceSettings {
    unsigned int m_id;
    SPKReference m_reference;
    int          m_enabled;
    int          m_priority;
    int          m_delay;
    float        m_gain;
    SPKVoiceSettings();
};
SPKVoiceSettings::SPKVoiceSettings() : m_id(0), m_enabled(1), m_priority(0), m_delay(0), m_gain(1.0f)
{
}
int main()
{
    union { double align; unsigned char bytes[sizeof(SPKVoiceSettings)]; } buf;
    std::memset(buf.bytes, 0x5A, sizeof(buf.bytes));
    SPKVoiceSettings* v = new (buf.bytes) SPKVoiceSettings;
    float d = v->m_gain - 1.0f;
    bool ok = v->m_id == 0 && v->m_enabled == 1 && v->m_priority == 0 && v->m_delay == 0 && d < 1e-6f && d > -1e-6f &&
              gRefCtorCalls == 1 && v->m_reference.m_pObject == 0;
    if (!ok) { std::printf("00c2d1a0_TEST FAIL\n"); return 1; }
    std::printf("00c2d1a0_TEST PASS\n");
    return 0;
}