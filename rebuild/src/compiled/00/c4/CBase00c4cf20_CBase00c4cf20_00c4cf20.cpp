// CBase_00c4cf20::CBase_00c4cf20() -- store the vtable and clear +0x04 and +0x08
// Leaf: ??0CBase_00c4cf20@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c4cf20, 17 bytes, thiscall, plain `ret`.
// Identity (descriptive): it stores vtable 0x012B8778 (no RTTI; bound ??_7CVtable_012B8778@@6B@) and zeroes +0x04 / +0x08; it is the base constructor of the landed 0x00C3C590, 0x00C3CEA0, 0x00C3DCD0, 0x00C3DDE0, 0x00C42D60, 0x00C596A0 and 0x00C59BA0, which bind it under four stand-in names (CBase_00c4cf20 kept here) and, in 0x00C596A0, as CIndexBuffer (wrong: CIndexBuffer's RTTI constructor is 0x00AC1EC0).
// Shape (family L114): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x012B8778, two zero stores, `this` returned.
class CBase_00c4cf20 {
public:
    CBase_00c4cf20();
    virtual ~CBase_00c4cf20();
    void* Field04;                                         // +0x04
    void* Field08;                                         // +0x08
};
CBase_00c4cf20::CBase_00c4cf20()
    : Field04(0), Field08(0)
{
}