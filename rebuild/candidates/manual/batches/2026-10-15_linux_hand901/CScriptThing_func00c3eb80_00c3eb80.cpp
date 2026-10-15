// CScriptThing::func_00c3eb80(long a0, long a1, long a2, long a3) -- forward to the +0x1d4 object's virtual slot 9, or -9 when it is unset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EB80, length 25 bytes (32 with padding). Caller 0x00C4F2B5.
// The catalogue row 00c3eb60 held this body and 0x00C3EB60 (slot 8, same shape) (skipped ledger, chunk 901).
// The manifest class is propagated: this is an audio-library object (0x00C3xxxx) with a polymorphic member at +0x1d4;
// `this` is modelled locally. -9 (0xFFFFFFF7) is the library's "no device" result.
// Shape: `mov eax,[ecx+1D4h] ; test eax,eax ; je E ; mov ecx,eax ; mov eax,[ecx] ; jmp [eax+0x24] ; E: mov eax,-9 ; ret 10h`.
struct CAudioDevice_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual long vfunc_9(long a0, long a1, long a2, long a3);   // slot 9
};

struct CAudioOwner_Retail {
    unsigned char       _pad_0x00[0x1d4];
    CAudioDevice_Retail* m_pDevice;            // +0x1d4
    long func_00c3eb80(long a0, long a1, long a2, long a3);
};

long CAudioOwner_Retail::func_00c3eb80(long a0, long a1, long a2, long a3)
{
    if (m_pDevice)
        return m_pDevice->vfunc_9(a0, a1, a2, a3);
    return -9;
}
