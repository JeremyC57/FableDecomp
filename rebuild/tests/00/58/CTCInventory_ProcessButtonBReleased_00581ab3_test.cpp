#include <cstdio>
#include <cstring>
// Behaviour of 0x00581AB3 (chunk 901 piece): mode 0 -> slot 32, mode 1 -> slot 46(0), nothing when a suppress flag is set
static int g_s32 = 0, g_s46 = 0; static long g_arg = -1;
struct CTCInventory {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long v06() { return 6; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual long v09() { return 9; }
    virtual long v10() { return 10; }
    virtual long v11() { return 11; }
    virtual long v12() { return 12; }
    virtual long v13() { return 13; }
    virtual long v14() { return 14; }
    virtual long v15() { return 15; }
    virtual long v16() { return 16; }
    virtual long v17() { return 17; }
    virtual long v18() { return 18; }
    virtual long v19() { return 19; }
    virtual long v20() { return 20; }
    virtual long v21() { return 21; }
    virtual long v22() { return 22; }
    virtual long v23() { return 23; }
    virtual long v24() { return 24; }
    virtual long v25() { return 25; }
    virtual long v26() { return 26; }
    virtual long v27() { return 27; }
    virtual long v28() { return 28; }
    virtual long v29() { return 29; }
    virtual long v30() { return 30; }
    virtual long v31() { return 31; }
    virtual void vfunc_32() { ++g_s32; }
    virtual long v33() { return 33; }
    virtual long v34() { return 34; }
    virtual long v35() { return 35; }
    virtual long v36() { return 36; }
    virtual long v37() { return 37; }
    virtual long v38() { return 38; }
    virtual long v39() { return 39; }
    virtual long v40() { return 40; }
    virtual long v41() { return 41; }
    virtual long v42() { return 42; }
    virtual long v43() { return 43; }
    virtual long v44() { return 44; }
    virtual long v45() { return 45; }
    virtual void vfunc_46(long arg) { ++g_s46; g_arg = arg; }
    unsigned char _pad_0x04[0x1c - 0x04];
    long m_menuMode;
    unsigned char _pad_0x20[0x1cc - 0x20];
    bool m_suppressA;
    bool m_suppressB;
    void ProcessButtonBReleased();
};
void CTCInventory::ProcessButtonBReleased()
{
    if (!m_suppressA && !m_suppressB) {
        switch (m_menuMode) {
        case 0: vfunc_32(); break;
        case 1: vfunc_46(0); break;
        }
    }
}
int main()
{
    CTCInventory inv; inv.m_suppressA = false; inv.m_suppressB = false;
    inv.m_menuMode = 0; inv.ProcessButtonBReleased();
    inv.m_menuMode = 1; inv.ProcessButtonBReleased();
    inv.m_menuMode = 2; inv.ProcessButtonBReleased();
    inv.m_suppressB = true; inv.m_menuMode = 0; inv.ProcessButtonBReleased();
    inv.m_suppressB = false; inv.m_suppressA = true; inv.m_menuMode = 1; inv.ProcessButtonBReleased();
    if (g_s32 != 1 || g_s46 != 1 || g_arg != 0) { std::printf("00581ab3_TEST FAIL\n"); return 1; }
    std::printf("00581ab3_TEST PASS\n"); return 0;
}