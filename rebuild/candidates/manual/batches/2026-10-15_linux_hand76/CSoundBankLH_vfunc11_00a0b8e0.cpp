// CSoundBankLH::vfunc_11() -- forward to slot 1 of the object held at +0x10, or return 0 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B8E0, length 17 bytes (32 with padding).
// Vtable 0x0129C9DC (CSoundBankLH, rtti-vtables.tsv) slot 11. The landed constructor 0x00A0BAC0 zeroes the
// word at +0x10; here it is an object pointer whose slot 1 (+4) is called.
// Shape: null test, `mov ecx,eax ; mov eax,[ecx] ; jmp [eax+4]` (the virtual call is the tail call), the
// null arm `xor eax,eax ; ret` placed after it: a word-sized result.
struct CSoundBankObject_00a0b8e0 {
    virtual void v00();
    virtual long Get();                         // slot 1 (+0x04)
};

struct CSoundBankLH {                           // vtable 0x0129C9DC
    virtual void v00();
    long                      m_4;              // +0x04
    long                      m_8;              // +0x08
    long                      m_bankIndex;      // +0x0c
    CSoundBankObject_00a0b8e0* m_pObject;       // +0x10
    virtual long vfunc_11();
};

long CSoundBankLH::vfunc_11()
{
    if (m_pObject)
        return m_pObject->Get();
    return 0;
}
