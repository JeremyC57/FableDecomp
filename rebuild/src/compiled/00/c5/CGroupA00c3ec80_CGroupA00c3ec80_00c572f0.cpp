// CGroupA_00c3ec80::CGroupA_00c3ec80() -- store the vtable and zero the three member dwords
// Leaf: ??0CGroupA_00c3ec80@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c572f0, 20 bytes, thiscall, `ret`.
// Identity (descriptive): it stores vtable 0x012B8DF8 (no RTTI) and zeroes +0x08, +0x04 and +0x10 in that order; callers 0x00C3EC80 and the landed 0x00C56D30 (which calls this base C3DGroup2, but Ego_r.pdb C3DGroup2 has no vtable -- BoneIndex at +0); the landed destructor of the same vtable, 0x00C57310, binds it as ??_7CDisplayManager@@6B@, but CDisplayManager's RTTI vtable is 0x0129B3F8 and its Ego_r.pdb layout (748 bytes, D3DDevice at +0x58) does not fit; the class name is the existing descriptive binding of this address (??0CGroupA_00c3ec80@@QAE@XZ).
// Shape (family L109): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x012B8DF8, the three zero stores, `this` returned.
class CGroupA_00c3ec80 {
public:
    CGroupA_00c3ec80();
    virtual ~CGroupA_00c3ec80();
    void* Field04;                                         // +0x04
    void* Field08;                                         // +0x08 (the landed destructor 0x00C57310 releases a COM interface here)
    void* Field0C;                                         // +0x0C (not touched by the constructor)
    void* Field10;                                         // +0x10
};
CGroupA_00c3ec80::CGroupA_00c3ec80()
{
    Field08 = 0;
    Field04 = 0;
    Field10 = 0;
}