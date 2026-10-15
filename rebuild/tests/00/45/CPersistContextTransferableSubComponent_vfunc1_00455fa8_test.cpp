#include <cstdio>
#include <cstring>
// Behaviour of 0x00455FA8 (chunk 901 piece): reads only bit 2 of the owner's +0x24 byte
struct CTransferableOwner_Retail { unsigned char _pad_0x00[0x24]; bool m_bit0 : 1; bool m_bit1 : 1; bool m_bit2 : 1; };
struct CPersistContext_TransferableSubComponent {
    virtual void vfunc_0() {}
    virtual bool vfunc_1();
    CTransferableOwner_Retail* m_pOwner;
};
bool CPersistContext_TransferableSubComponent::vfunc_1() { return m_pOwner->m_bit2; }
int main()
{
    CTransferableOwner_Retail owner; std::memset(&owner, 0, sizeof owner);
    CPersistContext_TransferableSubComponent c; c.m_pOwner = &owner;
    owner.m_bit0 = true; owner.m_bit1 = true;
    bool a = c.vfunc_1(); owner.m_bit2 = true; bool b = c.vfunc_1();
    if (a || !b) { std::printf("00455fa8_TEST FAIL\n"); return 1; }
    std::printf("00455fa8_TEST PASS\n"); return 0;
}