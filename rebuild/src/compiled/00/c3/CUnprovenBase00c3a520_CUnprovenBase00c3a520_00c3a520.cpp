// CUnprovenBase_00c3a520::CUnprovenBase_00c3a520() -- store the vtable and clear +0x04 and +0x08
// Leaf: ??0CUnprovenBase_00c3a520@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c3a520, 17 bytes, thiscall, plain `ret`.
// Identity (descriptive): it stores vtable 0x012B54C4 (no RTTI) and zeroes +0x04 / +0x08; callers 0x00C39130 and the landed 0x00C3A780 (as its base); unbound, nothing names the class.
// Shape (family L114): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x012B54C4, two zero stores, `this` returned.
class CUnprovenBase_00c3a520 {
public:
    CUnprovenBase_00c3a520();
    virtual ~CUnprovenBase_00c3a520();
    void* Field04;                                         // +0x04
    void* Field08;                                         // +0x08
};
CUnprovenBase_00c3a520::CUnprovenBase_00c3a520()
    : Field04(0), Field08(0)
{
}