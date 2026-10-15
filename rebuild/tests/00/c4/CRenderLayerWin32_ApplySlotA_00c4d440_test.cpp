// Behaviour test for 0x00C4D440 CRenderLayerWin32::ApplySlotA: success/failure of the helper and the x/y match gate.
#include <cstdio>
struct SStateSlot { long id; long x, y, z, w; };
long g_slotStateA = 0, g_slotStateB = 0, g_sharedSlotState = 0;
static long g_helperResult = 0; static long g_lastSet = -1;
struct L {
    SStateSlot m_slotA, m_slotB;
    long UpdateSlot(long, SStateSlot*, long*, long*, long set, long) { g_lastSet = set; return g_helperResult; }
    long ApplySlotA(long a, long b)
    {
        SStateSlot& src = m_slotA;
        bool same = src.x == m_slotB.x && src.y == m_slotB.y;
        long result = UpdateSlot(a, &src, &g_slotStateA, &g_sharedSlotState, 0, b);
        if (result == 0 && same) {
            m_slotB.x = src.x; m_slotB.y = src.y; m_slotB.z = src.z; m_slotB.w = src.w;
            g_slotStateB = g_slotStateA;
        }
        return result;
    }
};
int main()
{
    L l; l.m_slotA.x = 1; l.m_slotA.y = 2; l.m_slotA.z = 3; l.m_slotA.w = 4; l.m_slotB = l.m_slotA; l.m_slotB.z = 0; l.m_slotB.w = 0;
    l.m_slotA.z = 30; l.m_slotA.w = 40; g_slotStateA = 7; g_slotStateB = 0;
    if (l.ApplySlotA(0, 0) != 0 || l.m_slotB.z != 30 || l.m_slotB.w != 40 || g_slotStateB != 7 || g_lastSet != 0) { std::printf("00c4d440_TEST FAIL\n"); return 1; }
    l.m_slotB.z = -1; g_slotStateB = 0; g_helperResult = 5;
    if (l.ApplySlotA(0, 0) != 5 || l.m_slotB.z != -1 || g_slotStateB != 0) { std::printf("00c4d440_TEST FAIL\n"); return 1; }
    g_helperResult = 0; l.m_slotB.x = 99;
    if (l.ApplySlotA(0, 0) != 0 || l.m_slotB.z != -1) { std::printf("00c4d440_TEST FAIL\n"); return 1; }
    std::printf("00c4d440_TEST PASS\n");
    return 0;
}