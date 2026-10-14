// CMarkedBufferOwner_00c43810::ReportMark() -- when the `__stdcall` callback at +0xd4 is set, read the buffer's 20-byte mark record (@ 0x00C442F0 on +0x54) and hand it to the callback with the context at +0xd8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C432F0, length 64 bytes.
// Callees: GetMark @ 0x00C442F0 (five words into the local; manifest "CTCPhysicsRigidBody::GetFacingVector" is propagated), the callback (`push context ; push &mark ; push this ; call [esi+0D4h]`,
// callee-cleaned). Shape: the landed Shutdown @ 0x00C43810's callback block on its own: a 0x14 frame for the record, `this` in esi. The chunk label "CGuiWindow::OnDelete" is propagated.
struct SBufferMark_00c43810 { long m_words[5]; };   // 20 bytes: GetMark @ 0x00C442F0 writes five words

struct CMarkedBuffer_00c43810 {                    // the landed CMarkedBuffer_00c44550
    void* m_pBlock;
    void* m_pCursor;
    void* m_pBuffer;
    void GetMark(SBufferMark_00c43810& out);       // 0x00C442F0
};

struct CMarkedBufferOwner_00c43810;
typedef void (__stdcall* PFNMARK_00c43810)(CMarkedBufferOwner_00c43810* owner, SBufferMark_00c43810* mark, void* context);

struct CMarkedBufferOwner_00c43810 {
    unsigned char          _pad[0x54];
    CMarkedBuffer_00c43810 m_buffer;               // +0x54
    unsigned char          _pad60[0x74];
    PFNMARK_00c43810       m_pfnMark;              // +0xd4
    void*                  m_pMarkContext;         // +0xd8
    void ReportMark();
};

void CMarkedBufferOwner_00c43810::ReportMark()
{
    if (m_pfnMark) {
        SBufferMark_00c43810 mark;
        m_buffer.GetMark(mark);
        m_pfnMark(this, &mark, m_pMarkContext);
    }
}
