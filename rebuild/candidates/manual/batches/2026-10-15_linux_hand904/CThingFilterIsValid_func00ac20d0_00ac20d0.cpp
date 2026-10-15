// CThingFilter_IsValid::func_00ac20d0() -- forward to the object's own virtual slot 4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC20D0, length 5 bytes (16 with padding). Callers 0x00A90887, 0x00A94860
// (tail jump), 0x00A95666, 0x00BC9D4C, 0x00BCA1EF, 0x00BCA2E2, 0x00BCA62F ... (an engine-library object). The catalogue
// row 00ac20d0 (38 bytes) held this body, a word getter at 0x00AC20E0 and a descriptor getter at 0x00AC20F0 (skipped
// ledger, chunk 904); the class label is propagated, so the owner is modelled locally.
// Shape: `mov eax,[ecx] ; jmp [eax+10h]`.
struct CSlot4Owner_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void vfunc_4();                    // slot 4 (+0x10)
    void func_00ac20d0();
};

void CSlot4Owner_Retail::func_00ac20d0()
{
    vfunc_4();
}
