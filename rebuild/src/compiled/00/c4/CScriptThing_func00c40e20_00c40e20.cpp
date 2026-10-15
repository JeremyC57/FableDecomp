// CScriptThing::func_00c40e20() -- forward to the +0x7c object's virtual slot 6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40E20, length 8 bytes (16 with padding). Caller 0x00C3D5CB.
// The catalogue row 00c40e20 (56 bytes) held this body and three float maths wrappers (0x00C40E30 atan2,
// 0x00C40E40 log10, 0x00C40E50 pow) (skipped ledger, chunk 901). The manifest class is propagated: an
// audio-library object with a polymorphic member at +0x7c; `this` is modelled locally.
// Shape: `mov ecx,[ecx+7Ch] ; mov eax,[ecx] ; jmp [eax+18h]`.
struct CAudioChild_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void vfunc_6();                    // slot 6 (+0x18)
};

struct CAudioOwner7C_Retail {
    unsigned char      _pad_0x00[0x7c];
    CAudioChild_Retail* m_pChild;              // +0x7c
    void func_00c40e20();
};

void CAudioOwner7C_Retail::func_00c40e20()
{
    m_pChild->vfunc_6();
}