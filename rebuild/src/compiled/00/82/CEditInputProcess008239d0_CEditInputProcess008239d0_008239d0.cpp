// CEditInputProcess_008239d0::CEditInputProcess_008239d0() -- set the enabled byte at +4 and build the member at +9 with its default constructor @ 0x0050EE40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008239D0, length 19 bytes.
// Callees: the member's default constructor @ 0x0050EE40 (manifest "CLayer::SetAllPolys" is propagated). Shape: the byte store is scheduled between the
// member's `lea` and its call; returns `this`. The chunk label "CEditInputProcessCopyPaste::AddToInterface" is propagated (the body is a constructor:
// `mov eax,esi` return, no arguments); the class is named by address. The member at the odd offset +9 follows packed bytes.
struct CInputState_0050ee40 {
    unsigned char _body[3];
    CInputState_0050ee40();                        // 0x0050EE40
};

#pragma pack(push, 1)
struct CEditInputProcess_008239d0 {
    void*                __vftable;                // +0x00 (not stored here: the vtable belongs to a novtable root)
    unsigned char        m_enabled;                // +0x04
    unsigned char        _pad05[4];                // +0x05
    CInputState_0050ee40 m_state;                  // +0x09
    CEditInputProcess_008239d0();
};
#pragma pack(pop)

CEditInputProcess_008239d0::CEditInputProcess_008239d0()
    : m_enabled(1), m_state()
{
}