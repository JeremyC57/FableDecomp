// Behaviour test for 0x00C4D4B0 CRenderLayerWin32::ApplySlotB: success/failure of the helper and the x/y match gate.
#include <cstdio>
struct SStateSlot { long id; long x, y, z, w; };
long g_slotStateA = 0, g_slotStateB = 0, g_sharedSlotState = 0;
static long g_helperResult = 0; static long g_lastSet = -1;
struct L {
    SStateSlot m_slotA, m_slotB;
    long UpdateSlot(long, SStateSlot*, long*, long*, long set, long) { g_lastSet = set; return g_helperResult; }
    long ApplySlotB(long a, long b)
    {
        SStateSlot& src = m_slotB;
        bool same = m_slotA.x == src.x && m_slotA.y == src.y;
        long result = UpdateSlot(a, &src, &g_slotStateB, &g_sharedSlotState, 1, b);
        if (result == 0 && same) {
            m_slotA.x = src.x; m_slotA.y = src.y; m_slotA.z = src.z; m_slotA.w = src.w;
            g_slotStateA = g_slotStateB;
        }
        return result;
    }
};
int main()
{
    L l; l.m_slotA.x = 1; l.m_slotA.y = 2; l.m_slotA.z = 3; l.m_slotA.w = 4; l.m_slotB = l.m_slotA; l.m_slotB.z = 0; l.m_slotB.w = 0;
    l.m_slotB.z = 30; l.m_slotB.w = 40; g_slotStateB = 7; g_slotStateA = 0;
    if (l.ApplySlotB(0, 0) != 0 || l.m_slotA.z != 30 || l.m_slotA.w != 40 || g_slotStateA != 7 || g_lastSet != 1) { std::printf("00c4d4b0_TEST FAIL\n"); return 1; }
    l.m_slotA.z = -1; g_slotStateA = 0; g_helperResult = 5;
    if (l.ApplySlotB(0, 0) != 5 || l.m_slotA.z != -1 || g_slotStateA != 0) { std::printf("00c4d4b0_TEST FAIL\n"); return 1; }
    g_helperResult = 0; l.m_slotB.x = 99;
    if (l.ApplySlotB(0, 0) != 0 || l.m_slotA.z != -1) { std::printf("00c4d4b0_TEST FAIL\n"); return 1; }
    std::printf("00c4d4b0_TEST PASS\n");
    return 0;
}
