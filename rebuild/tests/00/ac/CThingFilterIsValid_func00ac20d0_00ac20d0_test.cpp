#include <cstdio>
// Behaviour of func_00ac20d0 (0x00AC20D0): forwards to the object's virtual slot 4.
static int g_hits = 0;
struct CSlot4Owner_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual void vfunc_4() { ++g_hits; }
    void func_00ac20d0();
};
void CSlot4Owner_Retail::func_00ac20d0() { vfunc_4(); }
int main()
{
    CSlot4Owner_Retail o; o.func_00ac20d0(); o.func_00ac20d0(); o.func_00ac20d0();
    if (g_hits != 3) { std::printf("00ac20d0_TEST FAIL\n"); return 1; }
    std::printf("00ac20d0_TEST PASS\n"); return 0;
}