// CPersistContext_TransferableSubComponent::vfunc_1() -- read bit 2 of the owner's (+0x04) flag byte at +0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455FA8, length 12 bytes (no padding: slot 2 follows at 0x00455FB4).
// Vtable 0x012338FC (CPersistContext_TransferableSubComponent, rtti-vtables.tsv) slot 1; the setter is slot 2
// (0x00455FB4). Part of the skipped-ledger row 00455fa0 (three bodies, chunk 901).
// Shape: `mov eax,[ecx+4] ; mov al,[eax+24h] ; shr al,2 ; and al,1 ; ret` -- a one-bit bool bitfield read.
struct CTransferableOwner_Retail {              // the object at +0x04
    unsigned char _pad_0x00[0x24];
    bool m_bit0 : 1;                           // +0x24 bit 0
    bool m_bit1 : 1;                           //       bit 1
    bool m_bit2 : 1;                           //       bit 2 (this slot's flag)
};

struct CPersistContext_TransferableSubComponent {   // vtable 0x012338FC
    virtual void vfunc_0();
    virtual bool vfunc_1();
    CTransferableOwner_Retail* m_pOwner;       // +0x04
};

bool CPersistContext_TransferableSubComponent::vfunc_1()
{
    return m_pOwner->m_bit2;
}
