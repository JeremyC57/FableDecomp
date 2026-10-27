// CMember_00c34b30::CMember_00c34b30() -- store the vtable and zero the three member dwords
// Leaf: ??0CMember_00c34b30@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c34b30, 20 bytes, thiscall, `ret`.
// Identity (descriptive): it stores vtable 0x012B4FEC, which has no RTTI (the class is compiled without it), and zeroes three dwords; callers the landed constructors 0x00C1F8A0 / 0x00C1FB00 / 0x00C1FD60 (their +0x08 member); the class name is the existing descriptive binding of this address (reloc_bindings ??0CMember_00c34b30@@QAE@XZ).
// Shape (family L109): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x012B4FEC, the three zero stores, `this` returned.
class CMember_00c34b30 {
public:
    CMember_00c34b30();
    virtual ~CMember_00c34b30();
    void* Field04;                                         // +0x04
    void* Field08;                                         // +0x08
    void* Field0C;                                         // +0x0C
};
CMember_00c34b30::CMember_00c34b30()
    : Field04(0), Field08(0), Field0C(0)
{
}