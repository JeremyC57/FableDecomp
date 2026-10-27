// CMorphAction_00c3dcd0::CMorphAction_00c3dcd0() -- a pooled character-morph action whose +0x48 word starts at 0x80000000
// Leaf: ??0CMorphAction_00c3dcd0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3CE50, 25 bytes, thiscall, plain `ret`.
// Identity (descriptive): it stores vtable 0x012B5A80 (no RTTI in this module) after the list-node base constructor
//   0x00C4CF20; its only reference is the `eh vector constructor iterator` push in 0x00C3DFD0, which builds an array of
//   0x4C-byte elements (destructor 0x00C3D7B0). The landed factory 0x00C3DCD0 builds the same 0x4C-byte class and binds this
//   vtable as ??_7CMorphAction_00c3dcd0@@6B@ and the base as CBase_00c4cf20, so those names are kept (one retail address, one
//   name). The manifest's "C3DAnimationBlendState::C3DAnimationBlendState" is propagated.
// Callees: CBase_00c4cf20::CBase_00c4cf20() 0x00C4CF20.
// Shape (family M193, representative 0x006920F0): `CMorphAction_00c3dcd0() : m_48(0x80000000) { }`. Speed TU.
class CBase_00c4cf20 {                                     // 0x48 bytes (vptr, prev +4, next +8, ...)
public:
    CBase_00c4cf20();                                     // 0x00C4CF20
    virtual ~CBase_00c4cf20();
    void* Field04;                                        // +0x04
    void* Field08;                                        // +0x08
    unsigned char _pad0C[0x3C];                           // +0x0C
};
class CMorphAction_00c3dcd0 : public CBase_00c4cf20 {      // vtable 0x012B5A80, 0x4C bytes
public:
    CMorphAction_00c3dcd0();
    virtual ~CMorphAction_00c3dcd0();
    long m_48;                                            // +0x48
};
CMorphAction_00c3dcd0::CMorphAction_00c3dcd0()
    : m_48(0x80000000)
{
}