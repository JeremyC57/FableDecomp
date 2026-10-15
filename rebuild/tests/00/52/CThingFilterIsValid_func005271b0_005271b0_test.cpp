#include <cstdio>
// Behaviour of func_005271b0 (0x005271B0): forwards to the object's virtual slot 43.
static int g_hits = 0;
struct CSlot43Owner_Retail {
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
    virtual void vfunc_43() { ++g_hits; }
    void func_005271b0();
};
void CSlot43Owner_Retail::func_005271b0() { vfunc_43(); }
int main()
{
    CSlot43Owner_Retail o; o.func_005271b0(); o.func_005271b0();
    if (g_hits != 2) { std::printf("005271b0_TEST FAIL\n"); return 1; }
    std::printf("005271b0_TEST PASS\n"); return 0;
}