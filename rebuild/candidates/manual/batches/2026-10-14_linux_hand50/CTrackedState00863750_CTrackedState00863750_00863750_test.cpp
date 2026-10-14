#include <cstdio>
#include <cstring>
#include <new>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static unsigned char g_trackingDisabled = 0;
struct STrackWords { long m_value18; long m_value1c; long m_value20; STrackWords() : m_value18(0), m_value1c(0), m_value20(0) { } };
struct CTrackedState { bool m_enabled; bool m_started; unsigned char _pad02[2]; long m_value04; bool m_flag08; bool m_flag09; unsigned char _pad0a[2]; long m_value0c; long m_value10; bool m_flag14; unsigned char _pad15[3]; STrackWords m_words; void* m_pOwner;
    CTrackedState(void* pOwner) : m_words() { m_pOwner = pOwner; m_enabled = g_trackingDisabled == 0; m_started = false; m_flag08 = false; m_value0c = 0; m_flag14 = false; m_flag09 = false; m_value04 = 0; m_value10 = 0; } };
int main()
{
    int owner; unsigned char raw[sizeof(CTrackedState)]; std::memset(raw, 0xee, sizeof raw); CTrackedState* s = new (raw) CTrackedState(&owner);
    if (!s->m_enabled || s->m_started || s->m_value04 || s->m_flag08 || s->m_flag09 || s->m_value0c || s->m_value10 || s->m_flag14 || s->m_words.m_value18 || s->m_words.m_value1c || s->m_words.m_value20 || s->m_pOwner != &owner || (g_is32 && sizeof(CTrackedState) != 0x28)) { std::printf("00863750_TEST FAIL\n"); return 1; }
    g_trackingDisabled = 1; CTrackedState t(0); if (t.m_enabled || t.m_pOwner != 0) { std::printf("00863750_TEST FAIL\n"); return 1; }
    std::printf("00863750_TEST PASS\n");
    return 0;
}
