// CPersistContext_TransferableSubComponent::vfunc_2(bool value) -- write bit 2 of the owner's (+0x04) flag byte at +0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455FB4, length 22 bytes (26 with padding).
// Vtable 0x012338FC (CPersistContext_TransferableSubComponent, rtti-vtables.tsv) slot 2; the getter is slot 1
// (0x00455FA8). Part of the skipped-ledger row 00455fa0 (three bodies, chunk 901).
// A size-optimised TU (#pragma optimize("s", on)): the read-modify-write goes straight through memory.
// Shape: `mov cl,[esp+4] ; shl cl,2 ; xor cl,[eax+24h] ; and cl,4 ; xor [eax+24h],cl ; ret 4` -- the VC7.1 one-bit
// bool bitfield store from a bool argument.
#pragma optimize("s", on)
struct CTransferableOwner_Retail {              // the object at +0x04
    unsigned char _pad_0x00[0x24];
    bool m_bit0 : 1;                           // +0x24 bit 0
    bool m_bit1 : 1;                           //       bit 1
    bool m_bit2 : 1;                           //       bit 2 (this slot's flag)
};

struct CPersistContext_TransferableSubComponent {   // vtable 0x012338FC
    virtual void vfunc_0();
    virtual bool vfunc_1();
    virtual void vfunc_2(bool value);
    CTransferableOwner_Retail* m_pOwner;       // +0x04
};

void CPersistContext_TransferableSubComponent::vfunc_2(bool value)
{
    m_pOwner->m_bit2 = value;
}
