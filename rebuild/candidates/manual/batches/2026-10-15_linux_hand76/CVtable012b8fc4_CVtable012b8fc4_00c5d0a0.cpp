// CVtable_012b8fc4::CVtable_012b8fc4() -- this class's vtable, then the seven dwords +0x04..+0x1c zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D0A0, length 32 bytes.
// Leaf: ??0CVtable_012b8fc4
// The chunk label (CSoundInit::CSoundInit) is propagated; the vtable stored here, 0x012B8FC4, has no
// complete-object locator (the no-RTTI 0x00C5xxxx module), so the class is named after it.
// Shape: no frame, `mov eax,ecx ; xor ecx,ecx`, the vtable store, then seven `mov [eax+N],ecx`. The vtable
// store precedes the zero stores, so the zeroed words are one embedded member object with an inline
// constructor (plain init-list members sink the vtable store below them). Types not recoverable: `long`.
struct SZeroed_012b8fc4 {                       // +0x04..+0x1f
    long m_4, m_8, m_c, m_10, m_14, m_18, m_1c;
    SZeroed_012b8fc4() : m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0) { }
};

struct CVtable_012b8fc4 {
    virtual void v00();
    SZeroed_012b8fc4 m_data;                    // +0x04
    CVtable_012b8fc4();
};

CVtable_012b8fc4::CVtable_012b8fc4()
{
}
