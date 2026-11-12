// Behaviour test for 0x00A04630 CRenderStateManager::SetupRenderStates (copy of the model's logic).
#include <cstdio>
#include <cstring>

struct CStateInfo { unsigned long CurrentState, DesiredState, BookmarkMask, Param; bool DirtyListFlag; unsigned char StateType, TextureStage; };
struct Model {
    unsigned long RasterCaps;
    unsigned long InitFlags;
    CStateInfo* AllStatesList[256];
    long AllStatesListSize;
    CStateInfo Render[64];
    CStateInfo Stage[21][8];
    static unsigned long WrapTable[8];
    void SR(CStateInfo& i, unsigned long s) { i.CurrentState = 0; i.DesiredState = 0; i.DirtyListFlag = false; i.StateType = 1; i.Param = s; AllStatesList[AllStatesListSize++] = &i; }
    void ST(CStateInfo& i, unsigned long t, long st) { i.CurrentState = 0; i.DesiredState = 0; i.DirtyListFlag = false; i.StateType = 2; i.Param = t; i.TextureStage = (unsigned char)st; AllStatesList[AllStatesListSize++] = &i; }
    void SS(CStateInfo& i, unsigned long t, long st) { i.CurrentState = 0; i.DesiredState = 0; i.DirtyListFlag = false; i.StateType = 3; i.Param = t; i.TextureStage = (unsigned char)st; AllStatesList[AllStatesListSize++] = &i; }
    void Setup()
    {
        static const unsigned long rs[47] = {9,8,0xA8,0xE,0x17,7,0x16,0x13,0x14,0x1B,0xF,0x19,0x18,0x97,0x1C,0x1A,0x24,0x25,0x26,0x22,0x23,0x8C,0x3C,0x1D,0x89,0x8F,0x93,0x91,0x92,0x9C,0x9D,0x9A,0x9B,0xA6,0x9E,0x9F,0xA0,0xAB,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0};
        int r = 0;
        for (int k = 0; k < 46; ++k) SR(Render[r++], rs[k]);
        for (int k = 0; k < 8; ++k) SR(Render[r++], WrapTable[k]);
        SR(Render[r++], 0x88);
        if (RasterCaps & 0x04000000) SR(Render[r++], 0xC3);
        if (RasterCaps & 0x02000000) SR(Render[r++], 0xAF);
        for (long s = 0; s < 8; ++s) {
            SS(Stage[0][s], 7, s); SS(Stage[1][s], 1, s); SS(Stage[2][s], 2, s); SS(Stage[3][s], 3, s); SS(Stage[4][s], 5, s); SS(Stage[5][s], 6, s);
            if (RasterCaps & 0x2000) SS(Stage[6][s], 8, s);
            SS(Stage[7][s], 4, s); SS(Stage[8][s], 10, s);
            ST(Stage[9][s], 1, s); ST(Stage[10][s], 4, s); ST(Stage[11][s], 2, s); ST(Stage[12][s], 3, s); ST(Stage[13][s], 5, s); ST(Stage[14][s], 6, s); ST(Stage[15][s], 11, s);
            if (!(InitFlags & 2)) ST(Stage[16][s], 24, s);
            ST(Stage[17][s], 7, s); ST(Stage[18][s], 9, s); ST(Stage[19][s], 8, s); ST(Stage[20][s], 10, s);
        }
    }
};
unsigned long Model::WrapTable[8] = {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87};

static int run(unsigned long caps, unsigned long flags, long expect)
{
    static Model m;
    std::memset(&m, 0xCD, sizeof(m));
    m.RasterCaps = caps; m.InitFlags = flags; m.AllStatesListSize = 0;
    m.Setup();
    if (m.AllStatesListSize != expect) return 0;
    for (long i = 0; i < m.AllStatesListSize; ++i) {
        CStateInfo* p = m.AllStatesList[i];
        if (p->CurrentState != 0 || p->DesiredState != 0 || p->DirtyListFlag) return 0;
        if (p->StateType < 1 || p->StateType > 3) return 0;
    }
    if (m.AllStatesList[46]->Param != 0x80 || m.AllStatesList[53]->Param != 0x87 || m.AllStatesList[54]->Param != 0x88) return 0;
    CStateInfo* last = m.AllStatesList[expect - 1];
    if (last->StateType != 2 || last->Param != 10 || last->TextureStage != 7) return 0;
    return 1;
}

int main()
{
    if (!run(0x06002000, 0, 55 + 2 + 8 * 21) || !run(0, 2, 55 + 8 * 19) || !run(0x04000000, 0, 56 + 8 * 20)) {
        std::printf("00a04630_TEST FAIL\n");
        return 1;
    }
    std::printf("00a04630_TEST PASS\n");
    return 0;
}
