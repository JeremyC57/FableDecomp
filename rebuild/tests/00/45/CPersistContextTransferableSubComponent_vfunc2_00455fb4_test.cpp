#include <cstdio>
#include <cstring>
// Behaviour of 0x00455FB4 (chunk 901 piece): writes only bit 2, leaving the other bits alone
struct CTransferableOwner_Retail { unsigned char _pad_0x00[0x24]; bool m_bit0 : 1; bool m_bit1 : 1; bool m_bit2 : 1; };
struct CPersistContext_TransferableSubComponent {
    virtual void vfunc_0() {}
    virtual bool vfunc_1() { return false; }
    virtual void vfunc_2(bool value);
    CTransferableOwner_Retail* m_pOwner;
};
void CPersistContext_TransferableSubComponent::vfunc_2(bool value) { m_pOwner->m_bit2 = value; }
int main()
{
    CTransferableOwner_Retail owner; std::memset(&owner, 0, sizeof owner); owner.m_bit0 = true;
    CPersistContext_TransferableSubComponent c; c.m_pOwner = &owner;
    c.vfunc_2(true);
    if (!owner.m_bit2 || !owner.m_bit0 || owner.m_bit1) { std::printf("00455fb4_TEST FAIL\n"); return 1; }
    c.vfunc_2(false);
    if (owner.m_bit2 || !owner.m_bit0) { std::printf("00455fb4_TEST FAIL\n"); return 1; }
    std::printf("00455fb4_TEST PASS\n"); return 0;
}