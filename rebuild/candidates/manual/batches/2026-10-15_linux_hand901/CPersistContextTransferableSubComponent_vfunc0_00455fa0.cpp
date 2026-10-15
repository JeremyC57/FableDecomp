// CPersistContext_TransferableSubComponent::vfunc_0() -- forward vtable slot 0 to the owner's (+0x04) virtual slot 18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455FA0, length 8 bytes (no padding: slot 1 follows at 0x00455FA8).
// Vtable 0x012338FC (CPersistContext_TransferableSubComponent, rtti-vtables.tsv) slot 0; slots 1 and 2 (0x00455FA8,
// 0x00455FB4) read and write bit 2 of the owner's flag byte at +0x24. The row was catalogued as one 42-byte function
// holding all three bodies (skipped ledger 00455fa0, chunk 901).
// Shape: `mov ecx,[ecx+4] ; mov eax,[ecx] ; jmp [eax+48h]` -- a tail call through the owner's vtable with the same
// (empty) argument list.
struct CTransferableOwner_Retail {              // the object at +0x04
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void vfunc_18();                   // slot 18 (+0x48)
};

struct CPersistContext_TransferableSubComponent {   // vtable 0x012338FC
    virtual void vfunc_0();
    CTransferableOwner_Retail* m_pOwner;       // +0x04
};

void CPersistContext_TransferableSubComponent::vfunc_0()
{
    m_pOwner->vfunc_18();
}
