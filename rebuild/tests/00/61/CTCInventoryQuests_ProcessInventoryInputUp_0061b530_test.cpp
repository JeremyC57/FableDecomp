#include <cstdio>
// Behaviour of CTCInventoryQuests::ProcessInventoryInputUp (0x0061B530): the quest-list hook when the +0x157 flag is set; else mode 0 -> the base handler, mode 1 -> virtual slot 72, other modes nothing.
static char g_log[16]; static int g_n = 0;
struct CTCInventoryBase_Calls {
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
    virtual long v32() { return 32; }
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
    virtual long v46() { return 46; }
    virtual long v47() { return 47; }
    virtual long v48() { return 48; }
    virtual long v49() { return 49; }
    virtual long v50() { return 50; }
    virtual long v51() { return 51; }
    virtual long v52() { return 52; }
    virtual long v53() { return 53; }
    virtual long v54() { return 54; }
    virtual long v55() { return 55; }
    virtual long v56() { return 56; }
    virtual long v57() { return 57; }
    virtual long v58() { return 58; }
    virtual long v59() { return 59; }
    virtual long v60() { return 60; }
    virtual long v61() { return 61; }
    virtual long v62() { return 62; }
    virtual long v63() { return 63; }
    virtual long v64() { return 64; }
    virtual long v65() { return 65; }
    virtual long v66() { return 66; }
    virtual long v67() { return 67; }
    virtual long v68() { return 68; }
    virtual long v69() { return 69; }
    virtual long v70() { return 70; }
    virtual long v71() { return 71; }
    virtual void vfunc_72() { g_log[g_n++] = 'V'; }
    unsigned char _pad_0x04[0x1c - 0x04];
    long m_menuMode;
    void ProcessInventoryInputUp() { g_log[g_n++] = 'B'; }
};
struct CTCInventoryQuests : CTCInventoryBase_Calls {
    unsigned char _pad_0x20[0x157 - 0x20];
    bool m_questListActive;
    void QuestListHook() { g_log[g_n++] = 'H'; }
    void ProcessInventoryInputUp();
};
void CTCInventoryQuests::ProcessInventoryInputUp()
{
    if (m_questListActive) { QuestListHook(); return; }
    switch (m_menuMode) {
    case 0: CTCInventoryBase_Calls::ProcessInventoryInputUp(); break;
    case 1: vfunc_72(); break;
    }
}
int main()
{
    CTCInventoryQuests q; q.m_questListActive = true; q.m_menuMode = 0; q.ProcessInventoryInputUp();
    q.m_questListActive = false; q.ProcessInventoryInputUp();
    q.m_menuMode = 1; q.ProcessInventoryInputUp();
    q.m_menuMode = 2; q.ProcessInventoryInputUp();
    g_log[g_n] = 0;
    if (g_n != 3 || g_log[0] != 'H' || g_log[1] != 'B' || g_log[2] != 'V') { std::printf("0061b530_TEST FAIL %s\n", g_log); return 1; }
    std::printf("0061b530_TEST PASS\n"); return 0;
}