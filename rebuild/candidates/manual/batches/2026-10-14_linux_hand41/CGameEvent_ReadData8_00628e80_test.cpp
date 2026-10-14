#include <cstdio>
#include <cstring>
#include <new>

struct SEventData8 { int m_low; int m_high; };   // 4-byte words (VC7.1 long; int keeps the test portable to LP64)
static long g_count;
struct CGameEvent {
    int m_a, m_b;
    void ReadBytes(long count, void* destination) { g_count = count; int v[2] = { m_a, m_b }; std::memcpy(destination, v, 8); }
    static SEventData8 ReadData8(CGameEvent* event) { SEventData8 value; event->ReadBytes(8, &value); return value; }
};
int main()
{
    CGameEvent ev; ev.m_a = 0x11223344; ev.m_b = -7;
    SEventData8 v = CGameEvent::ReadData8(&ev);
    if (sizeof(v) != 8 || g_count != 8 || v.m_low != 0x11223344 || v.m_high != -7) { std::printf("00628e80_TEST FAIL\n"); return 1; }
    std::printf("00628e80_TEST PASS\n");
    return 0;
}
