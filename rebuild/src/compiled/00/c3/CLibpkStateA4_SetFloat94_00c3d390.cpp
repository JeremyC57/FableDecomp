// CLibpkStateA4::SetFloat94(float value) -- descriptive: when the value differs from the stored one (+0x94), store it, keep its scaled copy (+0x98 = value / 330, the pooled float 0x012B5AAC) and raise dirty bit 0x10000 in the flags word at +0xA4
// Leaf: ?SetFloat94@CLibpkStateA4@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3D390, 65 bytes, thiscall `ret 4`.
// Identity: reached only through the libpk C-API forwarder 0x00C33660 (`mov ecx,0143BB58h ; jmp`), like the landed
//   CLibpkStateA4::SetFloat90 0x00C3D360 / SetFloat9C 0x00C3D3E0 / SetFloatA0 0x00C3D410 on the same flags word; the class
//   and member names follow those rows. The scale constant 0x012B5AAC is 0.0030303f = 1.0f/330.0f. Descriptive (libpk).
// Shape: `if (field != value)` (fld value; fld field; fucompp); a LITERAL constant gives `fld value ; fmul [pool]` ahead of
//   the dword copy (an extern float global, either operand order, loads the global first and multiplies after the store).

class CLibpkStateA4 {
public:
    unsigned char _pad00[0x90];
    float         Float90;        // +0x90
    float         Float94;        // +0x94
    float         Float98;        // +0x98
    float         Float9C;        // +0x9C
    float         FloatA0;        // +0xA0
    unsigned long DirtyFlags;      // +0xA4
    void SetFloat94(float value);
};

void CLibpkStateA4::SetFloat94(float value)
{
    if (Float94 != value) {
        Float94 = value;
        Float98 = value * (1.0f / 330.0f);
        DirtyFlags |= 0x10000;
    }
}