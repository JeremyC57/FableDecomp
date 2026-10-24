// CInputTypeDataRecord::SetValueB4(unsigned long value) -- store one dword member at +0xB4
// Retail 0x0066F9E0, 13 bytes: `mov eax,[esp+4] ; mov [ecx+0B4h],eax ; ret 4`.
// An out-of-line inline setter emitted into the input-type TU (0x0066F7D0..0x0066FA50:
// CInputType*Event::GetControlDirection and friends). Fable.exe holds no call site, jump
// or absolute reference to this address (dead COMDAT kept by the linker), so the owning
// class cannot be pinned from callers; the class name is descriptive and the member is
// modelled at its retail offset. __thiscall with one stack argument (`ret 4`).
// Leaf: SetValueB4

class CInputTypeDataRecord
{
public:
    void SetValueB4(unsigned long value);

    unsigned long m_header[0xB4 / 4];   // +0x00..+0xB3 (members not touched here)
    unsigned long m_valueB4;            // +0xB4
};

void CInputTypeDataRecord::SetValueB4(unsigned long value)
{
    m_valueB4 = value;
}
