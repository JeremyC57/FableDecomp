// CVtable_012b5c10::CVtable_012b5c10() -- CIndexBuffer base (@ 0x00C4CF20), own vtable (0x012B5C10, a no-RTTI module) and the sentinel 0x80000000 at +0x2c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59BA0, length 25 bytes.
// Callees: CIndexBuffer::CIndexBuffer @ 0x00C4CF20 (vtable 0x012B8778, nulls +4 and +8).
// The manifest label (C3DAnimationBlendState::C3DAnimationBlendState) is propagated; the base
// is identified by its constructor and engine/CIndexBuffer.h (0x14 bytes: vptr, PD3DIndexBuffer,
// ByteLength, PrimitiveType, PAllocatedBuffer). The vtable has no complete-object locator, so the
// derived class is named by its vtable address; +0x2c holds 0x80000000 (an "unset" sentinel).
struct CIndexBuffer {                      // engine/CIndexBuffer.h layout
    virtual void v00();
    void* PD3DIndexBuffer;                 // +4
    unsigned long ByteLength;              // +8
    long PrimitiveType;                    // +0xc
    void* PAllocatedBuffer;                // +0x10
    CIndexBuffer();                        // 0x00C4CF20
};

struct CVtable_012b5c10 : CIndexBuffer {
    unsigned char _pad_0x14[0x18];         // +0x14 (left as the base constructor leaves it)
    unsigned long m_lockOffset;            // +0x2c = 0x80000000
    CVtable_012b5c10();
};

CVtable_012b5c10::CVtable_012b5c10()
    : m_lockOffset(0x80000000)
{
}
