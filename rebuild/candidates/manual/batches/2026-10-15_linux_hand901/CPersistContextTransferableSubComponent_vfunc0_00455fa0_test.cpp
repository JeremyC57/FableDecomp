#include <cstdio>
#include <cstring>
// Behaviour of 0x00455FA0 (chunk 901 piece): slot 0 forwards to the owner's slot 18
static int g_hits = 0;
struct CTransferableOwner_Retail {
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
    virtual void vfunc_18() { ++g_hits; }
};
struct CPersistContext_TransferableSubComponent {
    virtual void vfunc_0();
    CTransferableOwner_Retail* m_pOwner;
};
void CPersistContext_TransferableSubComponent::vfunc_0() { m_pOwner->vfunc_18(); }
int main()
{
    CTransferableOwner_Retail owner; CPersistContext_TransferableSubComponent c; c.m_pOwner = &owner;
    c.vfunc_0(); c.vfunc_0();
    if (g_hits != 2) { std::printf("00455fa0_TEST FAIL\n"); return 1; }
    std::printf("00455fa0_TEST PASS\n"); return 0;
}
