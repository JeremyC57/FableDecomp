// CBufferedReader_00c3aac0::CBufferedReader_00c3aac0(void* source, long size) -- construct the base reader @ 0x00C4B300 with the source and a {size, 0, 2} parameter record, then reset the three words at +0x70 (@ 0x00C3A9C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3AAC0, length 63 bytes.
// Callees: the base constructor @ 0x00C4B300 (EH-framed, stores vtable 0x012B8458; manifest "PKCBufferedReader_Initialize"; `(source, const SParams&)`), the reset @ 0x00C3A9C0
// (zeroes +0x70/+0x74/+0x78; manifest "CV_MurderTwistScript::Init" is propagated). Shape: a 12-byte frame for the parameter record (filled after its address and the source are pushed),
// `this` returned in eax, `ret 8`. The derived class stores no vtable of its own (`__declspec(novtable)`). The chunk label "CGuiWindowManager::AddRootWindow" is propagated; named by address.
// Leaf: ??0CBufferedReader_00c3aac0
struct SReaderParams_00c3aac0 {                    // 12 bytes
    long m_size;
    long m_offset;
    long m_mode;
    SReaderParams_00c3aac0(long size, long offset, long mode) : m_size(size), m_offset(offset), m_mode(mode) {}
};

struct CReaderBase_00c4b300 {                      // vtable 0x012B8458 (no RTTI module)
    virtual void v00();
    unsigned char _body[0x7c - 4];
    CReaderBase_00c4b300(void* source, const SReaderParams_00c3aac0& params);   // 0x00C4B300
    void Reset();                                  // 0x00C3A9C0
};

struct __declspec(novtable) CBufferedReader_00c3aac0 : CReaderBase_00c4b300 {
    CBufferedReader_00c3aac0(void* source, long size);
};

CBufferedReader_00c3aac0::CBufferedReader_00c3aac0(void* source, long size)
    : CReaderBase_00c4b300(source, SReaderParams_00c3aac0(size, 0, 2))
{
    Reset();
}
