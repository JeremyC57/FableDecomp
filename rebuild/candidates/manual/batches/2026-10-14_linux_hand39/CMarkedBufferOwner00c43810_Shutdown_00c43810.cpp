// CMarkedBufferOwner_00c43810::Shutdown() -- clean the owner up, clear the marked buffer at +0x54, report its mark record (20 bytes, read by 0x00C442F0) through the `__stdcall` callback at +0xd4 (with the context at +0xd8) when one is set, then release the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C43810, length 76 bytes.
// Callees: the cleanup @ 0x00C436C0 (on `this`; the manifest label "Thing_Cleanup"), the buffer's ClearMarks @ 0x00C443D0, GetMark @ 0x00C442F0 (the 20-byte
// local by address) and Release @ 0x00C44550 (all on +0x54; the landed CMarkedBuffer_00c44550), the callback (`push context ; push &local ; push this ; call
// [esi+0D4h]`). Shape: a 0x14-byte frame for the vector, the buffer address kept in edi. The chunk label "CExclusiveAssert::~CExclusiveAssert" is propagated
// (the 0x00C4xxxx module); the class is named by address.
struct SBufferMark_00c43810 { long m_words[5]; };   // 20 bytes: GetMark @ 0x00C442F0 writes five words (+0/+4/+8 from the buffer's +8/+0/+4, +0xc/+0x10 from its +0x10/+0x14)

struct CMarkedBuffer_00c43810 {                    // the landed CMarkedBuffer_00c44550
    void* m_pBlock;
    void* m_pCursor;
    void* m_pBuffer;
    void ClearMarks();                             // 0x00C443D0
    void GetMark(SBufferMark_00c43810& out);      // 0x00C442F0
    void Release();                                // 0x00C44550
};

struct CMarkedBufferOwner_00c43810;
typedef void (__stdcall* PFNMARK_00c43810)(CMarkedBufferOwner_00c43810* owner, SBufferMark_00c43810* mark, void* context);

struct CMarkedBufferOwner_00c43810 {
    unsigned char          _pad[0x54];
    CMarkedBuffer_00c43810 m_buffer;               // +0x54
    unsigned char          _pad60[0x74];
    PFNMARK_00c43810     m_pfnMark;            // +0xd4
    void*                  m_pMarkContext;       // +0xd8
    void Cleanup();                                // 0x00C436C0
    void Shutdown();
};

void CMarkedBufferOwner_00c43810::Shutdown()
{
    Cleanup();
    m_buffer.ClearMarks();
    if (m_pfnMark) {
        SBufferMark_00c43810 mark;
        m_buffer.GetMark(mark);
        m_pfnMark(this, &mark, m_pMarkContext);
    }
    m_buffer.Release();
}
