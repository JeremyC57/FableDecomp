// CMember_00c34dd0::CMember_00c34dd0() -- store the vtable and zero the three member dwords
// Leaf: ??0CMember_00c34dd0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c34dd0, 20 bytes, thiscall, `ret`.
// Identity (descriptive): it stores vtable 0x012B501C (no RTTI) and zeroes three dwords; callers the landed constructors 0x00C1F960 / 0x00C1FBE0 / 0x00C1FE00; the class name is the existing descriptive binding of this address (??0CMember_00c34dd0@@QAE@XZ).
// Shape (family L109): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x012B501C, the three zero stores, `this` returned.
class CMember_00c34dd0 {
public:
    CMember_00c34dd0();
    virtual ~CMember_00c34dd0();
    void* Field04;                                         // +0x04
    void* Field08;                                         // +0x08
    void* Field0C;                                         // +0x0C
};
CMember_00c34dd0::CMember_00c34dd0()
    : Field04(0), Field08(0), Field0C(0)
{
}