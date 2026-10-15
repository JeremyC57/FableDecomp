#include <cstdio>
#include <cstring>
// Behaviour of 0x00C40E20 (chunk 901 piece): forwards to the +0x7c object's slot 6
static int g_hits = 0;
struct CAudioChild_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual void vfunc_6() { ++g_hits; }
};
struct CAudioOwner7C_Retail {
    unsigned char _pad_0x00[0x7c];
    CAudioChild_Retail* m_pChild;
    void func_00c40e20();
};
void CAudioOwner7C_Retail::func_00c40e20() { m_pChild->vfunc_6(); }
int main()
{
    CAudioChild_Retail c; CAudioOwner7C_Retail o; o.m_pChild = &c;
    o.func_00c40e20(); o.func_00c40e20();
    if (g_hits != 2) { std::printf("00c40e20_TEST FAIL\n"); return 1; }
    std::printf("00c40e20_TEST PASS\n"); return 0;
}
