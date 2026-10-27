// CUnprovenBase_00c39d30::CUnprovenBase_00c39d30() -- store the vtable and clear +0x04 and +0x08
// Leaf: ??0CUnprovenBase_00c39d30@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c39d30, 17 bytes, thiscall, plain `ret`.
// Identity (descriptive): it stores vtable 0x012B5484 (no RTTI) and zeroes +0x04 / +0x08; callers 0x00C39130 and the landed 0x00C3A700, which binds it as the free __fastcall ?CThingNoiseDef_baseTransfer@@YIXPAX@Z (it is a constructor: `this` in ecx, returned in eax); nothing names the class.
// Shape (family L114): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x012B5484, two zero stores, `this` returned.
class CUnprovenBase_00c39d30 {
public:
    CUnprovenBase_00c39d30();
    virtual ~CUnprovenBase_00c39d30();
    void* Field04;                                         // +0x04
    void* Field08;                                         // +0x08
};
CUnprovenBase_00c39d30::CUnprovenBase_00c39d30()
    : Field04(0), Field08(0)
{
}