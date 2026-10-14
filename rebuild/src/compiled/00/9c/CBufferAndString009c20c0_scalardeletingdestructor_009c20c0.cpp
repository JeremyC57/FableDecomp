// CBufferAndString_009c20c0::`scalar deleting destructor'(unsigned int) -- non-virtual: destroy the buffer holder at +0x18 and the wide string at +0x14 out of line (reverse order), then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C20C0, length 41 bytes.
// Callees: the holder destructor @ 0x009F9F70, CWideString::~CWideString @ 0x0099B510, operator delete @ 0x00BFE9BC. Shape: two `lea ecx ; call` pairs then
// `test byte ptr [esp+8],1`. The chunk label is propagated; the class is named by address.
struct CWideString {
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};

struct CBufferHolder_009f9f70 {
    unsigned char _body[0xc];
    ~CBufferHolder_009f9f70();                     // 0x009F9F70
};

struct CBufferAndString_009c20c0 {
    unsigned char          _pad[0x14];
    CWideString            m_name;                 // +0x14
    CBufferHolder_009f9f70 m_buffer;               // +0x18
    ~CBufferAndString_009c20c0() { }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CBufferAndString_009c20c0(CBufferAndString_009c20c0* object)
{
    delete object;
}